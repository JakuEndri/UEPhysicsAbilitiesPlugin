// Copyright Endri Jaku. All Rights Reserved.


#include "CharacterPhysicsAbilities.h"

#include "PhysicsInteractable.h"
#include "CharacterPhysicsAbilitiesWidget.h"

#include "Components/SphereComponent.h"
#include "Blueprint/UserWidget.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

USphereComponent* UCharacterPhysicsAbilities::GrabInteractable(const FHitResult& HitResult, USceneComponent* NewParentComponent, bool bGrabAtHitLocation)
{
	LOG_IF(AbilityState, "");
	//Grab fail conditions
	if (bGrabCooldown)
	{
		PlayGrabFailEffects();
		return nullptr;
	}
	if (AbilityState != EPAS_Ready)
	{
		PlayGrabFailEffects();
		return nullptr;
	}
	if (!HitResult.bBlockingHit)
	{
		SetGrabCooldown(true);
		GrabCooldownDelegate.BindLambda(
			[this]()
			{
				if (IsValid(this))
				{
					SetGrabCooldown(false);
				}
			}
		);
		TIMER_MANAGER.SetTimer(GrabTimer, GrabCooldownDelegate, GrabNoHitCooldownLength, false);
		if (CharacterWidget)
		{
			CharacterWidget->PlayGrabNoHitAnimation(GrabTimer);
		}
		GrabNoHit.PlayAtLocation(this, GetOwner()->GetActorLocation(), HitResult.TraceEnd);
		return nullptr;
	}

	HitInteractor = Cast<IPhysicsAbilitiesInteractable>(HitResult.GetActor());

	if (!HitInteractor)
	{
		LOG_IF(!HitInteractor, "HitInteractor is not APhysicsInteractable");
		SetGrabCooldown(true);
		GrabCooldownDelegate.BindLambda(
			[this]()
			{
				if (IsValid(this))
				{
					SetGrabCooldown(false);
				}
			}
		);
		TIMER_MANAGER.SetTimer(GrabTimer, GrabCooldownDelegate, GrabHitWrongObjectCooldownLength, false);
		if (CharacterWidget)
		{
			CharacterWidget->PlayGrabWrongObjectAnimation(GrabTimer);
		}
		GrabWrongObject.PlayAtLocation(this, GetOwner()->GetActorLocation(), HitResult.ImpactPoint + HitResult.ImpactNormal * DistanceFromImpactPoint);
		return nullptr;
	}
	if (!HitInteractor->Execute_CanBeGrabbed(HitResult.GetActor(), this, HitResult))
	{
		return nullptr;
	}

	USphereComponent* StaticControllerOfConstraint = GrabBasic(HitResult.GetComponent(), NewParentComponent, bGrabAtHitLocation, HitResult.ImpactPoint, HitResult.BoneName);
	//Grab Succeed condition
	//FUTURE ME: If(HitInteractor->Execute_OverrideGrabbEffects(HitResult.GetActor(), this, HitResult) //<-- Here if you want to add it 
	if (StaticControllerOfConstraint)
	{
		HitInteractor->Execute_Grabb(HitResult.GetActor(), this, HitResult);
		AbilityState = EPAS_Grabbing;
		//FUTURE ME: If(HitInteractor->Execute_OverrideGrabbEffects(HitResult.GetActor(), this, HitResult) //<-- Here if you want to add it
		SetGrabCooldown(true);
		GrabCooldownDelegate.BindLambda(
			[this]()
			{
				if (IsValid(this))
				{
					SetGrabCooldown(false);
				}
			}
		);
		TIMER_MANAGER.SetTimer(GrabTimer, GrabCooldownDelegate, GrabSucceededCooldownLength, false);
		//Play effects
		if (CharacterWidget)
		{
			CharacterWidget->PlayGrabSucceedAnimation(GrabTimer);
		}
		GrabSuccess.Niagara.SpawnSystemAtAttached(HitResult.GetComponent(), HitResult.BoneName, HitResult.ImpactPoint + HitResult.ImpactNormal * DistanceFromImpactPoint, true);
		GrabSuccess.Sound.PlaySoundAtLocation(this, GetOwner()->GetActorLocation());
		GrabSuccess.CameraShake.StartCameraShake(this);
	}
	return StaticControllerOfConstraint;
}

void UCharacterPhysicsAbilities::PlayGrabFailEffects()
{
	GrabFail.PlayAtLocation(this, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation());
	//Not sure if I want to not play the widget animations if is already grabbing something
	//if (AbilityState == EPAS_Grabbing) return nullptr;
	if (CharacterWidget)
	{
		FTimerHandle ErrorTimer;
		TIMER_MANAGER.SetTimer(ErrorTimer, GrabFailedCooldownLength, false);
		CharacterWidget->PlayGrabFailAnimation(ErrorTimer);
	}
}



UPrimitiveComponent* UCharacterPhysicsAbilities::DropInteractable(FName& GrabbedBone)
{
	//Drop fail conditions
	if (AbilityState != EPAS_Grabbing)
	{
		if (CharacterWidget)
		{
			FTimerHandle ErrorTimer;
			TIMER_MANAGER.SetTimer(ErrorTimer, DropFailWidgetAnimationLength, false);
			CharacterWidget->PlayDropFailAnimation(ErrorTimer);
		}
		DropFail.PlayAtLocation(this, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation());
		return nullptr;
	}
	if (!HitInteractor)
	{
		LOG_IF(!HitInteractor, "");
		return nullptr;
	}
	if (!HitInteractor->Execute_CanBeDropped(GrabbedComponent->GetOwner(), this))
	{
		return nullptr;
	}

	UPrimitiveComponent* DroppedComponent = DropObject(GrabbedBone);
	//Drop Succeed condition
	if (DroppedComponent)
	{
		HitInteractor->Execute_Drop(DroppedComponent->GetOwner(), this, GrabbedBone);
		AbilityState = EPAS_Ready;
		HitInteractor = nullptr;
		//FUTURE ME: If(HitInteractor->Execute_OverrideDropEffects(HitResult.GetActor(), this, HitResult) //<-- Here if you want to add it
		if (CharacterWidget)
		{
			CharacterWidget->PlayDropSucceedAnimation();
		}
		DropSucceed.PlayAtLocation(this, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation());
	}
	return DroppedComponent;
}


void UCharacterPhysicsAbilities::PokeInteractable(FHitResult HitResult, const FVector& Direction, const float PokePower)
{
	if (AbilityState == EPAS_Throwing)
	{
		return;
	}
	FVector ImpulseLocation = HitResult.ImpactPoint + HitResult.ImpactNormal * DistanceFromImpactPoint;
	UPrimitiveComponent* CompToPoke = HitResult.GetComponent();
	FName GrabbedBone{ NAME_None };
	if (AbilityState == EPAS_Grabbing)
	{
		//Poke Interactable Fail conditions
		if (bPokeOrThrowCooldown)
		{
			PokeFailEffects(HitResult);
			return;
		}
		if (!HitInteractor->Execute_CanBePoked(GrabbedComponent->GetOwner(), this, HitResult, Direction))
		{
			return;
		}
		ImpulseLocation = GrabbedComponent->GetComponentLocation();
		CompToPoke = DropInteractable(GrabbedBone);
	}
	else //AbilityState != EPAS_Grabbing
	{
		//Poke Interactable Fail conditions
		if (bPokeOrThrowCooldown)
		{
			PokeFailEffects(HitResult);
			return;
		}
		if (!HitResult.bBlockingHit)
		{
			//REVIEW: Maybe you want a "NoHit" effect instead of "fail" effects
			SetPokeOrThrowCooldown(true);
			PokeOrThrowCooldownDelegate.BindLambda(
				[this]()
				{
					if (IsValid(this))
					{
						SetPokeOrThrowCooldown(false);
					}
				}
			);
			TIMER_MANAGER.SetTimer(PokeTimer, PokeOrThrowCooldownDelegate, PokeNoHitCooldownLength, false);
			//REVIEW: maybe to many effect played
			if (CharacterWidget)
			{
				CharacterWidget->PlayPokeNoHitAnimation(PokeTimer);
			}
			PokeNoHit.PlayAtLocation(this, GetOwner()->GetActorLocation(), HitResult.TraceEnd);
			return;
		}
		HitInteractor = Cast<IPhysicsAbilitiesInteractable>(CompToPoke->GetOwner());
		if (!HitInteractor)
		{
			SetPokeOrThrowCooldown(true);
			PokeOrThrowCooldownDelegate.BindLambda(
				[this]()
				{
					if (IsValid(this))
					{
						SetPokeOrThrowCooldown(false);
					}
				}
			);
			TIMER_MANAGER.SetTimer(PokeTimer, PokeOrThrowCooldownDelegate, PokeWrongObjectCooldownLength, false);

			if (CharacterWidget)
			{
				CharacterWidget->PlayPokeWrongObjectAnimation(PokeTimer);
			}
			PokeWrongObject.PlayAtLocation(this, GetOwner()->GetActorLocation(), HitResult.ImpactPoint + HitResult.ImpactNormal * DistanceFromImpactPoint);
			return;
		}
		if (!HitInteractor->Execute_CanBePoked(HitResult.GetActor(), this, HitResult, Direction))
		{
			return;
		}
	}

	float PokePowerTemp = PokePower < 0.f ? PokePowerDefault : PokePowerDefault;

	CompToPoke->AddImpulseAtLocation(Direction * PokePowerTemp, ImpulseLocation, GrabbedBone);
	HitInteractor->Execute_Poke(CompToPoke->GetOwner(), this, HitResult);
	HitInteractor = nullptr;
	//FUTURE ME: If(HitInteractor->Execute_OverridePokeEffects(HitResult.GetActor(), this, HitResult) //<-- Here if you want to add it
	SetPokeOrThrowCooldown(true);
	PokeOrThrowCooldownDelegate.BindLambda(
		[this]()
		{
			if (IsValid(this))
			{
				SetPokeOrThrowCooldown(false);
			}
		}
	);
	TIMER_MANAGER.SetTimer(PokeTimer, PokeOrThrowCooldownDelegate, PokeSuccedCooldownLength, false);
	if (CharacterWidget)
	{
		CharacterWidget->PlayPokeSucceedAnimation(PokeTimer);
	}
	PokeSucceed.PlayAtLocation(this, GetOwner()->GetActorLocation(), ImpulseLocation);
}


void UCharacterPhysicsAbilities::PokeFailEffects(const FHitResult& HitResult)
{
	FTimerHandle PokeFailTimer;
	TIMER_MANAGER.SetTimer(PokeFailTimer, PokeFailCooldownLength, false);
	//REVIEW: maybe to many effects played
	if (CharacterWidget)
	{
		CharacterWidget->PlayPokeFailAnimation(PokeFailTimer);
	}
	PokeFail.PlayAtLocation(this, GetOwner()->GetActorLocation(), HitResult.TraceEnd);
}

void UCharacterPhysicsAbilities::SetGrabCooldown(bool Value)
{
	if (bUseSameCooldownGrabAndPoke)
	{
		bPokeOrThrowCooldown = Value;
	}
	bGrabCooldown = Value;
}

void UCharacterPhysicsAbilities::SetPokeOrThrowCooldown(bool Value)
{
	if (bUseSameCooldownGrabAndPoke)
	{
		bGrabCooldown = Value;
	}
	bPokeOrThrowCooldown = Value;
}

UPrimitiveComponent* UCharacterPhysicsAbilities::ThrowInteractable(EInPins ExecPins, const float ThrowPowerBase)
{
	if (ExecPins == EInPins::Pressed)
	{
		if (bPokeOrThrowCooldown)
		{
			ThrowFailEffects();
			return nullptr;
		}
		if (AbilityState != EPAS_Grabbing)
		{
			ThrowFailEffects();
			return nullptr;
		}
		if (!HitInteractor)
		{
			ThrowFailEffects();
			return nullptr;
		}
		if (!HitInteractor->Execute_CanStartThrowing(GrabbedComponent->GetOwner(), this))
		{
			return nullptr;
		}

		AbilityState = EPAS_Throwing;
		HitInteractor->Execute_StartThrowing(GrabbedComponent->GetOwner(), this);
		//FUTURE ME: If(HitInteractor->Execute_OverrideStartThrowingEffects(HitResult.GetActor(), this, HitResult) //<-- Here if you want to add it
		PokeOrThrowCooldownDelegate.BindLambda(
			[this]()
			{
				if (IsValid(this))
				{
					ThrowInteractable(EInPins::Released);
				}
			}
		);
		TIMER_MANAGER.SetTimer(ThrowTimer, PokeOrThrowCooldownDelegate, ThrowMaxChargeTime, false);
		if (CharacterWidget)
		{
			CharacterWidget->PlayThrowStartSucceedAnimation(ThrowTimer);
		}
		if (IsValid(NC_ThrowStartSucceed))
		{
			NC_ThrowStartSucceed->DestroyComponent();
		}
		NC_ThrowStartSucceed = ThrowStartSucced.Niagara.SpawnSystemAtAttached(GrabbedComponent, NAME_None, SphereCollision->GetComponentLocation());
		if (IsValid(SC_ThrowStartSucceed))
		{
			SC_ThrowStartSucceed->DestroyComponent();
		}
		SC_ThrowStartSucceed = ThrowStartSucced.Sound.SpawnSoundAttached(GrabbedComponent, NAME_None, GetOwner()->GetActorLocation());
		ThrowStartSucced.CameraShake.StartCameraShake(this);
		return nullptr;
	}
	else //ExecPins == EInPins::Released
	{
		if (bPokeOrThrowCooldown)
		{
			//I'm really not liking the effects right here
			//ThrowFailEffects();
			return nullptr;
		}
		if (AbilityState != EPAS_Throwing)
		{
			//I'm really not liking the effects right here
			//ThrowFailEffects();
			//FUTURE ME: I think I want to start a throw cooldown only if the throw actually succeeds
			//ThrowFailEffects(ThrowTimer); 
			return nullptr;
		}
		if (!HitInteractor)
		{
			ThrowFailEffects();
			//FUTURE ME: I think I want to start a throw cooldown only if the throw actually succeeds
			//ThrowFailEffects(ThrowTimer); 
			return nullptr;
		}
		if (!HitInteractor->Execute_CanBeThrown(GrabbedComponent->GetOwner(), this))
		{
			return nullptr;
		}
		const float ElapsedTime = TIMER_MANAGER.GetTimerElapsed(ThrowTimer);
		TIMER_MANAGER.ClearTimer(ThrowTimer);

		FVector Direction = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetActorForwardVector();
		FVector ImpulseLocation = GrabbedComponent->GetComponentLocation();
		FName GrabbedBone;
		AbilityState = EPAS_Grabbing;//Needed to actually be able to execute correctly DropInteractable
		//MAYBE: Extrapolate only what you need from DropInteractable
		UPrimitiveComponent* CompToThrow = DropInteractable(GrabbedBone);//Remember Sets AbilityState = EPAS_Ready
		float ThrowPowerBaseTemp = ThrowPowerBase < 0.f ? ThrowPowerBaseDefault : ThrowPowerBaseDefault;

		CompToThrow->AddImpulseAtLocation(Direction * ThrowPowerBaseTemp * (ElapsedTime + 1.f), ImpulseLocation, GrabbedBone);
		HitInteractor->Execute_Throw(CompToThrow->GetOwner(), this);
		//FUTURE ME: If(HitInteractor->Execute_OverrideThrowEffects(HitResult.GetActor(), this, HitResult) //<-- Here if you want to add it
		SetPokeOrThrowCooldown(true);
		PokeOrThrowCooldownDelegate.BindLambda(
			[this]()
			{
				if (IsValid(this))
				{
					SetPokeOrThrowCooldown(false);
				}
			}
		);
		TIMER_MANAGER.SetTimer(ThrowTimer, PokeOrThrowCooldownDelegate, ThrowBaseCooldown + ElapsedTime, false);
		if (CharacterWidget)
		{
			CharacterWidget->PlayThrowSucceedAnimation(ThrowTimer);
		}
		if (IsValid(NC_ThrowStartSucceed))
		{
			NC_ThrowStartSucceed->DestroyComponent();
		}
		if (IsValid(SC_ThrowStartSucceed))
		{
			SC_ThrowStartSucceed->DestroyComponent();
		}
		ThrowSucceed.PlayAtLocation(this, GetOwner()->GetActorLocation(), SphereCollision->GetComponentLocation());
		return CompToThrow;
	}
}

void UCharacterPhysicsAbilities::ThrowFailEffects()
{
	FTimerHandle ThrowFailTimer;
	TIMER_MANAGER.SetTimer(ThrowFailTimer, ThrowBaseCooldown, false);
	if (CharacterWidget)
	{
		CharacterWidget->PlayThrowFailAnimation(ThrowFailTimer);
	}
	ThrowFail.PlayAtLocation(this, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation());
}

void UCharacterPhysicsAbilities::ThrowFailEffects(FTimerHandle& Timer)
{
	SetPokeOrThrowCooldown(true);
	PokeOrThrowCooldownDelegate.BindLambda(
		[this]()
		{
			if (IsValid(this))
			{
				SetPokeOrThrowCooldown(false);
			}
		});
	TIMER_MANAGER.SetTimer(Timer, PokeOrThrowCooldownDelegate, ThrowBaseCooldown, false);
	if (CharacterWidget)
	{
		CharacterWidget->PlayThrowFailAnimation(Timer);
	}
	ThrowFail.PlayAtLocation(this, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation());
}


void UCharacterPhysicsAbilities::BeginPlay()
{
	Super::BeginPlay();
	AbilityState = EPAS_Ready;
	if (CharacterWidgetClass)
	{
		CharacterWidget = CreateWidget<UCharacterPhysicsAbilitiesWidget>(GetWorld(), CharacterWidgetClass);
		CharacterWidget->AddToViewport();
	}
}

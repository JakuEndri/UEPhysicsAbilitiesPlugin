// Copyright Endri Jaku. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsAbilitiesBase.h"

#include "PhysicsAbilitiesInteractable.h"
#include "CommonHelpers.h"

#include "CharacterPhysicsAbilities.generated.h"


UENUM(BlueprintType)
enum EPhysAbilityState : uint8
{
	EPAS_None UMETA(DisplayName = "None"),
	EPAS_Ready UMETA(DisplayName = "Ready"),
	EPAS_Grabbing UMETA(DisplayName = "Grabbing"),
	EPAS_Throwing UMETA(DisplayName = "Throwing")
};
UENUM(BlueprintType)
enum class EInPins : uint8
{
	Pressed UMETA(DisplayName = "Pressed"),
	Released UMETA(DisplayName = "Released")
};

USTRUCT(BlueprintType)
struct FCharacterPhysicsAbilitiesEffects
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "PhysicsAbilitiesEffects")
	FSoundHelper Sound;
	UPROPERTY(EditAnywhere, Category = "PhysicsAbilitiesEffects")
	FNiagaraHelper Niagara;
	UPROPERTY(EditAnywhere, Category = "PhysicsAbilitiesEffects")
	FCameraShakeHelper CameraShake;

	void PlayAtLocation(UObject* WorldContextObject, const FVector& SoundLocation, const FVector NiagaraLocation)
	{
		Sound.PlaySoundAtLocation(WorldContextObject, SoundLocation);
		Niagara.SpawnSystemAtLocation(WorldContextObject, NiagaraLocation);
		CameraShake.StartCameraShake(WorldContextObject);
	}

	void SpawnAttached(USceneComponent* AttachToComponent, const FVector& SoundLocation, const FVector& NiagaraLocation, FName AttachPointName = NAME_None, bool bAutoDestroy = true)
	{
		Sound.SpawnSoundAttached(AttachToComponent, AttachPointName, SoundLocation, bAutoDestroy);
		Niagara.SpawnSystemAtAttached(AttachToComponent, AttachPointName, NiagaraLocation, bAutoDestroy);
		CameraShake.StartCameraShake(AttachToComponent);
	}

};

/**
 *
 */
UCLASS()
class PHYSICSABILITIES_API UCharacterPhysicsAbilities : public UPhysicsAbilitiesBase
{
	GENERATED_BODY()

#define TIMER_MANAGER GetWorld()->GetTimerManager()

public:


	/**
	* fully works only with actors that implement IPhysicsAbilitiesInteractable
	*/
	UFUNCTION(BlueprintCallable, Category = "CharacterPhysicsAbilities|IPhysicsAbilitiesInteractable")
	virtual class USphereComponent* GrabInteractable(const FHitResult& HitResult, USceneComponent* NewParentComponent, bool bGrabAtHitLocation = true);
	UFUNCTION(BlueprintCallable, Category = "CharacterPhysicsAbilities|IPhysicsAbilitiesInteractable")
	virtual UPrimitiveComponent* DropInteractable(FName& GrabbedBone);
	/**
	* @PARAM PokePower if negative PokePowerDefault is used instead
	*/
	UFUNCTION(BlueprintCallable, Category = "CharacterPhysicsAbilities")
	void PokeInteractable(FHitResult HitResult, const FVector& Direction, const float PokePower = -1.f);
	/**
	* @PARAM PokePower if negative PokePowerDefault is used instead
	*/
	UFUNCTION(BlueprintCallable, Category = "CharacterPhysicsAbilities", meta = (ExpandEnumAsExecs = "ExecPins"))
	UPrimitiveComponent* ThrowInteractable(EInPins ExecPins, const float ThrowPowerBase = -1.f);

	UPROPERTY(EditAnywhere, Category = "NiagaraEffects")
	float DistanceFromImpactPoint{ 0.f };

	UPROPERTY(EditAnywhere, Category = "GrabInteractable")
	bool bUseSameCooldownGrabAndPoke{ false };
	UPROPERTY(EditAnywhere, Category = "GrabInteractable")
	float GrabSucceededCooldownLength{ 1.2f };
	UPROPERTY(EditAnywhere, Category = "GrabInteractable")
	float GrabFailedCooldownLength{ 1.2f };
	UPROPERTY(EditAnywhere, Category = "GrabInteractable")
	float GrabNoHitCooldownLength{ 1.2f };
	UPROPERTY(EditAnywhere, Category = "GrabInteractable")
	float GrabHitWrongObjectCooldownLength{ 1.2f };
	UPROPERTY(EditAnywhere, Category = "GrabInteractable")
	FCharacterPhysicsAbilitiesEffects GrabFail;
	UPROPERTY(EditAnywhere, Category = "GrabInteractable")
	FCharacterPhysicsAbilitiesEffects GrabNoHit;
	UPROPERTY(EditAnywhere, Category = "GrabInteractable")
	FCharacterPhysicsAbilitiesEffects GrabWrongObject;
	UPROPERTY(EditAnywhere, Category = "GrabInteractable")
	FCharacterPhysicsAbilitiesEffects GrabSuccess;

	UPROPERTY(EditAnywhere, Category = "DropInteractable")
	float DropFailWidgetAnimationLength{ 1.2f };
	UPROPERTY(EditAnywhere, Category = "DropInteractable")
	FCharacterPhysicsAbilitiesEffects DropFail;
	UPROPERTY(EditAnywhere, Category = "DropInteractable")
	FCharacterPhysicsAbilitiesEffects DropSucceed;

	UPROPERTY(EditAnywhere, Category = "PokeInteractable")
	float PokeFailCooldownLength{ 1.2f };
	UPROPERTY(EditAnywhere, Category = "PokeInteractable")
	float PokeNoHitCooldownLength{ 1.2f };
	UPROPERTY(EditAnywhere, Category = "PokeInteractable")
	float PokeWrongObjectCooldownLength{ 1.2f };

	UPROPERTY(EditAnywhere, Category = "PokeInteractable")
	float PokeSuccedCooldownLength{ 1.2f };
	UPROPERTY(EditAnywhere, Category = "PokeInteractable")
	float PokePowerDefault{ 10000.f };
	UPROPERTY(EditAnywhere, Category = "PokeInteractable")
	FCharacterPhysicsAbilitiesEffects PokeFail;
	UPROPERTY(EditAnywhere, Category = "PokeInteractable")
	FCharacterPhysicsAbilitiesEffects PokeNoHit;
	UPROPERTY(EditAnywhere, Category = "PokeInteractable")
	FCharacterPhysicsAbilitiesEffects PokeWrongObject;
	UPROPERTY(EditAnywhere, Category = "PokeInteractable")
	FCharacterPhysicsAbilitiesEffects PokeSucceed;

	UPROPERTY(EditAnywhere, Category = "ThrowInteractable")
	float ThrowPowerBaseDefault{ 10000.f };
	UPROPERTY(EditAnywhere, Category = "ThrowInteractable")
	float ThrowMaxChargeTime{ 2.f };
	UPROPERTY(EditAnywhere, Category = "ThrowInteractable")
	float ThrowFailCooldown{ 1.2f };
	UPROPERTY(EditAnywhere, Category = "ThrowInteractable")
	float ThrowBaseCooldown{ 1.2f };
	UPROPERTY(EditAnywhere, Category = "ThrowInteractable")
	FCharacterPhysicsAbilitiesEffects ThrowFail;
	UPROPERTY(EditAnywhere, Category = "ThrowInteractable")
	FCharacterPhysicsAbilitiesEffects ThrowStartSucced;
	UPROPERTY(EditAnywhere, Category = "ThrowInteractable")
	FCharacterPhysicsAbilitiesEffects ThrowSucceed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<class UCharacterPhysicsAbilitiesWidget> CharacterWidgetClass;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void PlayGrabFailEffects();
	void ThrowFailEffects();
	void ThrowFailEffects(FTimerHandle& Timer);
	void PokeFailEffects(const FHitResult& HitResult);
	void SetGrabCooldown(bool Value);
	void SetPokeOrThrowCooldown(bool Value);


	EPhysAbilityState AbilityState{ EPhysAbilityState::EPAS_None };
	IPhysicsAbilitiesInteractable* HitInteractor;
	UPROPERTY()
	UNiagaraComponent* NC_ThrowStartSucceed;//Used because IsValid works properly only with variables with the macro UPROPERTY()
	UPROPERTY()
	UAudioComponent* SC_ThrowStartSucceed;
	FTimerHandle GrabTimer;
	FTimerHandle PokeTimer;
	FTimerHandle ThrowTimer;
	FTimerDelegate GrabCooldownDelegate;
	FTimerDelegate PokeOrThrowCooldownDelegate;

	bool bGrabCooldown{ false };
	bool bPokeOrThrowCooldown{ false };
	UPROPERTY()
	UCharacterPhysicsAbilitiesWidget* CharacterWidget;



};

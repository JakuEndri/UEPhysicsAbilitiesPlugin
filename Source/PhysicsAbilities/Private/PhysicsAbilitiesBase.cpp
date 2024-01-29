// Copyright Endri Jaku. All Rights Reserved.


#include "PhysicsAbilitiesBase.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UPhysicsAbilitiesBase::UPhysicsAbilitiesBase()
{

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



// Called when the game starts
void UPhysicsAbilitiesBase::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	SphereCollision = Cast<USphereComponent>(
		Owner->AddComponentByClass(USphereComponent::StaticClass(), false, FTransform::Identity, false)
	);
	SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PhysicsConstraint = Cast<UPhysicsConstraintComponent>(
		Owner->AddComponentByClass(UPhysicsConstraintComponent::StaticClass(), false, FTransform::Identity, false)
	);
	UpdatePhysicsConstraint();
}


// Called every frame
void UPhysicsAbilitiesBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UPhysicsAbilitiesBase::UpdatePhysicsConstraint()
{
	if (!IsValid(PhysicsConstraint))
	{
		return false;
	}

	//Linear Limits 
	PhysicsConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Free, 0.f);
	PhysicsConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Free, 0.f);
	PhysicsConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Free, 0.f);
	//Linear motor
	PhysicsConstraint->SetLinearPositionDrive(true, true, true);
	PhysicsConstraint->SetLinearVelocityDrive(true, true, true);
	PhysicsConstraint->SetLinearDriveParams(LinearPositionStrength, LinearVelocityStrength, LinearInForceLimit);
	PhysicsConstraint->SetLinearBreakable(bLinearBreakable, LinearBreakThreshold);

	//AngularMotor
	PhysicsConstraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	PhysicsConstraint->SetAngularVelocityDrive(true, true);
	PhysicsConstraint->SetOrientationDriveTwistAndSwing(true, true);
	PhysicsConstraint->SetAngularDriveParams(AngularPositionStrength, AngularVelocityStrength, AngularInForceLimit);
	PhysicsConstraint->SetAngularBreakable(bAngularBreakable, AngularBreakThreshold);

	return true;
}

USphereComponent* UPhysicsAbilitiesBase::GrabBasic(UPrimitiveComponent* ComponentToGrab, USceneComponent* NewParent, bool bGrabAtLocation, const FVector& LocationToGrab, const FName& BoneToGrab)
{
	if (IsValid(GrabbedComponent) || !IsValid(ComponentToGrab) || !ComponentToGrab->IsSimulatingPhysics() || !IsValid(NewParent))
	{
		return nullptr;
	}

	SphereCollision->AttachToComponent(NewParent, FAttachmentTransformRules::KeepRelativeTransform);
	PhysicsConstraint->AttachToComponent(SphereCollision, FAttachmentTransformRules::KeepRelativeTransform);

	if (bGrabAtLocation)
	{
		SphereCollision->SetWorldLocation(LocationToGrab);
	}
	else
	{
		SphereCollision->SetWorldLocation(ComponentToGrab->GetComponentLocation());
	}

	//The actual start of the physics constraint
	PhysicsConstraint->SetConstrainedComponents(SphereCollision, FName("None"), ComponentToGrab, BoneToGrab);
	GrabbedComponent = ComponentToGrab;

	PreviousCollisionResponse = GrabbedComponent->GetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn);
	GrabbedComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	return SphereCollision;
}

UPrimitiveComponent* UPhysicsAbilitiesBase::DropObject(FName& GrabbedBone)
{
	//We use this function only if we are grabbin something
	if (!IsValid(GrabbedComponent))
	{
		return nullptr;
	}

	GrabbedBone = PhysicsConstraint->ConstraintInstance.ConstraintBone2;
	PhysicsConstraint->BreakConstraint();
	GrabbedComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, PreviousCollisionResponse);

	if (bZeroLinearAngularVelocity)
	{
		GrabbedComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
		GrabbedComponent->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	}
	else // bZeroLinearAngularVelocity = false
	{
		GrabbedComponent->SetPhysicsLinearVelocity(
			GrabbedComponent->GetPhysicsLinearVelocity() * OutLinearVelocityFactor
		);
		GrabbedComponent->SetPhysicsAngularVelocityInDegrees(
			GrabbedComponent->GetPhysicsAngularVelocityInDegrees() * OutAngularVelocityFactor
		);
	}
	UPrimitiveComponent* ReturnTemp = GrabbedComponent;
	GrabbedComponent = nullptr;
	return ReturnTemp;
}


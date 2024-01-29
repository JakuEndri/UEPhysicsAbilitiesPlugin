// Copyright Endri Jaku. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsAbilitiesBase.generated.h"





UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PHYSICSABILITIES_API UPhysicsAbilitiesBase : public UActorComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UPhysicsAbilitiesBase();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	* If bGrabAtLocation is a true LocationToGrab is used.
	* @return If grabbed something should return the "Base" of the PhysicsConstraint which allows to control the movement in a static way.
	* Else returns nullptr
	*/
	UFUNCTION(BlueprintCallable, Category = "PhysicsAbility")
	virtual class USphereComponent* GrabBasic(UPrimitiveComponent* ComponentToGrab, USceneComponent* NewParent, bool bGrabAtLocation = true, const FVector& LocationToGrab = FVector(0.f), const FName& BoneToGrab = FName("None"));
	/**
	* Drops the GrabbedComponent
	* @return Returns the Dropped object if successful, nullptr otherwise
	*/
	UFUNCTION(BlueprintCallable, Category = "PhysicsAbility")
	virtual UPrimitiveComponent* DropObject(FName& GrabbedBone);

	/** The Static Component of the physics constraint. Used to have static control of the physical component it is attached to through the Physics constraint */
	UPROPERTY(BlueprintReadOnly, Category = Constraint)
	USphereComponent* SphereCollision;

protected:
	AActor* Owner{};
	/*Updates the physics Constraint with the variables in Linear motor and angular motor.
	Make sure to call this function after you modify one of those variables*/
	bool UpdatePhysicsConstraint();

	UPROPERTY(BlueprintReadOnly, Category = Constraint)
	class UPhysicsConstraintComponent* PhysicsConstraint{};
	UPROPERTY(BlueprintReadOnly, Category = Constraint)
	UPrimitiveComponent* GrabbedComponent;

	//Linear Motor variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Linear Motor")
	float LinearPositionStrength{ 300.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Linear Motor")
	float LinearVelocityStrength{ 50.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Linear Motor")
	float LinearInForceLimit{ 0.f };
	/**I do not suggest to use it because very hard to test*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Linear Motor")
	bool bLinearBreakable{ false };
	/**I do not suggest to use it because very hard to test*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Linear Motor")
	float LinearBreakThreshold{ 3000.f };
	//If 0 or negative it will not break the constraint based on distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Linear Motor")
	int32 MaxDistanceFromPlayer{ 0 };

	//Angular Motor variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angular Motor")
	float AngularPositionStrength{ 300.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angular Motor")
	float AngularVelocityStrength{ 50.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angular Motor")
	float AngularInForceLimit{ 0.f };
	/**I do not suggest to use it because very hard to test*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angular Motor")
	bool bAngularBreakable{ false };
	/**I do not suggest to use it because very hard to test*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angular Motor")
	float AngularBreakThreshold{ 3000.f };

	//Dropping Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop Object State")
	bool bZeroLinearAngularVelocity{ true };
	/** Used only if bZeroLinearAngularVelocity  is false*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop Object State")
	float OutLinearVelocityFactor{ .1f };
	/** Used only if bZeroLinearAngularVelocity  is false*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop Object State")
	float OutAngularVelocityFactor{ .1f };



	// Called when the game starts
	virtual void BeginPlay() override;

private:
	ECollisionResponse PreviousCollisionResponse;

};

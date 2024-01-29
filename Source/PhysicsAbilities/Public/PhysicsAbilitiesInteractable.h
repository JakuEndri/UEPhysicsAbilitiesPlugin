// Copyright Endri Jaku. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PhysicsAbilitiesInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, BlueprintType)
class UPhysicsAbilitiesInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class PHYSICSABILITIES_API IPhysicsAbilitiesInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PhysicsAbilitiesInterface")
	bool CanBeGrabbed(UObject* Interactor, const FHitResult& Hit);
	virtual bool CanBeGrabbed_Implementation(UObject* Interactor, const FHitResult& Hit);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PhysicsAbilitiesInterface")
	void Grabb(UObject* Interactor, const FHitResult& Hit);
	virtual void Grabb_Implementation(UObject* Interactor, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PhysicsAbilitiesInterface")
	bool CanBeDropped(UObject* Interactor);
	virtual bool CanBeDropped_Implementation(UObject* Interactor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PhysicsAbilitiesInterface")
	void Drop(UObject* Interactor, FName GrabbedBone = NAME_None);
	virtual void Drop_Implementation(UObject* Interactor, FName GrabbedBone = FName());

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PhysicsAbilitiesInterface")
	bool CanBePoked(UObject* Interactor, const FHitResult& Hit, const FVector& Direction);
	virtual bool CanBePoked_Implementation(UObject* Interactor, const FHitResult& Hit, const FVector& Direction);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PhysicsAbilitiesInterface")
	void Poke(UObject* Interactor, const FHitResult& Hit);
	virtual void Poke_Implementation(UObject* Interactor, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PhysicsAbilitiesInterface")
	bool CanStartThrowing(UObject* Interactor);
	virtual bool CanStartThrowing_Implementation(UObject* Interactor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PhysicsAbilitiesInterface")
	void StartThrowing(UObject* Interactor);
	virtual void StartThrowing_Implementation(UObject* Interactor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PhysicsAbilitiesInterface")
	bool CanBeThrown(UObject* Interactor);
	virtual bool CanBeThrown_Implementation(UObject* Interactor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PhysicsAbilitiesInterface")
	void Throw(UObject* Interactor);
	virtual void Throw_Implementation(UObject* Interactor);
};

// Copyright Endri Jaku. All Rights Reserved.


#include "PhysicsAbilitiesInteractable.h"
#include "CharacterPhysicsAbilities.h"
#include "CommonHelpers.h"
// Add default functionality here for any IPhysicsAbilitiesInteractable functions that are not pure virtual.


bool IPhysicsAbilitiesInteractable::CanBePoked_Implementation(UObject* Interactor, const FHitResult& Hit, const FVector& Direction)
{
	return true;
}

void IPhysicsAbilitiesInteractable::Poke_Implementation(UObject* Interactor, const FHitResult& HitResult)
{
}

bool IPhysicsAbilitiesInteractable::CanStartThrowing_Implementation(UObject* Interactor)
{
	return true;
}

void IPhysicsAbilitiesInteractable::StartThrowing_Implementation(UObject* Interactor)
{
}

bool IPhysicsAbilitiesInteractable::CanBeThrown_Implementation(UObject* Interactor)
{
	return true;
}

void IPhysicsAbilitiesInteractable::Throw_Implementation(UObject* Interactor)
{
}

bool IPhysicsAbilitiesInteractable::CanBeGrabbed_Implementation(UObject* Interactor, const FHitResult& Hit)
{
	return true;
}

void IPhysicsAbilitiesInteractable::Grabb_Implementation(UObject* Interactor, const FHitResult& Hit)
{
}

bool IPhysicsAbilitiesInteractable::CanBeDropped_Implementation(UObject* Interactor)
{
	return true;
}

void IPhysicsAbilitiesInteractable::Drop_Implementation(UObject* Interactor, FName GrabbedBone)
{
}

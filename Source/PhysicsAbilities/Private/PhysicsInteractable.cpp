// Copyright Endri Jaku. All Rights Reserved.


#include "PhysicsInteractable.h"
#include "CharacterPhysicsAbilities.h"

// Sets default values
APhysicsInteractable::APhysicsInteractable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APhysicsInteractable::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APhysicsInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

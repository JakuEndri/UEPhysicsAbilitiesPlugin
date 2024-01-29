// Copyright Endri Jaku. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsInteractable.generated.h"
//
//using namespace UP;//UPROPERTY()
//using namespace UM;//meta
//using namespace UC;//UCLASS()
//using namespace UI;//UINTERFACE()
//using namespace US;//USTRUCT()
//using namespace UF;//UFUNCTION()
//


UCLASS(Blueprintable, BlueprintType)
class PHYSICSABILITIES_API APhysicsInteractable : public AActor, public IPhysicsAbilitiesInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APhysicsInteractable();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

};

// Copyright Endri Jaku. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonFunctions.generated.h"

/**
 *
 */
UCLASS()
class PHYSICSABILITIES_API UCommonFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	/*UFUNCTION(BlueprintPure, Meta = (DefaultToSelf = "ActorToGetInformations"))
		static int32 Straight_GetHealth(AActor* ActorToGetInformations);*/


	UFUNCTION(BlueprintCallable, meta = (category = "Custom", bIgnoreSelf = "true", AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "LineTraceSingleByReference", AdvancedDisplay = "TraceColor,TraceHitColor,DebugThickness", Keywords = "custom trace"))
	static bool LineTraceSingleByReference(
		FHitResult& OutHit,
		const USceneComponent* ComponentReference,
		const TArray<AActor*>& ActorsToIgnore,
		const float TraceLength = 10000.f,
		const bool bIgnoreSelf = true,
		const ECollisionChannel CollisionChannel = ECollisionChannel::ECC_Visibility,
		const bool bTraceComplex = false,
		const float DrawTime = 3.f,
		const float DebugThickness = 0.f,
		const FColor TraceColor = FColor::Red,
		const FColor TraceHitColor = FColor::Green
	);
	UFUNCTION(BlueprintCallable, meta = (category = "Custom", bIgnoreSelf = "true", AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "SphereTraceSingleByReference", AdvancedDisplay = "TraceColor,TraceHitColor,DebugThickness", Keywords = "custom trace"))
	static bool SphereTraceSingleByReference(
		FHitResult& OutHit,
		const USceneComponent* ComponentReference,
		const TArray<AActor*>& ActorsToIgnore,
		const float TraceLength = 10000.f,
		const float Radius = 10.f,
		const bool bIgnoreSelf = true,
		const ECollisionChannel CollisionChannel = ECollisionChannel::ECC_Visibility,
		const bool bTraceComplex = false,
		const float DrawTime = 3.f,
		const float DebugThickness = 0.f,
		const FColor TraceColor = FColor::Red,
		const FColor TraceHitColor = FColor::Green
	);
	UFUNCTION(BlueprintCallable, meta = (category = "Custom", bIgnoreSelf = "true", AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "LineTraceSingleWithToleranceByReference", AdvancedDisplay = "TraceColor,TraceHitColor,DebugThickness", Keywords = "custom trace"))
	static bool LineTraceSingleWithToleranceByReference(
		FHitResult& OutHit,
		const USceneComponent* ComponentReference,
		const TArray<AActor*>& ActorsToIgnore,
		const float TraceLength = 10000.f,
		const float Tolerance = 10.f/*basically is the radius of the SphereTrace*/,
		const bool bIgnoreSelf = true,
		const ECollisionChannel CollisionChannel = ECollisionChannel::ECC_Visibility,
		const bool bTraceComplex = false,
		const float DrawTime = 3.f,
		const float DebugThickness = 0.f,
		const FColor TraceColor = FColor::Red,
		const FColor TraceHitColor = FColor::Green
	);
	UFUNCTION(BlueprintPure, Category = "PhysicGrabber")
	static void GetForwardLocation(FVector& StartLocation, FVector& ForwardLocation, const USceneComponent* Comp, const float ForwardDistance = 1000);

private:
	static FCollisionQueryParams InitializeCollisionQueryParams(const AActor* Owner, const bool bIgnoreSelf, const TArray<AActor*>& ActorsToIgnore, const bool bTraceComplex);
};

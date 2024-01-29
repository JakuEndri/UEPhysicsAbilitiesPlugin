// Copyright Endri Jaku. All Rights Reserved.


#include "CommonFunctions.h"

#include "DrawDebugHelpers.h"
#include "DrawDebugHelpers.h"
#include "CollisionShape.h"

constexpr float LINE_TRACE_DEBUG_POINT_SIZE_OFFSET = 8.f;
constexpr float SPHERE_TRACE_DEBUG_POINT_SIZE_OFFSET = 8.f;
constexpr int32 SPHERE_TRACE_DEBUG_DRAW_NUM_SEGMENTS = 4;


bool UCommonFunctions::LineTraceSingleByReference(
	FHitResult& OutHit,
	const USceneComponent* ComponentReference,
	const TArray<AActor*>& ActorsToIgnore,
	const float TraceLength,
	const bool bIgnoreSelf,
	const ECollisionChannel CollisionChannel,
	const bool bTraceComplex,
	const float DrawTime,
	const float DebugThickness,
	const FColor TraceColor,
	const FColor TraceHitColor
)
{
	if (!ComponentReference) return false;
	AActor* ComponentOwner = ComponentReference->GetOwner();
	if (!ComponentOwner) return false;
	UWorld* World = ComponentOwner->GetWorld();
	if (!World) return false;
	FVector StartLocation = ComponentReference->GetComponentLocation();
	FVector EndLocation = ComponentReference->GetComponentLocation() + ComponentReference->GetForwardVector() * TraceLength;
	FCollisionQueryParams CollisionQueryParams = InitializeCollisionQueryParams(ComponentOwner, bIgnoreSelf, ActorsToIgnore, bTraceComplex);

	World->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, CollisionChannel, CollisionQueryParams);
	if (DrawTime > 0)
	{
		DrawDebugLine(World, StartLocation, EndLocation, TraceColor, false, DrawTime, 0U, DebugThickness);
		if (OutHit.bBlockingHit)
		{
			DrawDebugPoint(World, OutHit.ImpactPoint, DebugThickness + LINE_TRACE_DEBUG_POINT_SIZE_OFFSET, TraceHitColor, false, DrawTime);
			UE_LOG(LogTemp, Log, TEXT("Hit Component %s   "), *OutHit.Component.Get()->GetName());
		}
	}
	return OutHit.bBlockingHit;
}

bool UCommonFunctions::SphereTraceSingleByReference(
	FHitResult& OutHit,
	const USceneComponent* ComponentReference,
	const TArray<AActor*>& ActorsToIgnore,
	const float TraceLength,
	const float Radius,
	const bool bIgnoreSelf,
	const ECollisionChannel CollisionChannel,
	const bool bTraceComplex,
	const float DrawTime,
	const float DebugThickness,
	const FColor TraceColor,
	const FColor TraceHitColor
)
{
	if (!ComponentReference) return false;
	AActor* ComponentOwner = ComponentReference->GetOwner();
	if (!ComponentOwner) return false;
	UWorld* World = ComponentOwner->GetWorld();
	if (!World) return false;

	FVector StartLocation = ComponentReference->GetComponentLocation();
	FVector EndLocation = ComponentReference->GetComponentLocation() + ComponentReference->GetForwardVector() * TraceLength;
	FQuat Rotation = ComponentReference->GetComponentRotation().Quaternion();
	FCollisionQueryParams CollisionQueryParams = InitializeCollisionQueryParams(ComponentOwner, bIgnoreSelf, ActorsToIgnore, bTraceComplex);

	World->SweepSingleByChannel(OutHit, StartLocation, EndLocation, Rotation, CollisionChannel, FCollisionShape::MakeSphere(Radius), CollisionQueryParams);
	if (DrawTime > 0)
	{
		DrawDebugCylinder(World, StartLocation, EndLocation, Radius, SPHERE_TRACE_DEBUG_DRAW_NUM_SEGMENTS, TraceColor, false, DrawTime, 0U, DebugThickness);
		if (OutHit.bBlockingHit)
		{
			DrawDebugPoint(World, OutHit.ImpactPoint, Radius + SPHERE_TRACE_DEBUG_POINT_SIZE_OFFSET, TraceHitColor, false, DrawTime);
			UE_LOG(LogTemp, Log, TEXT("Hit Component %s   "), *OutHit.Component.Get()->GetName());
		}
	}
	return OutHit.bBlockingHit;
}

bool UCommonFunctions::LineTraceSingleWithToleranceByReference(
	FHitResult& OutHit,
	const USceneComponent* ComponentReference,
	const TArray<AActor*>& ActorsToIgnore,
	const float TraceLength,
	const float Tolerance,
	const bool bIgnoreSelf,
	const ECollisionChannel CollisionChannel,
	const bool bTraceComplex,
	const float DrawTime,
	const float DebugThickness,
	const FColor TraceColor,
	const FColor TraceHitColor)
{
	if (
		LineTraceSingleByReference(
			OutHit, ComponentReference, ActorsToIgnore, TraceLength,
			bIgnoreSelf, CollisionChannel, bTraceComplex, DrawTime,
			DebugThickness, TraceColor, TraceHitColor)
		)
	{
		return true;
	}
	if (
		SphereTraceSingleByReference(OutHit, ComponentReference, ActorsToIgnore, TraceLength,
			Tolerance, bIgnoreSelf, CollisionChannel, bTraceComplex, DrawTime,
			DebugThickness, TraceColor, TraceHitColor)
		)
	{
		return true;
	}
	return false;
}



void UCommonFunctions::GetForwardLocation(FVector& StartLocation, FVector& ForwardLocation, const USceneComponent* Comp, const float ForwardDistance)
{
	if (!Comp) return;
	StartLocation = Comp->GetComponentLocation();
	ForwardLocation = StartLocation + Comp->GetForwardVector() * ForwardDistance;
}

FCollisionQueryParams UCommonFunctions::InitializeCollisionQueryParams(const AActor* Owner, const bool bIgnoreSelf, const TArray<AActor*>& ActorsToIgnore, const bool bTraceComplex)
{
	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
	if (bIgnoreSelf)
	{
		CollisionQueryParams.AddIgnoredActor(Owner);
	}
	if (ActorsToIgnore.Num() > 0)
	{
		CollisionQueryParams.AddIgnoredActors(ActorsToIgnore);
	}
	CollisionQueryParams.bTraceComplex = bTraceComplex;

	return CollisionQueryParams;
}
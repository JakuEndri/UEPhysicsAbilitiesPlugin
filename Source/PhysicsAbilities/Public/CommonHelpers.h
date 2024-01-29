// Copyright Endri Jaku. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
//For FNiagaraStruct
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
//For FSoundStruct
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

#include "CommonHelpers.generated.h"


#if WITH_EDITOR
#define LOG_IF(Condition, Comment)\
{\
if(Condition)\
{\
FString FileName(__FILE__);\
int32 index{};\
FileName = FileName.FindLastChar('\\', index) ? FileName.RightChop(index + 1) : FileName;\
UE_LOG(LogTemp, Warning, TEXT("FUNCTION: %s::%s \t LINE: %d \t CAUSE: %s \t COMMENT: %s"), *FileName, *FString(__func__), __LINE__, *FString(#Condition), *FString(Comment));\
}\
}
#define SCREEN_DEBUG_IF(Condition, Comment, FColor)\
{\
if (Condition)\
{\
FString FileName(__FILE__);\
int32 index{};\
FileName = FileName.FindLastChar('\\', index) ? FileName.RightChop(index + 1) : FileName;\
UE_LOG(LogTemp, Warning, TEXT("FUNCTION: %s::%s \t LINE: %d \t CAUSE: %s \t COMMENT: %s"), *FileName, *FString(__func__), __LINE__, *FString(#Condition), *FString(Comment));\
if (GEngine)\
{\
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor, FString::Printf(TEXT("FUNCTION: %s::%s \t LINE: %d \t CAUSE: %s \t COMMENT: %s"), *FileName, *FString(__func__), __LINE__, *FString(#Condition), *FString(Comment)));\
}\
}\
}
#else
#define LOG_IF(Condition, Comment)
#define SCREEN_DEBUG_IF(Condition, Comment, FColor)
#endif // WITH_EDITOR

#if WITH_EDITOR
#define SPEED_TEST_SIMPLE(Code, Comment, FColor)\
double StartTime = FPlatformTime::Seconds();\
Code;\
double EndTime = FPlatformTime::Seconds();\
double ElapsedTime = EndTime - StartTime;\
UE_LOG(LogTemp, Warning, TEXT("%s %lf"), *FString(Comment), ElapsedTime);\
if(GEngine)\
{\
GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor, FString::Printf(TEXT("%s %lf"), *FString(Comment), ElapsedTime));\
}
#else
#define SPEED_TEST_SIMPLE(Code, Comment, FColor) Code;
#endif //WITH_EDITOR


USTRUCT(BlueprintType)
struct FNiagaraHelper
{
	GENERATED_BODY()

	// The Niagara System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraSystem* NiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	FVector Scale{ 1.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	TMap<FName, float> FloatParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	TMap<FName, FLinearColor> LinearColorParameters;


	void SetSystemParameters(UNiagaraComponent* NiagaraCompnent) const
	{
		for (auto FloatParameter : FloatParameters)
		{
			NiagaraCompnent->SetVariableFloat(FloatParameter.Key, FloatParameter.Value);
		}
		for (auto LinearColorParameter : LinearColorParameters)
		{
			NiagaraCompnent->SetVariableLinearColor(LinearColorParameter.Key, LinearColorParameter.Value);
		}
	}

	// Function to play the Niagara System at Location
	UNiagaraComponent* SpawnSystemAtLocation(const UObject* WorldContextObject, const FVector& SpawnLocation, const bool bAutoDestroy = true, const FRotator SpawnRotation = FRotator::ZeroRotator, const bool bAutoActivate = true) const
	{
		if (!NiagaraSystem)
		{
			return nullptr;
		}
		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(WorldContextObject, NiagaraSystem, SpawnLocation, SpawnRotation, Scale, bAutoDestroy, bAutoActivate);
		SetSystemParameters(NiagaraComponent);

		return NiagaraComponent;
	}

	// Function to play the Niagara System at Location
	UNiagaraComponent* SpawnSystemAtAttached(USceneComponent* AttachToComponent, const FName& AttachPointName, const FVector Location, const bool bAutoDestroy = false, const FRotator Rotation = FRotator::ZeroRotator, const EAttachLocation::Type LocationType = EAttachLocation::KeepWorldPosition, const bool bAutoActivate = true) const
	{
		if (!NiagaraSystem)
		{
			return nullptr;
		}

		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, AttachToComponent, AttachPointName, Location, Rotation, LocationType, bAutoDestroy, bAutoActivate);
		NiagaraComponent->SetRelativeScale3D(Scale);

		SetSystemParameters(NiagaraComponent);

		return NiagaraComponent;

	}

};

USTRUCT(BlueprintType)
struct FSoundHelper
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	float VolumeMultiplier{ 1.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	float PitchMultiplier{ 1.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	float StartTime{ 0.f };

	void PlaySoundAtLocation(const UObject* WorldContextObject, FVector Location)
	{
		if (!Sound)
		{
			return;
		}

		UGameplayStatics::PlaySoundAtLocation(WorldContextObject, Sound, Location, VolumeMultiplier, PitchMultiplier, StartTime);

	}

	void PlaySound2D(const UObject* WorldContextObject, FVector Location)
	{
		if (!Sound)
		{
			return;
		}

		UGameplayStatics::PlaySound2D(WorldContextObject, Sound, VolumeMultiplier, PitchMultiplier, StartTime);
	}

	UAudioComponent* SpawnSoundAttached(USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, bool bAutoDestroy = false, EAttachLocation::Type LocationType = EAttachLocation::KeepWorldPosition, bool bStopWhenAttachedToDestroyed = true)
	{
		if (!Sound)
		{
			return nullptr;
		}

		return UGameplayStatics::SpawnSoundAttached(Sound, AttachToComponent, AttachPointName, Location, LocationType, bStopWhenAttachedToDestroyed, VolumeMultiplier, PitchMultiplier, StartTime, nullptr, nullptr, bAutoDestroy);
	}
};


USTRUCT(BlueprintType)
struct FCameraShakeHelper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake")
	float CameraShakeScale{ 1.f };

	UCameraShakeBase* StartCameraShake(const UObject* WorldContextObject) const
	{
		if (!CameraShake)
		{
			return nullptr;
		}
		return UGameplayStatics::GetPlayerCameraManager(WorldContextObject, 0)->StartCameraShake(CameraShake, CameraShakeScale);

	}

};

/**
 *
 */
UCLASS()
class PHYSICSABILITIES_API UCommonHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

};

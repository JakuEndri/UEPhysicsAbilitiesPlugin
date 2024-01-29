// Copyright Endri Jaku. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterPhysicsAbilitiesWidget.generated.h"

/**
 *
 */
UCLASS()
class PHYSICSABILITIES_API UCharacterPhysicsAbilitiesWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	/*Plays the fail animation for the duration of the Timer given as a parameter*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "WidgetAnimation")
	void PlayGrabFailAnimation(const FTimerHandle& Timer);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "WidgetAnimation")
	void PlayGrabNoHitAnimation(const FTimerHandle& Timer);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "WidgetAnimation")
	void PlayGrabWrongObjectAnimation(const FTimerHandle& Timer);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "WidgetAnimation")
	void PlayGrabSucceedAnimation(const FTimerHandle& Timer);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "WidgetAnimation")
	void PlayDropFailAnimation(const FTimerHandle& Timer);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "WidgetAnimation")
	void PlayDropSucceedAnimation();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "WidgetAnimation")
	void PlayPokeFailAnimation(const FTimerHandle& Timer);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "WidgetAnimation")
	void PlayPokeNoHitAnimation(const FTimerHandle& Timer);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "WidgetAnimation")
	void PlayPokeWrongObjectAnimation(const FTimerHandle& Timer);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "WidgetAnimation")
	void PlayPokeSucceedAnimation(const FTimerHandle& Timer);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "WidgetAnimation")
	void PlayThrowFailAnimation(const FTimerHandle& Timer);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "WidgetAnimation")
	void PlayThrowStartSucceedAnimation(const FTimerHandle& Timer);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "WidgetAnimation")
	void PlayThrowSucceedAnimation(const FTimerHandle& Timer);

};

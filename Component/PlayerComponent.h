// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "InputMappingContext.h"

#include "PlayerComponent.generated.h"

class UInputConfig;
struct FGameplayTag;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LYRASTUDYPROJECT_API UPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputConfig* InputConfig;

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//IA에 함수를 바인드하는 내용으로, 추후에 캐릭터에서 실행할 것이다.
	void InitializePlayerInput(UInputComponent* PlayerInputComponent);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "PlayerAbilitySystemComponent.h"
#include "PlayerComponent.h"
#include "PlayerInputComponent.h"
#include "AbilitySet.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class LYRASTUDYPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPlayerAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	UPlayerComponent* PlayerComponent;

	UPROPERTY(EditDefaultsOnly)
	UAbilitySet* AbilitySet;

	FAbilitySet_GrantedHandles GrantedHandles;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION()
	void OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController);

	void GiveAbilitySet();
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerComponent.h"
#include "PlayerInputComponent.h"
#include "PlayerGameplayTags.h"
#include "InputConfig.h"
#include "PlayerAbilitySystemComponent.h"

// Sets default values for this component's properties
UPlayerComponent::UPlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);


	UE_LOG(LogTemp, Warning, TEXT("UPlayerComponent::InitializePlayerInput"));

	APawn* Pawn = Cast<APawn>(GetOwner());
	/*lyra에서는 GameFrameworkComponent 계열에서 정의해서 현 컴포넌트가 붙어있는
	Pawn에 정보를 받아오는 함수를 구현하고 그것을 사용한다.*/
	if (!Pawn) return;

	APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
	if (!PC) return;
	//위와 동일한 상황으로, 현 컴포넌트는 Player전용이기에 AI라면 막는것이다.

	/*
		Lyra에서는 PawnData라는 데이터 에셋으로 캐릭터 정보, InputConfig 등을 받아오고,
		또한 변수로 IMC를 받아와서 새로 생성한 캐릭터에 등록하는 함수도 작성되어 있으나,
		이번 적용은 단순히 변수로 생성해서 받아오는 방식으로 대체한다.
	*/

	if (InputConfig)
	{
		UPlayerInputComponent* PlayerIC = Cast<UPlayerInputComponent>(PlayerInputComponent);
		if (ensureMsgf(PlayerIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UPlayerInputComponent or a subclass of it.")))
		{
			UE_LOG(LogTemp, Warning, TEXT("UPlayerComponent::InitializePlayerInput_PlayerIC_Cast_Valid"));
			TArray<uint32> BindHandles;
			PlayerIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

			PlayerIC->BindNativeAction(InputConfig, PlayerGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
			PlayerIC->BindNativeAction(InputConfig, PlayerGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look, /*bLogIfNotFound=*/ false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UPlayerComponent::InitializePlayerInput_PlayerIC_Cast_InValid"));
		}
	}
}

void UPlayerComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		//ASC를 불러와서 현재 InputTag를 전달한다. --> ASC를 구현하고 다시 진행한다.
		if (UPlayerAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UPlayerAbilitySystemComponent>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Character -> ASC Pressed: %s"), *InputTag.ToString());
			ASC->AbilityInputTagPressed(InputTag);
		}

	}
}

void UPlayerComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		//ASC를 불러와서 현재 InputTag를 제거한다.
		if (UPlayerAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UPlayerAbilitySystemComponent>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Character -> ASC Pressed: %s"), *InputTag.ToString());
			ASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void UPlayerComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	//UE_LOG(LogTemp, Warning, TEXT("Character -> Move"));

	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn) return;
	APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
	if (!PC) return;

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	FVector MoveDirection = Pawn->GetActorForwardVector() * Value.Y + Pawn->GetActorRightVector() * Value.X;

	MoveDirection.Normalize(); //대각선도 단위벡터로 정규화하는 코드!!!
	Pawn->AddMovementInput(MoveDirection);
}

void UPlayerComponent::Input_Look(const FInputActionValue& InputActionValue)
{
	//UE_LOG(LogTemp, Warning, TEXT("Character -> Look"));

	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn) return;

	FVector2D Value = InputActionValue.Get<FVector2D>();
	Pawn->AddControllerPitchInput(Value.Y);
	Pawn->AddControllerYawInput(Value.X);
}

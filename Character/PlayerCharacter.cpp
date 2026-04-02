// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h" //UEnhancedInputLocalPlayerSubsystem를 위함
#include "EnhancedInputComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//각 컴포넌트 생성하기
	AbilitySystemComponent = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(TEXT("PlayerAbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	PlayerComponent = CreateDefaultSubobject<UPlayerComponent>(TEXT("PlayerCharacterComponent"));
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//IMC를 컨트롤러에 연결하기
	ReceiveControllerChangedDelegate.AddDynamic(this, &APlayerCharacter::OnControllerChanged);
	AController* BeginController = GetController();
	if (BeginController)
	{
		OnControllerChanged(this, nullptr, BeginController);
	}

	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AbilitySystemComponent) //GA를 실행시키기 위한 구성!
	{
		AbilitySystemComponent->ProcessAbilityInput(DeltaTime, false);
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerComponent)
	{
		PlayerComponent->InitializePlayerInput(InputComponent);
		//IA에 함수를 바인드 하기 위해 실행하는 함수.
	}
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this); //ASC 초기화

		GiveAbilitySet(); //GA의 등록은 ASC 초기화 이후에 해야한다.


		UE_LOG(LogTemp, Warning, TEXT("Ability Num: %d"), AbilitySystemComponent->GetActivatableAbilities().Num());
	}
}

void APlayerCharacter::OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
	APlayerController* PC = Cast<APlayerController>(NewController);
	if (PC)
	{
		ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();

		if (LocalPlayer)
		{
			UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
				LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

			if (InputSubsystem)
			{
				InputSubsystem->AddMappingContext(InputMappingContext, 0);
			}
		}

	}
}

void APlayerCharacter::GiveAbilitySet()
{
	if (AbilitySet && AbilitySystemComponent)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &GrantedHandles, this);
	}
}


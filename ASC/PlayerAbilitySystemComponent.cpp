// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAbilitySystemComponent.h"
#include "PlayerGameplayAbility.h"

UPlayerAbilitySystemComponent::UPlayerAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UPlayerAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	UE_LOG(LogTemp, Warning, TEXT("InputTag Pressed: %s"), *InputTag.ToString());

	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UPlayerAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UPlayerAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{

	UE_LOG(LogTemp, Warning, TEXT("PressedHandles Num: %d"), InputPressedSpecHandles.Num());
	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	//위 배열은 Lyra에서 static 구현이지만, 내 직접 만든 코드는 불완전하기에 배제한다.
	AbilitiesToActivate.Reset();

	//이번 적용에서는 일회성 어빌리티만을 실행한다.

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		/*
			AbilitySpecHandle은 어빌리티의 ID 역할이고,
			AbilitySpec은 실제 어빌리티의 실행정보를 담고 있다.
		*/
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UPlayerGameplayAbility* PlayerAbilityCDO = Cast<UPlayerGameplayAbility>(AbilitySpec->Ability);

					if (PlayerAbilityCDO)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
					/*
						LyraAbilityCDO->GetActivationPolicy() == ELyraAbilityActivationPolicy::OnInputTriggered
						조건문에 이와 같은 실행정책을 확인하는 코드가 Lyra에는 있다.
						이것으로 눌렸을때, 누르고 있을때를 구분 실행할수 있으나,
						현재는 생략한다.
					*/
				}
			}
		}
	}


	//위에서 확인한 AbilitySpec의 정보로 어빌리티를 실행한다.
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryActivateAbility Called"));
		TryActivateAbility(AbilitySpecHandle);
	}


	//실행이후에, AbilitySpec의 InputPressed를 false로 바꾼다.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}


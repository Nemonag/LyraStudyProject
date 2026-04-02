// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySet.h"
#include "PlayerGameplayAbility.h"
#include "PlayerAbilitySystemComponent.h"

void FAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle); // 나중에 제거하기 위한 기록용이다.
	}
}


UAbilitySet::UAbilitySet(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

//ASC에 GiveAbility 실행 함수 -> 추후 캐릭터에서 실행한다.
void UAbilitySet::GiveToAbilitySystem(UPlayerAbilitySystemComponent* PlayerASC, FAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(PlayerASC);

	//if (!(PlayerASC->IsOwnerActorAuthoritative())) return;

	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UPlayerGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UPlayerGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = PlayerASC->GiveAbility(AbilitySpec);
	}

	/*
		어빌리티와 InputTag 등의 정보가 Spec에 저장되고,
		ASC의 GiveAbility로 인해 등록된다.
	*/
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/BSMontageActionData.h"

UAnimMontage* UBSMontageActionData::GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index) const
{
	if (MontageGroupMap.Contains(GroupTag))
	{
		const FBSMontageGroup& MontageGroup = MontageGroupMap[GroupTag];

		if (MontageGroup.Animations.Num() > 0 && MontageGroup.Animations.Num() > Index)
		{
			return MontageGroup.Animations[Index];
		}
	}

	return nullptr;
}

UAnimMontage* UBSMontageActionData::GetRandomMontageForTag(const FGameplayTag& GroupTag) const
{
	if (MontageGroupMap.Contains(GroupTag))
	{
		const FBSMontageGroup& MontageGroup = MontageGroupMap[GroupTag];

		int32 Index = FMath::RandRange(0, MontageGroup.Animations.Num() - 1);

		return MontageGroup.Animations[Index];
	}

	return nullptr;
}

// Copyright © 2021++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "RpmAvatarCreatorTypes.h"

class FPayloadExtractor
{
public:
	static EAvatarGender GetGenderFromString(const FString& GenderStr);

	static FRpmAvatarProperties ExtractPayload(const FString& JsonString);
	
	static FString ExtractAvatarId(const FString& JsonString);
	
	static FString MakeCreatePayload(const FRpmAvatarProperties& AvatarProperties);

	static FString MakeUpdatePayload(ERpmPartnerAssetType AssetType, int64 AssetId);
};

// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "RpmAvatarCreatorTypes.h"

class FPartnerAssetExtractor
{
public:
	static TArray<FRpmPartnerAsset> ExtractAssets(const FString& JsonString);
	static TArray<FRpmColorPalette> ExtractColors(const FString& JsonString);
};

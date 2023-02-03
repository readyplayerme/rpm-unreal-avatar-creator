// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "RpmAvatarCreatorTypes.h"

class FPayloadUpdater
{
public:
	static FRpmAvatarProperties MakeAvatarProperties();
	static bool UpdatePayload(FRpmAvatarProperties& AvatarProperties, ERpmPartnerAssetType AssetType, int64 AssetId);
};

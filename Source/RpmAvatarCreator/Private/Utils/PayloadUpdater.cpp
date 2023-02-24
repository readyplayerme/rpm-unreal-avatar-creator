// Copyright © 2023++ Ready Player Me

#include "PayloadUpdater.h"

bool FPayloadUpdater::UpdatePayload(FRpmAvatarProperties& AvatarProperties, ERpmPartnerAssetType AssetType, int64 AssetId)
{
	if (AvatarProperties.Assets.Contains(AssetType))
	{
		if (AvatarProperties.Assets[AssetType] == AssetId)
		{
			return false;
		}
		AvatarProperties.Assets[AssetType] = AssetId;
	}
	AvatarProperties.Assets.Add(AssetType, AssetId);
	return true;
}

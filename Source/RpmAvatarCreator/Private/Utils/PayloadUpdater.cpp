// Copyright © 2023++ Ready Player Me

#include "PayloadUpdater.h"

FRpmAvatarProperties FPayloadUpdater::MakeAvatarProperties()
{
	FRpmAvatarProperties AvatarProperties;
	AvatarProperties.Partner = "demo";
	AvatarProperties.Gender = EAvatarGender::Masculine;
	AvatarProperties.BodyType = EAvatarBodyType::FullBody;
	AvatarProperties.Colors =
	{
		{ERpmPartnerAssetColor::SkinColor, 5},
		{ERpmPartnerAssetColor::BeardColor, 0},
		{ERpmPartnerAssetColor::HairColor, 0},
		{ERpmPartnerAssetColor::EyebrowColor, 0}
	};
	AvatarProperties.Assets =
	{
		{ERpmPartnerAssetType::EyeColor, 9781796},
		{ERpmPartnerAssetType::HairStyle, 23368535},
		{ERpmPartnerAssetType::EyebrowStyle, 41308196},
		{ERpmPartnerAssetType::Shirt, 9247449},
		{ERpmPartnerAssetType::Outfit, 109373713}
	};
	return AvatarProperties;
}

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

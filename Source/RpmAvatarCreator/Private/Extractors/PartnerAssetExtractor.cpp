// Copyright © 2022++ Ready Player Me

#include "PartnerAssetExtractor.h"
#include "PayloadExtractor.h"

#include "Templates/SharedPointer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

namespace
{
	const TMap<FString, ERpmPartnerAssetType> STRING_TO_ASSET_TYPES_MAP =
	{
		{"beard", ERpmPartnerAssetType::BeardStyle},
		{"eye", ERpmPartnerAssetType::EyeColor},
		{"eyeshape", ERpmPartnerAssetType::EyeShape},
		{"eyebrows", ERpmPartnerAssetType::EyebrowStyle},
		{"facemask", ERpmPartnerAssetType::FaceMask},
		{"faceshape", ERpmPartnerAssetType::FaceShape},
		{"glasses", ERpmPartnerAssetType::Glasses},
		{"hair", ERpmPartnerAssetType::HairStyle},
		{"headwear", ERpmPartnerAssetType::Headwear},
		{"facewear", ERpmPartnerAssetType::Facewear},
		{"lipshape", ERpmPartnerAssetType::LipShape},
		{"noseshape", ERpmPartnerAssetType::NoseShape},
		{"outfit", ERpmPartnerAssetType::Outfit},
		{"shirt", ERpmPartnerAssetType::Shirt}
	};

	ERpmPartnerAssetType ConvertAssetType(const FString& AssetType)
	{
		if (STRING_TO_ASSET_TYPES_MAP.Contains(AssetType))
		{
			return STRING_TO_ASSET_TYPES_MAP[AssetType];
		}
		return ERpmPartnerAssetType::None;
	}
}

TArray<FRpmPartnerAsset> FPartnerAssetExtractor::ExtractAssets(const FString& JsonString)
{
	TArray<FRpmPartnerAsset> Assets;
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(Reader, JsonArray))
	{
		return {};
	}
	for (const auto& JsonValue : JsonArray)
	{
		FRpmPartnerAsset Asset;
		const auto JsonObject = JsonValue->AsObject();
		if (JsonObject->HasField("id"))
		{
			Asset.Id = JsonObject->GetIntegerField("id");
		}
		if (JsonObject->HasField("name"))
		{
			Asset.Name = JsonObject->GetStringField("name");
		}
		if (JsonObject->HasField("assetType"))
		{
			Asset.AssetType = ConvertAssetType(JsonObject->GetStringField("assetType"));
		}
		if (JsonObject->HasField("gender"))
		{
			Asset.Gender = FPayloadExtractor::GetGenderFromString(JsonObject->GetStringField("gender"));
		}
		if (JsonObject->HasField("icon"))
		{
			Asset.Icon = JsonObject->GetStringField("icon");
		}
		if (Asset.AssetType == ERpmPartnerAssetType::EyeColor && JsonObject->HasField("mask"))
		{
			Asset.Icon = JsonObject->GetStringField("mask");
		}
		if (JsonObject->HasField("model"))
		{
			Asset.Model = JsonObject->GetStringField("model");
		}
		Assets.Add(Asset);
	}
	return Assets;
}
// Copyright © 2023++ Ready Player Me

#include "PartnerAssetExtractor.h"

#include "DataJsonUtils.h"
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

	const TMap<ERpmPartnerAssetColor, FString> COLOR_TO_STRING_MAP =
	{
		{ERpmPartnerAssetColor::BeardColor, "beard"},
		{ERpmPartnerAssetColor::EyebrowColor, "eyebrow"},
		{ERpmPartnerAssetColor::HairColor, "hair"},
		{ERpmPartnerAssetColor::SkinColor, "skin"},
	};
}

TArray<FRpmColorPalette> FPartnerAssetExtractor::ExtractColors(const FString& JsonString)
{
	TArray<FRpmColorPalette> Colors;

	const TSharedPtr<FJsonObject> DataObject = FDataJsonUtils::ExtractDataObject(JsonString);
	if (!DataObject)
	{
		return {};
	}

	for (const auto& Item : COLOR_TO_STRING_MAP)
	{
		if (DataObject->HasTypedField<EJson::Array>(Item.Value))
		{
			TArray<FColor> ColorArray;
			for (const auto& ColorItem : DataObject->GetArrayField(Item.Value))
			{
				const FString ColorHex = ColorItem->AsString();
				ColorArray.Add(FColor::FromHex(ColorHex));
			}
			FRpmColorPalette Palette;
			Palette.AssetColor = Item.Key;
			Palette.RgbColors = ColorArray;
			Colors.Add(Palette);
		}
	}
	return Colors;
}

TArray<FRpmPartnerAsset> FPartnerAssetExtractor::ExtractAssets(const FString& JsonString)
{
	TArray<FRpmPartnerAsset> Assets;
	TArray<TSharedPtr<FJsonValue>> JsonArray;

	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), JsonArray))
	{
		return {};
	}
	for (const auto& JsonValue : JsonArray)
	{
		FRpmPartnerAsset Asset;
		const auto JsonObject = JsonValue->AsObject();
		if (JsonObject->HasTypedField<EJson::String>("assetType"))
		{
			const FString AssetTypeStr = JsonObject->GetStringField("assetType");
			if (!STRING_TO_ASSET_TYPES_MAP.Contains(AssetTypeStr))
			{
				continue;
			}
			Asset.AssetType = STRING_TO_ASSET_TYPES_MAP[AssetTypeStr];
		}
		if (JsonObject->HasField("id"))
		{
			Asset.Id = JsonObject->GetIntegerField("id");
		}
		if (JsonObject->HasTypedField<EJson::String>("name"))
		{
			Asset.Name = JsonObject->GetStringField("name");
		}
		if (JsonObject->HasTypedField<EJson::String>("gender"))
		{
			Asset.Gender = FPayloadExtractor::GetGenderFromString(JsonObject->GetStringField("gender"));
		}
		if (JsonObject->HasTypedField<EJson::String>("icon"))
		{
			Asset.Icon = JsonObject->GetStringField("icon");
		}
		if (Asset.AssetType == ERpmPartnerAssetType::EyeColor && JsonObject->HasTypedField<EJson::String>("mask"))
		{
			Asset.Icon = JsonObject->GetStringField("mask");
		}
		if (JsonObject->HasTypedField<EJson::Object>("badgeLogo"))
		{
			const auto BadgeLogoJson = JsonObject->GetObjectField("badgeLogo");
			if (BadgeLogoJson->HasTypedField<EJson::Object>("responsiveImage"))
			{
				const auto ResponsiveImageJson = BadgeLogoJson->GetObjectField("responsiveImage");
				if (ResponsiveImageJson->HasTypedField<EJson::String>("src"))
				{
					Asset.Badge = ResponsiveImageJson->GetStringField("src");
				}
			}
		}
		Assets.Add(Asset);
	}
	return Assets;
}
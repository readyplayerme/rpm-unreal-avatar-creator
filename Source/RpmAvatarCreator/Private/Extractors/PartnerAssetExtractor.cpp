// Copyright © 2023++ Ready Player Me

#include "PartnerAssetExtractor.h"
#include "PayloadExtractor.h"

#include "Templates/SharedPointer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Kismet/KismetStringLibrary.h"

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
	TSharedPtr<FJsonObject> JsonObject;

	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), JsonObject))
	{
		return {};
	}

	if (!JsonObject->HasField("data"))
	{
		return {};
	}

	const auto DataObject = JsonObject->GetObjectField("data");

	for (const auto& Item : COLOR_TO_STRING_MAP)
	{
		if (DataObject->HasField(Item.Value))
		{
			TArray<FColor> ColorArray;
			for (const auto ColorItem : DataObject->GetArrayField(Item.Value))
			{
				const FString ColorHex = ColorItem->AsString();
				// const int32 RgbColor = UKismetStringLibrary::Conv_StringToInt(ColorHex.Replace(TEXT("#"), TEXT("")));
				ColorArray.Add(FColor::FromHex(ColorHex));
			}
			Colors.Add(FRpmColorPalette{Item.Key, ColorArray});
		}
	}
	return Colors;
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
		if (JsonObject->HasField("assetType"))
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
		if (JsonObject->HasField("name"))
		{
			Asset.Name = JsonObject->GetStringField("name");
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
		if (JsonObject->HasField("badgeLogo") && JsonObject->GetObjectField("badgeLogo")->HasField("responsiveImage"))
		{
			const auto BadgeLogoJson = JsonObject->GetObjectField("badgeLogo");
			if (BadgeLogoJson->HasField("responsiveImage"))
			{
				const auto ResponsiveImageJson = BadgeLogoJson->GetObjectField("responsiveImage");
				if (ResponsiveImageJson->HasField("src"))
				{
					Asset.Badge = ResponsiveImageJson->GetStringField("src");
				}
			}
		}
		Assets.Add(Asset);
	}
	return Assets;
}
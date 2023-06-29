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

	const FString JSON_FIELD_ASSET_TYPE = "assetType";
	const FString JSON_FIELD_ID = "id";
	const FString JSON_FIELD_NAME = "name";
	const FString JSON_FIELD_GENDER = "gender";
	const FString JSON_FIELD_ICON = "icon";
	const FString JSON_FIELD_MASK = "mask";
	const FString JSON_FIELD_BADGE_LOGO = "badgeLogo";
	const FString JSON_FIELD_RESPONSIVE_IMAGE = "responsiveImage";
	const FString JSON_FIELD_SRC = "src";
	const FString JSON_FIELD_IS_LOCKED = "locked";
	const FString JSON_FIELD_IS_LOCKED_CATEGORIES = "lockedCategories";
	const FString JSON_FIELD_PRICE = "price";
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
		if (JsonObject->HasTypedField<EJson::String>(JSON_FIELD_ASSET_TYPE))
		{
			const FString AssetTypeStr = JsonObject->GetStringField(JSON_FIELD_ASSET_TYPE);
			if (!STRING_TO_ASSET_TYPES_MAP.Contains(AssetTypeStr))
			{
				continue;
			}
			Asset.AssetType = STRING_TO_ASSET_TYPES_MAP[AssetTypeStr];
		}
		if (JsonObject->HasField(JSON_FIELD_ID))
		{
			Asset.Id = JsonObject->GetIntegerField(JSON_FIELD_ID);
		}
		if (JsonObject->HasTypedField<EJson::String>(JSON_FIELD_NAME))
		{
			Asset.Name = JsonObject->GetStringField(JSON_FIELD_NAME);
		}
		if (JsonObject->HasTypedField<EJson::String>(JSON_FIELD_GENDER))
		{
			Asset.Gender = FPayloadExtractor::GetGenderFromString(JsonObject->GetStringField(JSON_FIELD_GENDER));
		}
		if (JsonObject->HasTypedField<EJson::String>(JSON_FIELD_ICON))
		{
			Asset.Icon = JsonObject->GetStringField(JSON_FIELD_ICON);
		}
		if (Asset.AssetType == ERpmPartnerAssetType::EyeColor && JsonObject->HasTypedField<EJson::String>(JSON_FIELD_MASK))
		{
			Asset.Icon = JsonObject->GetStringField(JSON_FIELD_MASK);
		}
		if (JsonObject->HasTypedField<EJson::Object>(JSON_FIELD_BADGE_LOGO))
		{
			const auto BadgeLogoJson = JsonObject->GetObjectField(JSON_FIELD_BADGE_LOGO);
			if (BadgeLogoJson->HasTypedField<EJson::Object>(JSON_FIELD_RESPONSIVE_IMAGE))
			{
				const auto ResponsiveImageJson = BadgeLogoJson->GetObjectField(JSON_FIELD_RESPONSIVE_IMAGE);
				if (ResponsiveImageJson->HasTypedField<EJson::String>(JSON_FIELD_SRC))
				{
					Asset.Badge = ResponsiveImageJson->GetStringField(JSON_FIELD_SRC);
				}
			}
		}
		if (JsonObject->HasTypedField<EJson::Boolean>(JSON_FIELD_IS_LOCKED))
		{
			Asset.bIsLocked = JsonObject->GetBoolField(JSON_FIELD_IS_LOCKED);
		}
		if (JsonObject->HasTypedField<EJson::String>(JSON_FIELD_PRICE))
		{
			Asset.Price = JsonObject->GetStringField(JSON_FIELD_PRICE);
		}
		if (JsonObject->HasTypedField<EJson::Array>(JSON_FIELD_IS_LOCKED_CATEGORIES))
		{
			Asset.bIsCustomizable = JsonObject->GetArrayField(JSON_FIELD_IS_LOCKED_CATEGORIES).Num() == 0;
		}
		Assets.Add(Asset);
	}
	return Assets;
}
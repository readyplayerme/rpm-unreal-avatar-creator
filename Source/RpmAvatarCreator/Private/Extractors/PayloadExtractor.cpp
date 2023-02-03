// Copyright © 2023++ Ready Player Me

#include "PayloadExtractor.h"

#include "Misc/DefaultValueHelper.h"
#include "Templates/SharedPointer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

namespace
{
	template <typename KeyType, typename ValueType>
	TMap<ValueType, KeyType> GetReverseMapping(const TMap<KeyType, ValueType>& InMap)
	{
		TMap<ValueType, KeyType> ReverseMapping;
		for (const auto& Item : InMap)
		{
			ReverseMapping.Add(Item.Value, Item.Key);
		}
		return ReverseMapping;
	}

	const TMap<EAvatarBodyType, FString> BODY_TYPE_TO_STRING_MAP =
	{
		{EAvatarBodyType::FullBody, "fullbody"},
		{EAvatarBodyType::HalfBody, "halfbody"},
	};
	const TMap<FString, EAvatarBodyType> STRING_TO_BODY_TYPE_MAP = GetReverseMapping(BODY_TYPE_TO_STRING_MAP);
	const TMap<EAvatarGender, FString> GENDER_TO_STRING_MAP =
	{
		{EAvatarGender::Masculine, "male"},
		{EAvatarGender::Feminine, "female"},
	};
	const TMap<FString, EAvatarGender> STRING_TO_GENDER_MAP = GetReverseMapping(GENDER_TO_STRING_MAP);

	EAvatarBodyType StringToBodyType(const FString& BodyTypeField)
	{
		return STRING_TO_BODY_TYPE_MAP.Contains(BodyTypeField) ? STRING_TO_BODY_TYPE_MAP[BodyTypeField] : EAvatarBodyType::Undefined;
	}

	EAvatarGender StringToGender(const FString& GenderField)
	{
		return STRING_TO_GENDER_MAP.Contains(GenderField) ? STRING_TO_GENDER_MAP[GenderField] : EAvatarGender::Undefined;
	}

	FString BodyTypeToString(EAvatarBodyType BodyType)
	{
		return BODY_TYPE_TO_STRING_MAP.Contains(BodyType) ? BODY_TYPE_TO_STRING_MAP[BodyType] : "";
	}

	FString GenderToString(EAvatarGender Gender)
	{
		return GENDER_TO_STRING_MAP.Contains(Gender) ? GENDER_TO_STRING_MAP[Gender] : "";
	}

	const TMap<ERpmPartnerAssetType, FString> ASSET_TYPE_TO_STRING_MAP =
	{
		{ERpmPartnerAssetType::BeardStyle, "beardStyle"},
		{ERpmPartnerAssetType::EyeColor, "eyeColor"},
		{ERpmPartnerAssetType::EyeShape, "eyeShape"},
		{ERpmPartnerAssetType::EyebrowStyle, "eyebrowStyle"},
		{ERpmPartnerAssetType::FaceMask, "faceMask"},
		{ERpmPartnerAssetType::FaceShape, "faceShape"},
		{ERpmPartnerAssetType::Glasses, "glasses"},
		{ERpmPartnerAssetType::HairStyle, "hairStyle"},
		{ERpmPartnerAssetType::Headwear, "headwear"},
		{ERpmPartnerAssetType::Facewear, "facewear"},
		{ERpmPartnerAssetType::LipShape, "lipShape"},
		{ERpmPartnerAssetType::NoseShape, "noseShape"},
		{ERpmPartnerAssetType::Outfit, "outfit"},
		{ERpmPartnerAssetType::Shirt, "shirt"}
	};
	const TMap<ERpmPartnerAssetColor, FString> ASSET_COLOR_TO_STRING_MAP =
	{
		{ERpmPartnerAssetColor::BeardColor, "beardColor"},
		{ERpmPartnerAssetColor::EyebrowColor, "eyebrowColor"},
		{ERpmPartnerAssetColor::HairColor, "hairColor"},
		{ERpmPartnerAssetColor::SkinColor, "skinColor"},
	};
}

EAvatarGender FPayloadExtractor::GetGenderFromString(const FString& GenderStr)
{
	return StringToGender(GenderStr);
}

FRpmAvatarProperties FPayloadExtractor::ExtractPayload(const FString& JsonString)
{
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return {};
	}

	if (!JsonObject->HasField("data"))
	{
		return {};
	}

	const auto DataObject = JsonObject->GetObjectField("data");

	FRpmAvatarProperties AvatarProperties;
	AvatarProperties.Partner = DataObject->GetStringField("partner");
	AvatarProperties.Gender = StringToGender(DataObject->GetStringField("gender"));
	AvatarProperties.BodyType = StringToBodyType(DataObject->GetStringField("bodyType"));
	
	if (!DataObject->HasField("assets"))
	{
		return {};
	}
	const auto AssetsObject = DataObject->GetObjectField("assets");
	for (const auto& Item : ASSET_TYPE_TO_STRING_MAP)
	{
		if (AssetsObject->HasField(Item.Value))
		{
			int64 AssetId;
			FDefaultValueHelper::ParseInt64(AssetsObject->GetStringField(Item.Value), AssetId);
			AvatarProperties.Assets.Add(Item.Key, AssetId);
		}
	}
	for (const auto& Item : ASSET_TYPE_TO_STRING_MAP)
	{
		if (AssetsObject->HasField(Item.Value))
		{
			int64 AssetId;
			FDefaultValueHelper::ParseInt64(AssetsObject->GetStringField(Item.Value), AssetId);
			AvatarProperties.Assets.Add(Item.Key, AssetId);
		}
	}
	for (const auto& Item : ASSET_COLOR_TO_STRING_MAP)
	{
		if (AssetsObject->HasField(Item.Value))
		{
			AvatarProperties.Colors.Add(Item.Key, AssetsObject->GetNumberField(Item.Value));
		}
	}

	return AvatarProperties;
}

FString FPayloadExtractor::ExtractAvatarId(const FString& JsonString)
{
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return {};
	}

	if (!JsonObject->HasField("data"))
	{
		return {};
	}

	const auto DataObject = JsonObject->GetObjectField("data");
	if (!DataObject->HasField("id"))
	{
		return {};
	}
	return DataObject->GetStringField("id");
}

FString FPayloadExtractor::MakeCreatePayload(const FRpmAvatarProperties& AvatarProperties)
{
	FString OutputJsonString;

	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	const TSharedPtr<FJsonObject> DataObject = MakeShared<FJsonObject>();
	const TSharedPtr<FJsonObject> AssetsObject = MakeShared<FJsonObject>();

	JsonObject->SetObjectField("data", DataObject);
	DataObject->SetObjectField("assets", AssetsObject);

	DataObject->SetStringField("partner", AvatarProperties.Partner);
	DataObject->SetStringField("gender",  GenderToString(AvatarProperties.Gender));
	DataObject->SetStringField("bodyType", BodyTypeToString(AvatarProperties.BodyType));

	for (const auto& Item : AvatarProperties.Assets)
	{
		AssetsObject->SetStringField(ASSET_TYPE_TO_STRING_MAP[Item.Key], FString::FromInt(Item.Value));
	}
	for (const auto& Item : AvatarProperties.Colors)
	{
		AssetsObject->SetNumberField(ASSET_COLOR_TO_STRING_MAP[Item.Key], Item.Value);
	}

	const auto Writer = TJsonWriterFactory<>::Create(&OutputJsonString);
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		// UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to create a valid url"));
	}
	return OutputJsonString;
}

FString FPayloadExtractor::MakeUpdatePayload(ERpmPartnerAssetType AssetType, int64 AssetId)
{
	FString OutputJsonString;

	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	const TSharedPtr<FJsonObject> DataObject = MakeShared<FJsonObject>();
	const TSharedPtr<FJsonObject> AssetsObject = MakeShared<FJsonObject>();

	JsonObject->SetObjectField("data", DataObject);
	DataObject->SetObjectField("assets", AssetsObject);

	const FString AssetIdStr = AssetId != 0 ? FString::FromInt(AssetId) : "";
	AssetsObject->SetStringField(ASSET_TYPE_TO_STRING_MAP[AssetType], AssetIdStr);

	const auto Writer = TJsonWriterFactory<>::Create(&OutputJsonString);
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		// UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to create a valid url"));
	}
	return OutputJsonString;
}

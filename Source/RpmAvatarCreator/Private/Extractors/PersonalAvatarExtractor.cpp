// Copyright © 2023++ Ready Player Me


#include "PersonalAvatarExtractor.h"

#include "Templates/SharedPointer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

namespace
{
	const FString JSON_FIELD_DATA = "data";
	const FString JSON_FIELD_PARTNER = "partner";
	const FString JSON_FIELD_ID = "id";

	TArray<TSharedPtr<FJsonValue>> ExtractDataObject(const FString& JsonString)
	{
		TSharedPtr<FJsonObject> JsonObject;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

		if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject->HasField(JSON_FIELD_DATA))
		{
			return {};
		}
	
		return JsonObject->GetArrayField(JSON_FIELD_DATA);
	}
}

TArray<FRpmPersonalAvatar> FPersonalAvatarExtractor::ExtractPersonalAvatars(const FString& JsonString)
{
	const TArray<TSharedPtr<FJsonValue>> JsonArray = ExtractDataObject(JsonString);
	TArray<FRpmPersonalAvatar> PersonalAvatars;
	for (const auto& JsonValue : JsonArray)
	{
		const auto JsonObject = JsonValue->AsObject();
		if (JsonObject->HasTypedField<EJson::String>(JSON_FIELD_PARTNER) && JsonObject->HasTypedField<EJson::String>(JSON_FIELD_ID))
		{
			FRpmPersonalAvatar PersonalAvatar;
			PersonalAvatar.Partner = JsonObject->GetStringField(JSON_FIELD_PARTNER);
			PersonalAvatar.Id = JsonObject->GetStringField(JSON_FIELD_ID);
			PersonalAvatars.Add(PersonalAvatar);
		}
	}
	return PersonalAvatars;
}

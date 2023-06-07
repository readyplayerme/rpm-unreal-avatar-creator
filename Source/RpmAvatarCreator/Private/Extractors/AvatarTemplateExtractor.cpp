// Copyright © 2023++ Ready Player Me


#include "AvatarTemplateExtractor.h"

#include "Templates/SharedPointer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

namespace
{
	const FString JSON_FIELD_DATA = "data";
	const FString JSON_FIELD_IMAGE_URL = "imageUrl";
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

TArray<FRpmAvatarTemplate> FAvatarTemplateExtractor::ExtractAvatarTemplates(const FString& JsonString)
{
	const TArray<TSharedPtr<FJsonValue>> JsonArray = ExtractDataObject(JsonString);
	TArray<FRpmAvatarTemplate> AvatarTemplates;
	for (const auto& JsonValue : JsonArray)
	{
		const auto JsonObject = JsonValue->AsObject();
		if (JsonObject->HasTypedField<EJson::String>(JSON_FIELD_IMAGE_URL) && JsonObject->HasTypedField<EJson::String>(JSON_FIELD_ID))
		{
			FRpmAvatarTemplate AvatarTemplate;
			AvatarTemplate.ImageUrl = JsonObject->GetStringField(JSON_FIELD_IMAGE_URL);
			AvatarTemplate.Id = JsonObject->GetStringField(JSON_FIELD_ID);
			AvatarTemplates.Add(AvatarTemplate);
		}
	}
	return AvatarTemplates;
}

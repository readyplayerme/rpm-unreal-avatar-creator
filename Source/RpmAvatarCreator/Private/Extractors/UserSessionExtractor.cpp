// Copyright © 2023++ Ready Player Me


#include "UserSessionExtractor.h"

#include "Templates/SharedPointer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

TOptional<FRpmUserSession> FUserSessionExtractor::ExtractUserSession(const FString& JsonString)
{
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		if (JsonObject->HasField("data"))
		{
			const TSharedPtr<FJsonObject> DataObject = JsonObject->GetObjectField("data");
			if (DataObject->HasField("id") && DataObject->HasField("token"))
			{
				FRpmUserSession Session;
				Session.Id = DataObject->GetStringField("id");
				Session.Token = DataObject->GetStringField("token");
				if (!Session.Id.IsEmpty() && !Session.Token.IsEmpty())
				{
					return Session;
				}
			}
		}
	}

	return {};
}
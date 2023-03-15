// Copyright © 2023++ Ready Player Me


#include "UserSessionExtractor.h"

#include "Templates/SharedPointer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

TOptional<FRpmUserSession> FUserSessionExtractor::ExtractUserSession(const FString& JsonString)
{
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject->HasField("data"))
	{
		return {};
	}
	
	const TSharedPtr<FJsonObject> DataObject = JsonObject->GetObjectField("data");
	if (!DataObject->HasField("id") || !DataObject->HasField("token"))
	{
		return {};
	}

	FRpmUserSession Session;
	Session.Id = DataObject->GetStringField("id");
	Session.Token = DataObject->GetStringField("token");
	return Session;
}

TOptional<FRpmUserData> FUserSessionExtractor::ExtractUserData(const FString& JsonString)
{
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject->HasField("data"))
	{
		return {};
	}
	const TSharedPtr<FJsonObject> DataObject = JsonObject->GetObjectField("data");
	if (!DataObject->HasField("_id") || !DataObject->HasField("token"))
	{
		return {};
	}

	FRpmUserData UserData;
	UserData.Id = DataObject->GetStringField("_id");
	UserData.Name = DataObject->GetStringField("name");
	UserData.Email = DataObject->GetStringField("email");
	UserData.Token = DataObject->GetStringField("token");
	UserData.RefreshToken = DataObject->GetStringField("refreshToken");
	return UserData;
}

FString FUserSessionExtractor::MakeDataPayload(const TSharedPtr<FJsonObject> DataObject)
{
	FString OutputJsonString;

	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	JsonObject->SetObjectField("data", DataObject);

	const auto Writer = TJsonWriterFactory<>::Create(&OutputJsonString);
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		UE_LOG(LogRpmAvatarCreator, Warning, TEXT("Failed to create a valid url"));
	}
	return OutputJsonString;
}

FString FUserSessionExtractor::MakeSendCodePayload(const FString& Email)
{
	const TSharedPtr<FJsonObject> DataObject = MakeShared<FJsonObject>();
	DataObject->SetStringField("email", Email);
	DataObject->SetStringField("authType", "code");
	return MakeDataPayload(DataObject);
}

FString FUserSessionExtractor::MakeConfirmCodePayload(const FString& Code)
{
	const TSharedPtr<FJsonObject> DataObject = MakeShared<FJsonObject>();
	DataObject->SetStringField("code", Code);
	return MakeDataPayload(DataObject);
}

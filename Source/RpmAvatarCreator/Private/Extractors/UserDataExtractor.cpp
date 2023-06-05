// Copyright © 2023++ Ready Player Me


#include "UserDataExtractor.h"

#include "Templates/SharedPointer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

static const FString JSON_FIELD_DATA = "data";
static const FString JSON_FIELD_TOKEN = "token";
static const FString JSON_FIELD_REFRESH_TOKEN = "refreshToken";
static const FString JSON_FIELD_EMAIL = "email";
static const FString JSON_FIELD_CODE = "code";
static const FString JSON_FIELD_NAME = "name";
static const FString JSON_FIELD_ID = "id";
static const FString JSON_FIELD_AUTH_TYPE = "authType";

namespace
{
	TSharedPtr<FJsonObject> ExtractDataObject(const FString& JsonString)
	{
		TSharedPtr<FJsonObject> JsonObject;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

		if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject->HasField(JSON_FIELD_DATA))
		{
			return {};
		}
	
		return JsonObject->GetObjectField(JSON_FIELD_DATA);
	}

	FString MakeDataPayload(const TSharedPtr<FJsonObject> DataObject)
	{
		FString OutputJsonString;

		const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

		JsonObject->SetObjectField(JSON_FIELD_DATA, DataObject);

		const auto Writer = TJsonWriterFactory<>::Create(&OutputJsonString);
		if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
		{
			UE_LOG(LogRpmAvatarCreator, Warning, TEXT("Failed to create a valid url"));
		}
		return OutputJsonString;
	}
}

FRpmUserData FUserDataExtractor::ExtractAnonymousUserData(const FString& JsonString)
{
	if (const TSharedPtr<FJsonObject> DataObject = ExtractDataObject(JsonString))
	{
		if (!DataObject->HasField(JSON_FIELD_ID) || !DataObject->HasField(JSON_FIELD_TOKEN))
		{
			return {};
		}

		FRpmUserData UserData;
		UserData.bIsAuthenticated = true;
		UserData.bIsExistingUser = false;
		UserData.Id = DataObject->GetStringField(JSON_FIELD_ID);
		UserData.Token = DataObject->GetStringField(JSON_FIELD_TOKEN);
		return UserData;
	}
	return {};
}

FRpmUserData FUserDataExtractor::ExtractRefreshedUserSession(const FString& JsonString)
{
	if (const TSharedPtr<FJsonObject> DataObject = ExtractDataObject(JsonString))
	{
		if (!DataObject->HasField(JSON_FIELD_REFRESH_TOKEN) || !DataObject->HasField(JSON_FIELD_TOKEN))
		{
			return {};
		}

		FRpmUserData UserData;
		UserData.bIsAuthenticated = true;
		UserData.RefreshToken = DataObject->GetStringField(JSON_FIELD_REFRESH_TOKEN);
		UserData.Token = DataObject->GetStringField(JSON_FIELD_TOKEN);
		return UserData;
	}
	return {};
}

FRpmUserData FUserDataExtractor::ExtractUserData(const FString& JsonString)
{
	if (const TSharedPtr<FJsonObject> DataObject = ExtractDataObject(JsonString))
	{
		if (!DataObject->HasField("_id") || !DataObject->HasField(JSON_FIELD_TOKEN))
		{
			return {};
		}

		FRpmUserData UserData;
		UserData.bIsAuthenticated = true;
		UserData.bIsExistingUser = true;
		UserData.Id = DataObject->GetStringField("_id");
		UserData.Name = DataObject->GetStringField(JSON_FIELD_NAME);
		UserData.Email = DataObject->GetStringField(JSON_FIELD_EMAIL);
		UserData.Token = DataObject->GetStringField(JSON_FIELD_TOKEN);
		UserData.RefreshToken = DataObject->GetStringField(JSON_FIELD_REFRESH_TOKEN);
		return UserData;
	}
	return {};
}

FString FUserDataExtractor::MakeAuthStartPayload(const FString& Email, const FString& UserId, bool bIsTypeCode)
{
	const TSharedPtr<FJsonObject> DataObject = MakeShared<FJsonObject>();
	DataObject->SetStringField(JSON_FIELD_EMAIL, Email);
	if (!UserId.IsEmpty())
	{
		DataObject->SetStringField(JSON_FIELD_ID, UserId);
	}
	if (bIsTypeCode)
	{
		DataObject->SetStringField(JSON_FIELD_AUTH_TYPE, JSON_FIELD_CODE);
	}
	return MakeDataPayload(DataObject);
}

FString FUserDataExtractor::MakeConfirmCodePayload(const FString& Code)
{
	const TSharedPtr<FJsonObject> DataObject = MakeShared<FJsonObject>();
	DataObject->SetStringField(JSON_FIELD_CODE, Code);
	return MakeDataPayload(DataObject);
}

FString FUserDataExtractor::MakeTokenRefreshPayload(const FRpmUserData& UserData)
{
	const TSharedPtr<FJsonObject> DataObject = MakeShared<FJsonObject>();
	DataObject->SetStringField(JSON_FIELD_REFRESH_TOKEN, UserData.RefreshToken);
	DataObject->SetStringField(JSON_FIELD_TOKEN, UserData.Token);
	return MakeDataPayload(DataObject);
}

// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "RpmAvatarCreatorTypes.h"

class FUserDataExtractor
{
public:
	static TOptional<FRpmUserData> ExtractAnonymousUserData(const FString& JsonString);

	static TOptional<FRpmUserData> ExtractUserData(const FString& JsonString);

	static TOptional<FRpmUserData> ExtractRefreshedUserSession(const FString& JsonString);

	static FString MakeSendCodePayload(const FString& Email);

	static FString MakeConfirmCodePayload(const FString& Code);

	static FString MakeTokenRefreshPayload(const FRpmUserData& UserData);

private:
	static FString MakeDataPayload(const TSharedPtr<FJsonObject> DataObject);
};

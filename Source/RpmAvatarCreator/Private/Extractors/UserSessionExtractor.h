// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "RpmAvatarCreatorTypes.h"

class FUserSessionExtractor
{
public:
	static TOptional<FRpmUserSession> ExtractUserSession(const FString& JsonString);

	static TOptional<FRpmUserData> ExtractUserData(const FString& JsonString);

	static FString MakeSendCodePayload(const FString& Email);

	static FString MakeConfirmCodePayload(const FString& Code);

private:
	static FString MakeDataPayload(const TSharedPtr<FJsonObject> DataObject);
};

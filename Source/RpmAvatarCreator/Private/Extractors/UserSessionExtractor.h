// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "RpmAvatarCreatorTypes.h"

class FUserSessionExtractor
{
public:
	static TOptional<FRpmUserSession> ExtractUserSession(const FString& JsonString);
};

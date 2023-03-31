// Copyright © 2023++ Ready Player Me

#pragma once

#include "RpmAvatarCreatorTypes.h"

class FPersonalAvatarExtractor
{
public:
	static TArray<FRpmPersonalAvatar> ExtractPersonalAvatars(const FString& JsonString);
};

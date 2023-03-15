// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RpmUserSessionSaveGame.generated.h"

UCLASS()
class RPMAVATARCREATOR_API URpmUserSessionSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Ready Player Me")
	bool bIsAnonymous;

	UPROPERTY(VisibleAnywhere, Category = "Ready Player Me")
	FString Id;

	UPROPERTY(VisibleAnywhere, Category = "Ready Player Me")
	FString Token;

	UPROPERTY(VisibleAnywhere, Category = "Ready Player Me")
	FString RefreshToken;

	UPROPERTY(VisibleAnywhere, Category = "Ready Player Me")
	FString Partner;
};

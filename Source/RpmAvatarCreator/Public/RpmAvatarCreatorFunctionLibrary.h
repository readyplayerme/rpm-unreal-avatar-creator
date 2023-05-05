// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RpmAvatarCreatorFunctionLibrary.generated.h"

/**
 * Static functions to be used from the blueprint.
 */
UCLASS()
class RPMAVATARCREATOR_API URpmAvatarCreatorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Get unique id of the avatar. */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Get Base64 String From Render Target"))
	static FString GetBase64String(class UTextureRenderTarget2D* TextureRenderTarget);
};

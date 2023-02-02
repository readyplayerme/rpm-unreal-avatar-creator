// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "ReadyPlayerMeTypes.h"
#include "GameFramework/Actor.h"
#include "RpmPreviewRenderer.generated.h"

UCLASS()
class RPMAVATARCREATOR_API ARpmPreviewRenderer : public AActor
{
	GENERATED_BODY()
	
public:
	ARpmPreviewRenderer();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* FullBodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* HalfBodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneCaptureComponent2D* SceneCaptureComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* FullBodyFarPin;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* FullBodyNearPin;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* HalfBodyFarPin;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* HalfBodyNearPin;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float ActorRotationSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float ActorZoomSpeed;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Update Body Type"))
	void UpdateBodyType(EAvatarBodyType InBodyType);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Update Avatar"))
	void UpdateAvatar(class USkeletalMesh* Mesh);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Move Camera"))
	void MoveCamera(bool ToFace);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Rotate Avatar"))
	void RotateAvatar(float Delta);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Zoom Camera"))
	void ZoomCamera(float Delta);

protected:
	virtual void BeginPlay() override;

private:
	EAvatarBodyType BodyType;
};

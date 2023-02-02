// Fill out your copyright notice in the Description page of Project Settings.


#include "RpmPreviewRenderer.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/KismetSystemLibrary.h"

ARpmPreviewRenderer::ARpmPreviewRenderer()
	: ActorRotationSpeed(0.2f)
	, ActorZoomSpeed(8.f)
	, BodyType(EAvatarBodyType::Undefined)
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SceneRoot->SetMobility(EComponentMobility::Static);
	SetRootComponent(SceneRoot);

	FullBodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FullBodyMesh"));
	FullBodyMesh->SetupAttachment(RootComponent);

	HalfBodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HalfBodyMesh"));
	HalfBodyMesh->SetupAttachment(RootComponent);

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(RootComponent);

	FullBodyFarPin = CreateDefaultSubobject<USceneComponent>(TEXT("FullBodyFarPin"));
	FullBodyFarPin->SetMobility(EComponentMobility::Static);
	FullBodyFarPin->SetVisibility(false);
	FullBodyFarPin->SetupAttachment(RootComponent);

	FullBodyNearPin = CreateDefaultSubobject<USceneComponent>(TEXT("FullBodyNearPin"));
	FullBodyNearPin->SetMobility(EComponentMobility::Static);
	FullBodyNearPin->SetVisibility(false);
	FullBodyNearPin->SetupAttachment(RootComponent);

	HalfBodyFarPin = CreateDefaultSubobject<USceneComponent>(TEXT("HalfBodyFarPin"));
	HalfBodyFarPin->SetMobility(EComponentMobility::Static);
	HalfBodyFarPin->SetVisibility(false);
	HalfBodyFarPin->SetupAttachment(RootComponent);

	HalfBodyNearPin = CreateDefaultSubobject<USceneComponent>(TEXT("HalfBodyNearPin"));
	HalfBodyNearPin->SetMobility(EComponentMobility::Static);
	HalfBodyNearPin->SetVisibility(false);
	HalfBodyNearPin->SetupAttachment(RootComponent);
}

void ARpmPreviewRenderer::BeginPlay()
{
	Super::BeginPlay();
	SceneCaptureComponent->ShowOnlyActorComponents(this);
}

void ARpmPreviewRenderer::UpdateBodyType(EAvatarBodyType InBodyType)
{
	BodyType = InBodyType;
	FullBodyMesh->SetRelativeRotation(FRotator::ZeroRotator);
	HalfBodyMesh->SetRelativeRotation(FRotator::ZeroRotator);
	const bool IsHalfBody = BodyType == EAvatarBodyType::HalfBody;
	HalfBodyMesh->SetVisibility(IsHalfBody);
	FullBodyMesh->SetVisibility(!IsHalfBody);

	const USceneComponent* TargetPin = IsHalfBody ? HalfBodyNearPin : FullBodyFarPin;
	SceneCaptureComponent->SetRelativeTransform(TargetPin->GetRelativeTransform());
}

void ARpmPreviewRenderer::UpdateAvatar(USkeletalMesh* Mesh)
{
	const bool IsHalfBody = BodyType == EAvatarBodyType::HalfBody;
	USkeletalMeshComponent* TargetMesh = IsHalfBody ? HalfBodyMesh : FullBodyMesh;
	TargetMesh->SetSkeletalMesh(Mesh, false);
}

void ARpmPreviewRenderer::MoveCamera(bool ToFace)
{
	if (BodyType == EAvatarBodyType::HalfBody)
	{
		return;
	}
	const USceneComponent* TargetPin = ToFace ? FullBodyNearPin : FullBodyFarPin;
	
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(SceneCaptureComponent, TargetPin->GetRelativeLocation(), TargetPin->GetRelativeRotation(), true, false, 0.3f, false, EMoveComponentAction::Type::Move, LatentInfo);
}

void ARpmPreviewRenderer::RotateAvatar(float Delta)
{
	const bool IsHalfBody = BodyType == EAvatarBodyType::HalfBody;
	USkeletalMeshComponent* TargetMesh = IsHalfBody ? HalfBodyMesh : FullBodyMesh;
	TargetMesh->AddRelativeRotation(FRotator {0.f, Delta * ActorRotationSpeed, 0.f});
}

void ARpmPreviewRenderer::ZoomCamera(float Delta)
{
	const bool IsHalfBody = BodyType == EAvatarBodyType::HalfBody;
	
	const USceneComponent* TargetPin;
	if (Delta >= 0.f)
	{
		if (IsHalfBody)
		{
			TargetPin = HalfBodyNearPin;
		}
		else
		{
			TargetPin = FullBodyNearPin;
		}
	}
	else
	{
		if (IsHalfBody)
		{
			TargetPin = HalfBodyFarPin;
		}
		else
		{
			TargetPin = FullBodyFarPin;
		}
	}

	const FVector TargetPinLocation = TargetPin->GetRelativeLocation();
	
	const FVector CameraLocation = SceneCaptureComponent->GetRelativeLocation();
	FVector DestinationLocation = CameraLocation - TargetPinLocation;
	DestinationLocation.Normalize();
	
	const float Step = FMath::Min(FMath::Abs(ActorZoomSpeed * Delta), FVector::Distance(CameraLocation, TargetPinLocation));
	DestinationLocation *= Step;
	DestinationLocation = CameraLocation - DestinationLocation;
	SceneCaptureComponent->SetRelativeLocation(DestinationLocation);
}

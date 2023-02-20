// Copyright © 2023++ Ready Player Me

#pragma once

#include "ReadyPlayerMeTypes.h"

#include "RpmAvatarCreatorTypes.generated.h"

struct FRpmUserSession
{
	FString Id;
	FString Token;
};

UENUM(BlueprintType)
enum class ERpmPartnerAssetType : uint8
{
	None,
	BeardStyle,
	EyeColor,
	EyeShape,
	EyebrowStyle,
	FaceMask,
	FaceShape,
	Glasses,
	HairStyle,
	Headwear,
	Facewear,
	LipShape,
	NoseShape,
	Outfit,
	Shirt
};

UENUM(BlueprintType)
enum class ERpmPartnerAssetColor : uint8
{
	// "skinColorHex": "#e9bb98", //replaces v1 skinTone, can't be modified directly, depends on skinColor value
	SkinColor,
	HairColor,
	BeardColor,
	EyebrowColor
};

USTRUCT(BlueprintType)
struct FRpmPartnerAsset
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	int64 Id;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FString Name;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	ERpmPartnerAssetType AssetType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	EAvatarGender Gender;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FString Icon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FString Model;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FString Badge;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UTexture2D* IconTexture;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UTexture2D* BadgeTexture;

	FRpmPartnerAsset()
	{
		Id = 0;
		AssetType = ERpmPartnerAssetType::None;
		Gender = EAvatarGender::Undefined;
		IconTexture = nullptr;
		BadgeTexture = nullptr;
	}
};

USTRUCT(BlueprintType)
struct FRpmAvatarProperties
{
	GENERATED_BODY()
	// "id": "60f8210e8011cfb7d827de46",
	// "userId": "63b25e67f4eb12000fea504b",

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	FString Partner;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	EAvatarGender Gender;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	EAvatarBodyType BodyType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	TMap<ERpmPartnerAssetColor, int32> Colors;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	TMap<ERpmPartnerAssetType, int64> Assets;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	FString Base64Image;
};

UENUM(BlueprintType)
enum class ERpmAvatarCreatorError : uint8
{
	None,
	AuthenticationFailure,
	AssetDownloadFailure,
	AvatarPreviewFailure,
	AvatarSaveFailure
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FAvatarCreatorFailed, ERpmAvatarCreatorError, Error);

DECLARE_DYNAMIC_DELEGATE(FAuthenticationCompleted);

DECLARE_DYNAMIC_DELEGATE_OneParam(FAvatarSaveCompleted, const FString&, Url);

DECLARE_DYNAMIC_DELEGATE(FAvatarEditorReady);

DECLARE_DELEGATE_OneParam(FAvatarCreateCompleted, bool);

DECLARE_DELEGATE_OneParam(FPartnerAssetsDownloadCompleted, bool);

DECLARE_DYNAMIC_DELEGATE_OneParam(FPreviewDownloadCompleted, const USkeletalMesh*, SkeletalMesh);

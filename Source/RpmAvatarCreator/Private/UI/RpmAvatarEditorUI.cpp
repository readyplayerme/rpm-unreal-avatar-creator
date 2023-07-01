// Copyright © 2023++ Ready Player Me

#include "UI/RpmAvatarEditorUI.h"
#include "RpmAvatarCreatorApi.h"
#include "Blueprint/WidgetTree.h"
#include "Components/WrapBox.h"

void URpmAvatarEditorUI::SetupAssets()
{
	ClearContainers();
	AddClearSelectionButtons();
	AddAssetButtons();
	AddColorButtons();
}

bool URpmAvatarEditorUI::IsCustomizableAssetSelected() const
{
	return bIsCustomizableAssetSelected;
}

UWrapBox* URpmAvatarEditorUI::GetColorContainerByColor(ERpmPartnerAssetColor Color) const
{
	return ColorContainerMap[Color];
}

UWrapBox* URpmAvatarEditorUI::GetAssetContainerByAsset(ERpmPartnerAssetType AssetType) const
{
	return AssetContainerMap[AssetType];
}

void URpmAvatarEditorUI::ClearContainers()
{
	for (const auto Pair : ColorContainerMap)
	{
		Pair.Value->ClearChildren();
	}
	for (const auto Pair : AssetContainerMap)
	{
		Pair.Value->ClearChildren();
	}
}

void URpmAvatarEditorUI::AddClearSelectionButtons()
{
	for (const auto Pair : AssetContainerMap)
	{
		if (Pair.Key != ERpmPartnerAssetType::EyeColor && Pair.Key != ERpmPartnerAssetType::Shirt && Pair.Key != ERpmPartnerAssetType::Outfit)
		{
			FRpmPartnerAsset Asset;
			Asset.IconTexture = ClearSelectionTexture;
			Asset.AssetType = Pair.Key;
			URpmAssetButtonUI* AssetButton = WidgetTree->ConstructWidget<URpmAssetButtonUI>(AssetButtonClass);
			AssetButton->SetSelected(IsAssetSelected(Asset));
			AssetButton->Asset = Asset;
			AssetButton->UpdateUI();
			Pair.Value->AddChildToWrapBox(AssetButton);
			AssetButton->AssetButtonSelected.AddDynamic(this, &URpmAvatarEditorUI::OnAssetButtonClicked);
		}
	}
}

void URpmAvatarEditorUI::AddAssetButtons()
{
	bIsCustomizableAssetSelected = true;
	const TArray<FRpmPartnerAsset> AllAssets = AvatarCreatorApi->GetFilteredPartnerAssets();
	for (const auto Pair : AssetContainerMap)
	{
		for (const FRpmPartnerAsset& Asset : AllAssets)
		{
			if (Asset.AssetType == Pair.Key)
			{
				auto& ButtonClass = Asset.AssetType == ERpmPartnerAssetType::EyeColor ? EyeColorButtonClass : AssetButtonClass;
				URpmAssetButtonUI* AssetButton = WidgetTree->ConstructWidget<URpmAssetButtonUI>(ButtonClass);
				const bool IsSelected = IsAssetSelected(Asset);
				AssetButton->SetSelected(IsSelected);
				AssetButton->Asset = Asset;
				AssetButton->UpdateUI();
				Pair.Value->AddChildToWrapBox(AssetButton);
				AssetButton->AssetButtonSelected.AddDynamic(this, &URpmAvatarEditorUI::OnAssetButtonClicked);
				if (IsSelected && !Asset.bIsCustomizable)
				{
					bIsCustomizableAssetSelected = false;
				}
			}
		}
	}
}

void URpmAvatarEditorUI::OnAssetButtonClicked(const FRpmPartnerAsset& Asset)
{
	AssetSelected(Asset);
	AvatarCreatorApi->UpdateAvatarAsset(Asset.AssetType, Asset.Id);
	SetAssetSelectedPin(Asset);
}

bool URpmAvatarEditorUI::IsAssetSelected(const FRpmPartnerAsset& Asset) const
{
	if (AvatarCreatorApi->AvatarProperties.Assets.Contains(Asset.AssetType))
	{
		return AvatarCreatorApi->AvatarProperties.Assets[Asset.AssetType] == Asset.Id;
	}
	return Asset.Id == 0;
}

void URpmAvatarEditorUI::SetAssetSelectedPin(const FRpmPartnerAsset& Asset)
{
	const UWrapBox* AssetContainer = GetAssetContainerByAsset(Asset.AssetType);
	for (UWidget* Widget : AssetContainer->GetAllChildren())
	{
		URpmAssetButtonUI* AssetButton = Cast<URpmAssetButtonUI>(Widget);
		const bool IsSelected = AssetButton->Asset.Id == Asset.Id && AssetButton->Asset.AssetType == Asset.AssetType;
		AssetButton->SetSelected(IsSelected);
	}
}

void URpmAvatarEditorUI::AddColorButtons()
{
	const TArray<FRpmColorPalette>& ColorPalettes = AvatarCreatorApi->GetColorPalettes();
	for (const FRpmColorPalette& ColorPalette : ColorPalettes)
	{
		UWrapBox* ColorContainer = GetColorContainerByColor(ColorPalette.AssetColor);
		for (int i = 0; i < ColorPalette.RgbColors.Num(); ++i)
		{
			const FColor RgbColor = ColorPalette.RgbColors[i];
			URpmColorButtonUI* ColorButton = WidgetTree->ConstructWidget<URpmColorButtonUI>(ColorButtonClass);
			const bool IsSelected = AvatarCreatorApi->AvatarProperties.Colors[ColorPalette.AssetColor] == i;
			ColorButton->SetSelected(IsSelected);
			ColorButton->AssetColor = ColorPalette.AssetColor;
			ColorButton->RgbColor = RgbColor;
			ColorButton->ColorIndex = i;
			ColorButton->UpdateUI();
			ColorContainer->AddChildToWrapBox(ColorButton);
			ColorButton->ColorButtonSelected.AddDynamic(this, &URpmAvatarEditorUI::OnColorButtonClicked);
		}
	}
}

void URpmAvatarEditorUI::OnColorButtonClicked(ERpmPartnerAssetColor AssetColor, int32 ColorIndex)
{
	ColorSelected();
	AvatarCreatorApi->UpdateAvatarColor(AssetColor, ColorIndex);
	SetColorSelectedPin(AssetColor, ColorIndex);
}

void URpmAvatarEditorUI::SetColorSelectedPin(ERpmPartnerAssetColor AssetColor, int32 ColorIndex)
{
	const UWrapBox* AssetContainer = GetColorContainerByColor(AssetColor);
	for (UWidget* Widget : AssetContainer->GetAllChildren())
	{
		URpmColorButtonUI* ColorButton = Cast<URpmColorButtonUI>(Widget);
		const bool IsSelected = ColorButton->AssetColor == AssetColor && ColorButton->ColorIndex == ColorIndex;
		ColorButton->SetSelected(IsSelected);
	}
}

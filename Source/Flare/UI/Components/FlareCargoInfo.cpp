
#include "../../Flare.h"
#include "FlareCargoInfo.h"
#include "../../Spacecrafts/FlareSimulatedSpacecraft.h"
#include "../../Player/FlareMenuManager.h"

#define LOCTEXT_NAMESPACE "FlareCargoInfo"


/*----------------------------------------------------
	Construct
----------------------------------------------------*/

void SFlareCargoInfo::Construct(const FArguments& InArgs)
{
	TargetSpacecraft = InArgs._Spacecraft;
	CargoIndex = InArgs._CargoIndex;
	const FFlareStyleCatalog& Theme = FFlareStyleSet::GetDefaultTheme();
	
	ChildSlot
	.HAlign(HAlign_Left)
	.VAlign(VAlign_Center)
	.Padding(FMargin(0))
	[
		SNew(SBorder)
		.Padding(FMargin(0))
		.BorderImage(this, &SFlareCargoInfo::GetResourceIcon)
		[
			SNew(SBox)
			.WidthOverride(Theme.ResourceWidth)
			.HeightOverride(Theme.ResourceHeight)
			.Padding(FMargin(0))
			[
				SNew(SVerticalBox)
			
				// Resource name
				+ SVerticalBox::Slot()
				.Padding(Theme.SmallContentPadding)
				[
					SNew(STextBlock)
					.TextStyle(&Theme.SmallFont)
					.Text(this, &SFlareCargoInfo::GetResourceAcronym)
				]

				// Resource quantity
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(Theme.SmallContentPadding)
				.VAlign(VAlign_Bottom)
				[
					SNew(STextBlock)
					.TextStyle(&Theme.SmallFont)
					.Text(this, &SFlareCargoInfo::GetResourceQuantity)
				]
			]
		]
	];
}


/*----------------------------------------------------
	Callbacks
----------------------------------------------------*/

void SFlareCargoInfo::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SWidget::OnMouseEnter(MyGeometry, MouseEvent);

	AFlareMenuManager* MenuManager = AFlareMenuManager::GetSingleton();
	FFlareCargo* Cargo = &TargetSpacecraft->GetCargoBay()[CargoIndex];
	check(Cargo);

	if (MenuManager)
	{
		FText InfoText = Cargo->Resource ? Cargo->Resource->Name : LOCTEXT("EmptyInfo", "Empty bay");
		MenuManager->ShowTooltip(this, InfoText);
	}
}

void SFlareCargoInfo::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	SWidget::OnMouseLeave(MouseEvent);

	AFlareMenuManager* MenuManager = AFlareMenuManager::GetSingleton();
	if (MenuManager)
	{
		MenuManager->HideTooltip(this);
	}
}

const FSlateBrush* SFlareCargoInfo::GetResourceIcon() const
{
	FFlareCargo* Cargo = &TargetSpacecraft->GetCargoBay()[CargoIndex];
	check(Cargo);

	if (Cargo->Resource)
	{
		return &Cargo->Resource->Icon;
	}
	else
	{
		const FFlareStyleCatalog& Theme = FFlareStyleSet::GetDefaultTheme();
		return &Theme.ResourceBackground;
	}
}

FText SFlareCargoInfo::GetResourceAcronym() const
{
	FFlareCargo* Cargo = &TargetSpacecraft->GetCargoBay()[CargoIndex];
	check(Cargo);

	if (Cargo->Resource)
	{
		return Cargo->Resource->Acronym;
	}
	else
	{
		return LOCTEXT("Empty", "Empty");
	}
}

FText SFlareCargoInfo::GetResourceQuantity() const
{
	FFlareCargo* Cargo = &TargetSpacecraft->GetCargoBay()[CargoIndex];
	check(Cargo);

	return FText::FromString(FString::Printf(TEXT("%u/%u"), Cargo->Quantity, Cargo->Capacity));
}

#undef LOCTEXT_NAMESPACE

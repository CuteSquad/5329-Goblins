// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "UserInterface", BlueprintReadWrite)
	FString NumOfGoblinsText = FString("0");

	UPROPERTY(EditAnywhere, Category = "Animations", BlueprintReadWrite)
	UWidgetAnimation* HealthPickupAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations", BlueprintReadWrite)
	UWidgetAnimation* DamageAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations", BlueprintReadWrite)
	UWidgetAnimation* GoblinsKilledTextAnim;

	UPROPERTY(EditAnywhere, Category = "Animations", BlueprintReadWrite)
	UWidgetAnimation* GoblinsKilledAnim;

	UPROPERTY(EditAnywhere, Category = "Animations", BlueprintReadWrite)
	UWidgetAnimation* CrosshairAnim;
	
protected:
	virtual void NativeConstruct() override;
};

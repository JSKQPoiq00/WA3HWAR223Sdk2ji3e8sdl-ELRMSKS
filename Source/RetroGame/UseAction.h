#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UseAction.generated.h"

class ACharacter;

UCLASS(Blueprintable)
class RETROGAME_API AUseAction : public AActor
{
	GENERATED_BODY()

public:
	AUseAction();

	// Вызывается, когда игрок “использует” предмет/действие
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UseAction")
	void OnUse(ACharacter* User);
	virtual void OnUse_Implementation(ACharacter* User);
};

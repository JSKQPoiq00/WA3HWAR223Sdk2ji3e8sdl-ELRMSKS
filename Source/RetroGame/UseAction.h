#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UseAction.generated.h"

UCLASS(Abstract, Blueprintable)
class RETROGAME_API AUseAction : public AActor
{
	GENERATED_BODY()

public:
	AUseAction();

	// BlueprintNativeEvent нужен, чтобы C++ _Implementation гарантированно работал
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UseAction")
	bool RunAction(AActor* InstigatorActor);
	virtual bool RunAction_Implementation(AActor* InstigatorActor);
};

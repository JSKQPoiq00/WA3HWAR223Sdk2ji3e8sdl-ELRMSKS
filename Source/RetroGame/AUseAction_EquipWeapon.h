#pragma once

#include "CoreMinimal.h"
#include "UseAction.h"
#include "AUseAction_EquipWeapon.generated.h"

class ABaseWeapon;

UCLASS(Abstract, Blueprintable)
class RETROGAME_API AUseAction_EquipWeapon : public AUseAction
{
	GENERATED_BODY()

public:
	AUseAction_EquipWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equip")
	FName WeaponIdToEquip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equip")
	TSubclassOf<ABaseWeapon> WeaponClassToEquip;

	// -1 = не использовать слот
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equip")
	int32 PreferredSlot;

	virtual bool RunAction_Implementation(AActor* InstigatorActor) override;
};

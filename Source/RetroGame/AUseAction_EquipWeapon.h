#pragma once

#include "CoreMinimal.h"
#include "UseAction.h"
#include "BaseWeapon.h"
#include "AUseAction_EquipWeapon.generated.h"

class UWeaponManagerComponent;

UCLASS(Blueprintable)
class RETROGAME_API AUseAction_EquipWeapon : public AUseAction
{
	GENERATED_BODY()

public:
	AUseAction_EquipWeapon();

	// Какой класс оружия экипать
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipWeapon")
	TSubclassOf<ABaseWeapon> WeaponClass;

	// В какой слот записать (0..3). -1 => не записывать
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipWeapon")
	int32 PreferredSlot = -1;

	virtual void OnUse_Implementation(ACharacter* User) override;

protected:
	UWeaponManagerComponent* FindWeaponManager(ACharacter* User) const;
};

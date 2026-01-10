#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseWeapon.h"                 // ✅ ОБЯЗАТЕЛЬНО подключаем полный тип для UFUNCTION/TSubclassOf
#include "WeaponManagerComponent.generated.h"

UCLASS(BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RETROGAME_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponManagerComponent();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(TSubclassOf<ABaseWeapon> WeaponClass);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void UnequipCurrentWeapon();

	// ✅ чтобы точно появлялось в BP
	UFUNCTION(BlueprintCallable, Category = "Weapon", meta = (DisplayName = "Get Current Weapon"))
	ABaseWeapon* GetCurrentWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool HasWeapon() const;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	ABaseWeapon* CurrentWeapon;
};

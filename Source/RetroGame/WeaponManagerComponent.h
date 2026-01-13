#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseWeapon.h"
#include "WeaponManagerComponent.generated.h"

class ACharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RETROGAME_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponManagerComponent();

	// Быстрые слоты (0..3 = клавиши 1..4)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Loadout")
	TArray<TSubclassOf<ABaseWeapon>> WeaponSlots;

	// Вытащить оружие из слота (SlotIndex: 0..3)
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool EquipSlot(int32 SlotIndex);

	// Убрать текущее оружие (аналог X/5)
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void UnequipCurrentWeapon();

	// Экипнуть конкретный класс оружия (не обязательно в слот)
	// (Это нужно чтобы BP_UseAction_Pistol мог вызывать "EquipWeapon")
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool EquipWeapon(TSubclassOf<ABaseWeapon> WeaponClass);

	// Экипнуть оружие + (опционально) записать его в слот
	// SlotIndex: 0..3 => записать и достать, SlotIndex < 0 => просто достать
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool EquipWeaponSmart(TSubclassOf<ABaseWeapon> WeaponClass, int32 SlotIndex);

	// Записать оружие в слот (без доставания)
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool SetWeaponInSlot(int32 SlotIndex, TSubclassOf<ABaseWeapon> WeaponClass);

	// Авто-назначение по типу WeaponSlotType (Pistol->0, Rifle->1, Melee->2, Grenade->3)
	// Вернёт true если реально записало в пустой слот.
	UFUNCTION(BlueprintCallable, Category = "Weapon|Loadout")
	bool TryAutoAssignWeapon(TSubclassOf<ABaseWeapon> WeaponClass);

	// Текущее оружие
	UFUNCTION(BlueprintPure, Category = "Weapon")
	ABaseWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

private:
	UPROPERTY(Transient)
	TObjectPtr<ABaseWeapon> CurrentWeapon = nullptr;

	int32 CurrentSlotIndex = -1;

	ACharacter* GetOwnerCharacter() const;

	void DestroyCurrentWeapon();
	bool SpawnAndAttachWeapon(TSubclassOf<ABaseWeapon> WeaponClass);
};

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponManagerComponent.generated.h"

class ABaseWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponEquipped, ABaseWeapon*, NewWeapon, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponUnequipped);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RETROGAME_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponManagerComponent();

	// Слоты: индекс -> класс оружия
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Slots")
	TArray<TSubclassOf<ABaseWeapon>> WeaponSlots;

	// Реестр по ID (опционально)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Registry")
	TMap<FName, TSubclassOf<ABaseWeapon>> WeaponsById;

	// Если NAME_None — берём AttachSocketName из самого оружия
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sockets")
	FName WeaponAttachSocket;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<ABaseWeapon> CurrentWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Slots")
	int32 CurrentSlotIndex;

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
	FOnWeaponEquipped OnWeaponEquipped;

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
	FOnWeaponUnequipped OnWeaponUnequipped;

	UFUNCTION(BlueprintCallable, Category = "Weapon|Slots")
	bool AddWeaponToSlot(TSubclassOf<ABaseWeapon> WeaponClass, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Weapon|Slots")
	ABaseWeapon* EquipSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void UnequipCurrentWeapon(bool bDestroyWeapon = true);

	// ВАЖНО: default -1, потому что UHT не любит INDEX_NONE в default param
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	ABaseWeapon* EquipWeapon(TSubclassOf<ABaseWeapon> WeaponClass, int32 SlotIndex = -1);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	ABaseWeapon* EquipWeaponById(FName WeaponId, int32 SlotIndex = -1);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	ABaseWeapon* EquipWeaponSmart(int32 SlotIndex, FName WeaponId = NAME_None, TSubclassOf<ABaseWeapon> WeaponClass = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	ABaseWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

protected:
	virtual void BeginPlay() override;

private:
	USceneComponent* ResolveAttachParent() const;
	void AttachWeaponToOwner(ABaseWeapon* Weapon) const;
};

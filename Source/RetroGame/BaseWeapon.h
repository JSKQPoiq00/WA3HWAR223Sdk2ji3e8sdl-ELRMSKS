#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UENUM(BlueprintType)
enum class ERetroWeaponSlot : uint8
{
	Pistol  UMETA(DisplayName = "Slot 1 - Pistol"),
	Rifle   UMETA(DisplayName = "Slot 2 - Rifle/Shotgun"),
	Melee   UMETA(DisplayName = "Slot 3 - Melee"),
	Grenade UMETA(DisplayName = "Slot 4 - Grenade"),
	None    UMETA(DisplayName = "Not Assignable")
};

UCLASS(Blueprintable)
class RETROGAME_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABaseWeapon();

	// Меш оружия (в BP можно заменить на свой)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> Mesh;

	// К какому “быстрому” слоту относится оружие (для авто-назначения)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Loadout")
	ERetroWeaponSlot WeaponSlotType = ERetroWeaponSlot::None;

	// В какой сокет цеплять на персонажа
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attach")
	FName AttachSocketName = TEXT("hand_rSocket");
};

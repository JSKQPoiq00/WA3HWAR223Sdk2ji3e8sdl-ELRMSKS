#include "WeaponManagerComponent.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"

UWeaponManagerComponent::UWeaponManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// 4 слота (1..4). Можно расширить позже.
	WeaponSlots.SetNum(4);
}

ACharacter* UWeaponManagerComponent::GetOwnerCharacter() const
{
	return Cast<ACharacter>(GetOwner());
}

void UWeaponManagerComponent::DestroyCurrentWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}
	CurrentSlotIndex = -1;
}

bool UWeaponManagerComponent::SpawnAndAttachWeapon(TSubclassOf<ABaseWeapon> WeaponClass)
{
	if (!WeaponClass)
		return false;

	ACharacter* OwnerChar = GetOwnerCharacter();
	if (!OwnerChar)
		return false;

	UWorld* World = GetWorld();
	if (!World)
		return false;

	// убираем старое
	DestroyCurrentWeapon();

	FActorSpawnParameters Params;
	Params.Owner = OwnerChar;
	Params.Instigator = OwnerChar;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABaseWeapon* NewWeapon = World->SpawnActor<ABaseWeapon>(WeaponClass, FTransform::Identity, Params);
	if (!NewWeapon)
		return false;

	USkeletalMeshComponent* CharMesh = OwnerChar->GetMesh();
	if (!CharMesh)
	{
		NewWeapon->Destroy();
		return false;
	}

	const FName SocketName = NewWeapon->AttachSocketName;
	NewWeapon->AttachToComponent(CharMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);

	CurrentWeapon = NewWeapon;
	return true;
}

bool UWeaponManagerComponent::EquipWeapon(TSubclassOf<ABaseWeapon> WeaponClass)
{
	// Просто достаём, не трогаем слоты
	return SpawnAndAttachWeapon(WeaponClass);
}

bool UWeaponManagerComponent::SetWeaponInSlot(int32 SlotIndex, TSubclassOf<ABaseWeapon> WeaponClass)
{
	if (SlotIndex < 0 || SlotIndex >= 4)
		return false;

	// гарантируем размер
	if (WeaponSlots.Num() < 4)
		WeaponSlots.SetNum(4);

	WeaponSlots[SlotIndex] = WeaponClass;
	return true;
}

bool UWeaponManagerComponent::EquipWeaponSmart(TSubclassOf<ABaseWeapon> WeaponClass, int32 SlotIndex)
{
	if (!WeaponClass)
		return false;

	// Если слот валидный — записываем в слот и достаём из него
	if (SlotIndex >= 0 && SlotIndex < 4)
	{
		SetWeaponInSlot(SlotIndex, WeaponClass);
		return EquipSlot(SlotIndex);
	}

	// Иначе просто достаём оружие
	return EquipWeapon(WeaponClass);
}

bool UWeaponManagerComponent::EquipSlot(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= 4)
		return false;

	if (WeaponSlots.Num() < 4)
		WeaponSlots.SetNum(4);

	TSubclassOf<ABaseWeapon> WeaponClass = WeaponSlots[SlotIndex];
	if (!WeaponClass)
	{
		// слот пустой
		return false;
	}

	const bool bOk = SpawnAndAttachWeapon(WeaponClass);
	if (bOk)
	{
		CurrentSlotIndex = SlotIndex;
	}
	return bOk;
}

void UWeaponManagerComponent::UnequipCurrentWeapon()
{
	DestroyCurrentWeapon();
}

bool UWeaponManagerComponent::TryAutoAssignWeapon(TSubclassOf<ABaseWeapon> WeaponClass)
{
	if (!WeaponClass)
		return false;

	const ABaseWeapon* WeaponCDO = GetDefault<ABaseWeapon>(WeaponClass);
	if (!WeaponCDO)
		return false;

	int32 TargetIndex = -1;
	switch (WeaponCDO->WeaponSlotType)
	{
	case ERetroWeaponSlot::Pistol:  TargetIndex = 0; break;
	case ERetroWeaponSlot::Rifle:   TargetIndex = 1; break;
	case ERetroWeaponSlot::Melee:   TargetIndex = 2; break;
	case ERetroWeaponSlot::Grenade: TargetIndex = 3; break;
	default: return false;
	}

	if (WeaponSlots.Num() < 4)
		WeaponSlots.SetNum(4);

	// Записываем только если пусто
	if (!WeaponSlots[TargetIndex])
	{
		WeaponSlots[TargetIndex] = WeaponClass;
		return true;
	}

	return false;
}

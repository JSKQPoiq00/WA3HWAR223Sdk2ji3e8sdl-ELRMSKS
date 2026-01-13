#include "WeaponManagerComponent.h"

#include "BaseWeapon.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"

UWeaponManagerComponent::UWeaponManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentWeapon = nullptr;
	CurrentSlotIndex = INDEX_NONE;

	// Дефолт — твой сокет на Mesh персонажа
	WeaponAttachSocket = TEXT("WeaponSocket");
}

void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UWeaponManagerComponent::AddWeaponToSlot(TSubclassOf<ABaseWeapon> WeaponClass, int32 SlotIndex)
{
	if (!WeaponClass || SlotIndex < 0)
	{
		return false;
	}

	if (WeaponSlots.Num() <= SlotIndex)
	{
		WeaponSlots.SetNum(SlotIndex + 1);
	}

	WeaponSlots[SlotIndex] = WeaponClass;
	return true;
}

ABaseWeapon* UWeaponManagerComponent::EquipSlot(int32 SlotIndex)
{
	if (!WeaponSlots.IsValidIndex(SlotIndex) || !WeaponSlots[SlotIndex])
	{
		return nullptr;
	}

	return EquipWeapon(WeaponSlots[SlotIndex], SlotIndex);
}

void UWeaponManagerComponent::UnequipCurrentWeapon(bool bDestroyWeapon)
{
	if (!CurrentWeapon)
	{
		return;
	}

	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if (bDestroyWeapon)
	{
		CurrentWeapon->Destroy();
	}

	CurrentWeapon = nullptr;
	CurrentSlotIndex = INDEX_NONE;

	OnWeaponUnequipped.Broadcast();
}

ABaseWeapon* UWeaponManagerComponent::EquipWeapon(TSubclassOf<ABaseWeapon> WeaponClass, int32 SlotIndex)
{
	if (!WeaponClass)
	{
		return nullptr;
	}

	// Если указан слот — запоминаем класс в слоте
	if (SlotIndex != INDEX_NONE)
	{
		if (!AddWeaponToSlot(WeaponClass, SlotIndex))
		{
			return nullptr;
		}
	}

	// Снимаем старое
	UnequipCurrentWeapon(true);

	UWorld* World = GetWorld();
	AActor* OwnerActor = GetOwner();
	if (!World || !OwnerActor)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = OwnerActor->GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABaseWeapon* SpawnedWeapon = World->SpawnActor<ABaseWeapon>(WeaponClass, FTransform::Identity, SpawnParams);
	if (!SpawnedWeapon)
	{
		return nullptr;
	}

	AttachWeaponToOwner(SpawnedWeapon);

	CurrentWeapon = SpawnedWeapon;
	CurrentSlotIndex = (SlotIndex != INDEX_NONE) ? SlotIndex : INDEX_NONE;

	OnWeaponEquipped.Broadcast(CurrentWeapon, CurrentSlotIndex);
	return CurrentWeapon;
}

ABaseWeapon* UWeaponManagerComponent::EquipWeaponById(FName WeaponId, int32 SlotIndex)
{
	if (WeaponId == NAME_None)
	{
		return nullptr;
	}

	if (const TSubclassOf<ABaseWeapon>* Found = WeaponsById.Find(WeaponId))
	{
		if (*Found)
		{
			return EquipWeapon(*Found, SlotIndex);
		}
	}

	return nullptr;
}

ABaseWeapon* UWeaponManagerComponent::EquipWeaponSmart(int32 SlotIndex, FName WeaponId, TSubclassOf<ABaseWeapon> WeaponClass)
{
	if (WeaponClass)
	{
		return EquipWeapon(WeaponClass, SlotIndex);
	}

	if (WeaponId != NAME_None)
	{
		return EquipWeaponById(WeaponId, SlotIndex);
	}

	return nullptr;
}

USceneComponent* UWeaponManagerComponent::ResolveAttachParent() const
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return nullptr;
	}

	// ✅ САМОЕ ВАЖНОЕ: у Character сокеты на Mesh, а не на Capsule
	if (ACharacter* Char = Cast<ACharacter>(OwnerActor))
	{
		if (USkeletalMeshComponent* Mesh = Char->GetMesh())
		{
			return Mesh;
		}
	}

	// запасной вариант — любой SkeletalMeshComponent
	if (USkeletalMeshComponent* AnyMesh = OwnerActor->FindComponentByClass<USkeletalMeshComponent>())
	{
		return AnyMesh;
	}

	return OwnerActor->GetRootComponent();
}

void UWeaponManagerComponent::AttachWeaponToOwner(ABaseWeapon* Weapon) const
{
	if (!Weapon)
	{
		return;
	}

	USceneComponent* AttachParent = ResolveAttachParent();
	if (!AttachParent)
	{
		return;
	}

	const FName SocketToUse = (WeaponAttachSocket != NAME_None) ? WeaponAttachSocket : Weapon->AttachSocketName;

	Weapon->AttachToComponent(
		AttachParent,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		SocketToUse
	);
}

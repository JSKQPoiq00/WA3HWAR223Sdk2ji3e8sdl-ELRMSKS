#include "WeaponManagerComponent.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"

UWeaponManagerComponent::UWeaponManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentWeapon = nullptr;
}

void UWeaponManagerComponent::EquipWeapon(TSubclassOf<ABaseWeapon> WeaponClass)
{
	if (!WeaponClass) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (!Mesh) return;

	UnequipCurrentWeapon();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.Instigator = OwnerCharacter;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABaseWeapon* NewWeapon = GetWorld()->SpawnActor<ABaseWeapon>(
		WeaponClass,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (!IsValid(NewWeapon)) return;

	const FName SocketName = NewWeapon->AttachSocketName;

	if (Mesh->DoesSocketExist(SocketName))
	{
		NewWeapon->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[WeaponManager] Socket '%s' not found. Weapon will attach wrong (likely near feet)."), *SocketName.ToString());
		NewWeapon->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	CurrentWeapon = NewWeapon;
}

void UWeaponManagerComponent::UnequipCurrentWeapon()
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}
}

ABaseWeapon* UWeaponManagerComponent::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

bool UWeaponManagerComponent::HasWeapon() const
{
	return IsValid(CurrentWeapon);
}

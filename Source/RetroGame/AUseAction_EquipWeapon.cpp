#include "AUseAction_EquipWeapon.h"

#include "WeaponManagerComponent.h"
#include "GameFramework/Character.h"

AUseAction_EquipWeapon::AUseAction_EquipWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

UWeaponManagerComponent* AUseAction_EquipWeapon::FindWeaponManager(ACharacter* User) const
{
	if (!User) return nullptr;
	return User->FindComponentByClass<UWeaponManagerComponent>();
}

void AUseAction_EquipWeapon::OnUse_Implementation(ACharacter* User)
{
	UWeaponManagerComponent* WM = FindWeaponManager(User);
	if (!WM) return;

	// ВАЖНО: именно EquipWeaponSmart (чтобы и в слот записать, если нужно)
	WM->EquipWeaponSmart(WeaponClass, PreferredSlot);
}

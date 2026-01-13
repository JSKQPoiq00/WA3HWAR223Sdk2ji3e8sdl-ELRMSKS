#include "AUseAction_EquipWeapon.h"

#include "WeaponManagerComponent.h"
#include "BaseWeapon.h"
#include "GameFramework/Controller.h"

AUseAction_EquipWeapon::AUseAction_EquipWeapon()
{
	WeaponIdToEquip = NAME_None;
	WeaponClassToEquip = nullptr;
	PreferredSlot = INDEX_NONE;

	SetActorHiddenInGame(true);
	SetCanBeDamaged(false);
}

bool AUseAction_EquipWeapon::RunAction_Implementation(AActor* InstigatorActor)
{
	AActor* ContextActor = InstigatorActor;

	// »ногда инвентарь передаЄт Controller вместо Pawn
	if (AController* Controller = Cast<AController>(ContextActor))
	{
		ContextActor = Controller->GetPawn();
	}

	// на вс€кий случай Ч owner UseAction
	if (!ContextActor)
	{
		ContextActor = GetOwner();
	}

	if (!ContextActor)
	{
		return false;
	}

	UWeaponManagerComponent* WeaponManager = ContextActor->FindComponentByClass<UWeaponManagerComponent>();
	if (!WeaponManager)
	{
		return false;
	}

	ABaseWeapon* Equipped = WeaponManager->EquipWeaponSmart(PreferredSlot, WeaponIdToEquip, WeaponClassToEquip);
	return (Equipped != nullptr);
}

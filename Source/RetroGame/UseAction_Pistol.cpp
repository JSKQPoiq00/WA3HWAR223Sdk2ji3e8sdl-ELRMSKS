#include "UseAction_Pistol.h"

#include "BaseWeapon.h"
#include "UObject/ConstructorHelpers.h"

AUseAction_Pistol::AUseAction_Pistol()
{
	WeaponIdToEquip = TEXT("Pistol");
	PreferredSlot = 0;

	static ConstructorHelpers::FClassFinder<ABaseWeapon> PistolBP(TEXT("/Game/MY_RETRO_GAME/Weapons/BP_Pistol"));
	if (PistolBP.Succeeded())
	{
		WeaponClassToEquip = PistolBP.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AUseAction_Pistol: BP_Pistol not found. Check asset path /Game/MY_RETRO_GAME/Weapons/BP_Pistol"));
	}
}

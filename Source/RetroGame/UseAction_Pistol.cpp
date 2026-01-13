#include "UseAction_Pistol.h"

#include "UObject/ConstructorHelpers.h"
#include "BaseWeapon.h"

AUseAction_Pistol::AUseAction_Pistol()
{
	PreferredSlot = 0; // слот 1 (клавиша 1)

	// ѕуть как в Content Browser: /Game/...
	static ConstructorHelpers::FClassFinder<ABaseWeapon> PistolBPClass(TEXT("/Game/MY_RETRO_GAME/Weapons/BP_Pistol"));
	if (PistolBPClass.Succeeded())
	{
		WeaponClass = PistolBPClass.Class;
	}
}

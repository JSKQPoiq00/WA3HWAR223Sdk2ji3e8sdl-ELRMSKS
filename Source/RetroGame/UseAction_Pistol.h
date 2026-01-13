#pragma once

#include "CoreMinimal.h"
#include "AUseAction_EquipWeapon.h"
#include "UseAction_Pistol.generated.h"

UCLASS(Blueprintable)
class RETROGAME_API AUseAction_Pistol : public AUseAction_EquipWeapon
{
	GENERATED_BODY()

public:
	AUseAction_Pistol();
};

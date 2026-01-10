#include "BaseWeapon.h"
#include "Components/StaticMeshComponent.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	WeaponMesh->SetMobility(EComponentMobility::Movable);
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	WeaponMesh->SetSimulatePhysics(false);

	AttachSocketName = "hand_r_socket";
}

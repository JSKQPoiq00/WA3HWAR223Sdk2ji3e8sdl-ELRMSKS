#include "UseAction.h"

AUseAction::AUseAction()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorHiddenInGame(true);
	SetCanBeDamaged(false);
}

bool AUseAction::RunAction_Implementation(AActor* InstigatorActor)
{
	return false;
}

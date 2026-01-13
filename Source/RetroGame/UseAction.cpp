#include "UseAction.h"

AUseAction::AUseAction()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUseAction::OnUse_Implementation(ACharacter* User)
{
	// База ничего не делает
}

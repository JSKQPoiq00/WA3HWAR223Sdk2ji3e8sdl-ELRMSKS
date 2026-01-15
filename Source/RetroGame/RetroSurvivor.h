#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RetroSurvivor.generated.h"

UCLASS()
class RETROGAME_API ARetroSurvivor : public ACharacter
{
    GENERATED_BODY()

public:
    ARetroSurvivor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // --- КАМЕРА ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    class UCameraComponent* FollowCamera;

    // --- БОЕВЫЕ ПЕРЕМЕННЫЕ ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
    bool bIsAiming;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
    bool bHasWeapon;

    // Ссылка на Монтаж (сюда положишь AM_PistolAim)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
    class UAnimMontage* AimMontage;

    // --- ЭФФЕКТЫ ---
    UPROPERTY(EditAnywhere, Category = Combat)
    class UParticleSystem* MuzzleFlash;

    UPROPERTY(EditAnywhere, Category = Combat)
    class USoundBase* FireSound;

    UPROPERTY(EditAnywhere, Category = Combat)
    class UParticleSystem* ImpactEffect;

    // Тряска камеры
    UPROPERTY(EditAnywhere, Category = Combat)
    TSubclassOf<class UCameraShakeBase> FireCamShake;

    // --- ФУНКЦИИ (Теперь доступны в Блюпринте!) ---

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StartAiming();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StopAiming();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void FireWeapon();
};
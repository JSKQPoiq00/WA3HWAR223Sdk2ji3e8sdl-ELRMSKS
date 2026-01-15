#include "RetroSurvivor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
// Важно для тряски камеры
#include "Camera/CameraShakeBase.h"

ARetroSurvivor::ARetroSurvivor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Настройка камеры
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    bIsAiming = false;
    bHasWeapon = true;
}

void ARetroSurvivor::BeginPlay()
{
    Super::BeginPlay();
}

void ARetroSurvivor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Плавный зум камеры (FOV) при прицеливании
    float TargetFOV = bIsAiming ? 60.0f : 90.0f;
    float CurrentFOV = FMath::FInterpTo(FollowCamera->FieldOfView, TargetFOV, DeltaTime, 10.0f);
    FollowCamera->SetFieldOfView(CurrentFOV);
}

void ARetroSurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Привязка кнопок (для C++). 
    // Даже если мы вызовем их из Блюпринта, этот код мешать не будет.
    PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ARetroSurvivor::StartAiming);
    PlayerInputComponent->BindAction("Aim", IE_Released, this, &ARetroSurvivor::StopAiming);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ARetroSurvivor::FireWeapon);
}

void ARetroSurvivor::StartAiming()
{
    bIsAiming = true;

    // Запускаем Монтаж (поднимаем руки)
    if (AimMontage)
    {
        PlayAnimMontage(AimMontage);
    }

    // Замедляем скорость бега
    GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void ARetroSurvivor::StopAiming()
{
    bIsAiming = false;

    // Останавливаем Монтаж (опускаем руки)
    if (AimMontage)
    {
        StopAnimMontage(AimMontage);
    }

    // Возвращаем скорость
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void ARetroSurvivor::FireWeapon()
{
    // Стреляем, только если целимся или если разрешена стрельба от бедра
    if (!bIsAiming && !bHasWeapon) return;

    // 1. ЗВУК
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }

    // Получаем координаты выстрела (от камеры)
    FVector StartLocation;
    FRotator CameraRotation;
    GetController()->GetPlayerViewPoint(StartLocation, CameraRotation);

    // Разброс
    float Spread = 1.0f;
    FRotator ShotRotation = CameraRotation;
    ShotRotation.Pitch += FMath::RandRange(-Spread, Spread);
    ShotRotation.Yaw += FMath::RandRange(-Spread, Spread);

    FVector EndLocation = StartLocation + (ShotRotation.Vector() * 10000.0f);

    // 2. RAYCAST (Луч)
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // Не попадать в себя

    if (GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, Params))
    {
        // Попали! Рисуем эффект попадания
        if (ImpactEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, Hit.ImpactNormal.Rotation());
        }
    }

    // 3. ОТДАЧА
    AddControllerPitchInput(-0.5f);

    // 4. ТРЯСКА
    if (FireCamShake)
    {
        GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(FireCamShake);
    }
}
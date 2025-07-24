// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "FPSProjectile.h"
#include "FPSCharacter.generated.h"

UCLASS()
class FPSPROJECT_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USkeletalMeshComponent* MyMesh;

	// 스폰할 발사체 클래스입니다.
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AFPSProjectile> ProjectileClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 앞으로 이동 및 뒤로 이동 입력을 처리합니다.
	UFUNCTION()
	void MoveForward(float Value);

	// 오른쪽 이동 및 왼쪽 이동 입력을 처리합니다.
	UFUNCTION()
	void MoveRight(float Value);

	// 키가 눌릴 경우 플래그를 설정합니다.
	UFUNCTION()
	void StartJump();

	// 키가 떼어질 경우 점프 플래그를 지웁니다.
	UFUNCTION()
	void StopJump();

	// FPS 카메라입니다.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	// 일인칭 메시(팔)로, 소유 플레이어에게만 보입니다.
	UPROPERTY(EditAnywhere, Category = "Mesh")
	USkeletalMeshComponent* FPSMesh;

	// 발사체 발사를 처리하는 함수입니다.
	UFUNCTION()
	void Fire();

	// 카메라 위치로부터의 총구 오프셋입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset;

};

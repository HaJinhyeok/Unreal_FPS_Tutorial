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

	// ������ �߻�ü Ŭ�����Դϴ�.
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AFPSProjectile> ProjectileClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ������ �̵� �� �ڷ� �̵� �Է��� ó���մϴ�.
	UFUNCTION()
	void MoveForward(float Value);

	// ������ �̵� �� ���� �̵� �Է��� ó���մϴ�.
	UFUNCTION()
	void MoveRight(float Value);

	// Ű�� ���� ��� �÷��׸� �����մϴ�.
	UFUNCTION()
	void StartJump();

	// Ű�� ������ ��� ���� �÷��׸� ����ϴ�.
	UFUNCTION()
	void StopJump();

	// FPS ī�޶��Դϴ�.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	// ����Ī �޽�(��)��, ���� �÷��̾�Ը� ���Դϴ�.
	UPROPERTY(EditAnywhere, Category = "Mesh")
	USkeletalMeshComponent* FPSMesh;

	// �߻�ü �߻縦 ó���ϴ� �Լ��Դϴ�.
	UFUNCTION()
	void Fire();

	// ī�޶� ��ġ�κ����� �ѱ� �������Դϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset;

};

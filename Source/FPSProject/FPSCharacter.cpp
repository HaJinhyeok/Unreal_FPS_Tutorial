// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ����Ī ī�޶� ������Ʈ�� �����մϴ�.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);

	// ĸ�� ������Ʈ�� ī�޶� ������Ʈ�� ����ġ�մϴ�.
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// ī�޶� �� �ణ ���� ��ġ�ϵ��� �մϴ�.
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	// ���� ī�޶� ȸ���� �����ϵ��� �մϴ�.
	FPSCameraComponent->bUsePawnControlRotation = true;

	// ���� �÷��̾��� ����Ī �޽� ������Ʈ�� �����մϴ�.
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);

	// ���� �÷��̾ �� �޽ø� �� �� �ֽ��ϴ�.
	FPSMesh->SetOnlyOwnerSee(true);

	// FPS �޽ø� FPS ī�޶� ����ġ�մϴ�.
	FPSMesh->SetupAttachment(FPSCameraComponent);

	// �Ϻ� �ι��̷���Ʈ ������ ��Ȱ��ȭ�Ͽ� ���� �޽� ���� ������ �����մϴ�.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	// ���� �÷��̾ �Ϲ�(����Ī) �ٵ� �޽ø� �� �� �����ϴ�.
	GetMesh()->SetOwnerNoSee(true);
	/*MyMesh = GetMesh();
	MyMesh->SetOwnerNoSee(true);
	MyMesh->bCastDynamicShadow = false;
	MyMesh->CastShadow = false;*/
	//UE_LOG(LogTemp, Display, TEXT("MyMesh: %s"), MyMesh->SkeletalMesh->GetImportedModel());
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(GEngine != nullptr);

	// ����� �޽����� 5�ʰ� ǥ���մϴ�. 
	// -1 'Ű' �� �������ڰ� �޽����� ������Ʈ�ǰų� ���ΰ�ħ���� �ʵ��� �����մϴ�.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));

	
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 'movement' ���ε��� �����մϴ�.
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	// 'look' ���ε��� �����մϴ�.
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	// 'action' ���ε��� �����մϴ�.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);

}

void AFPSCharacter::MoveForward(float Value)
{
	// ��� '��'���� ã��, �÷��̾ �ش� �������� �̵��ϰ��� �Ѵٴ� ���� ����մϴ�.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AFPSCharacter::MoveRight(float Value)
{
	// ��� '������'���� ã��, �÷��̾ �ش� �������� �̵��ϰ��� �Ѵٴ� ���� ����մϴ�.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::StopJump()
{
	bPressedJump = false;
}

void AFPSCharacter::Fire()
{
	// ī�޶� Ʈ�������� ���մϴ�.
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	// MuzzleOffset�� ī�޶� ��¦ �տ��� �߻�ü�� �����ϵ��� ����
	MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

	// MuzzleOffset�� ī�޶� �����̽����� ���� �����̽��� ��ȯ�մϴ�.
	FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	// ������ ��¦ ���� ���ϵ��� �ְ��մϴ�.
	FRotator MuzzleRotation = CameraRotation;
	//MuzzleRotation.Pitch += 10.0f;

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// �ѱ��� �߻�ü�� �����մϴ�.
		AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (Projectile)
		{
			// �߻�ü�� �ʱ� ź���� �����մϴ�.
			FVector LaunchDirection = MuzzleRotation.Vector();
			Projectile->FireInDirection(LaunchDirection);
		}
	}
}


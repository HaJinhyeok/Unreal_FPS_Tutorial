// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 일인칭 카메라 컴포넌트를 생성합니다.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);

	// 캡슐 컴포넌트에 카메라 컴포넌트를 어태치합니다.
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// 카메라가 눈 약간 위에 위치하도록 합니다.
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	// 폰이 카메라 회전을 제어하도록 합니다.
	FPSCameraComponent->bUsePawnControlRotation = true;

	// 소유 플레이어의 일인칭 메시 컴포넌트를 생성합니다.
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);

	// 소유 플레이어만 이 메시를 볼 수 있습니다.
	FPSMesh->SetOnlyOwnerSee(true);

	// FPS 메시를 FPS 카메라에 어태치합니다.
	FPSMesh->SetupAttachment(FPSCameraComponent);

	// 일부 인바이런먼트 섀도를 비활성화하여 단일 메시 같은 느낌을 보존합니다.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	// 소유 플레이어가 일반(삼인칭) 바디 메시를 볼 수 없습니다.
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

	// 디버그 메시지를 5초간 표시합니다. 
	// -1 '키' 값 실행인자가 메시지가 업데이트되거나 새로고침되지 않도록 방지합니다.
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

	// 'movement' 바인딩을 구성합니다.
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	// 'look' 바인딩을 구성합니다.
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	// 'action' 바인딩을 구성합니다.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);

}

void AFPSCharacter::MoveForward(float Value)
{
	// 어디가 '앞'인지 찾고, 플레이어가 해당 방향으로 이동하고자 한다는 것을 기록합니다.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AFPSCharacter::MoveRight(float Value)
{
	// 어디가 '오른쪽'인지 찾고, 플레이어가 해당 방향으로 이동하고자 한다는 것을 기록합니다.
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
	// 카메라 트랜스폼을 구합니다.
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	// MuzzleOffset이 카메라 살짝 앞에서 발사체를 스폰하도록 설정
	MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

	// MuzzleOffset을 카메라 스페이스에서 월드 스페이스로 변환합니다.
	FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	// 조준이 살짝 위를 향하도록 왜곡합니다.
	FRotator MuzzleRotation = CameraRotation;
	//MuzzleRotation.Pitch += 10.0f;

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// 총구에 발사체를 스폰합니다.
		AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (Projectile)
		{
			// 발사체의 초기 탄도를 설정합니다.
			FVector LaunchDirection = MuzzleRotation.Vector();
			Projectile->FireInDirection(LaunchDirection);
		}
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSProject/Public/TPSPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//TPS 카메라를 SpringArm에 부착
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	springArmComp->SetupAttachment(RootComponent);//루트에 자식
	springArmComp->SetRelativeLocation(FVector(0.0f, 70.0f, 90.0f));//시작 위치 변경
	springArmComp->TargetArmLength = 400.0f; //암 길이
	
	//카메라 컴포넌트
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	cameraComp->SetupAttachment(springArmComp);
	
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//Enhanced Input 시스템이 IMC_TPS 사용하도록 설정
	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(imc_TPS, 0);
		}
	}
	
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	/* GetContolRotation() 함수는 좌우(Yaw) 말고도 상하(Pitch)까지 포함된 카메라 전체 회전
	Pitch 움직임에 X축을 기울이는 성질이 있어서 카메라가 아래를 향하는 상태에서 w입력시
	"앞으로 가는 입력" + "아래로 박는 입력"으로 섞임 -> 수평속도가 cos(Pitch)로 줄어듬 */
	
	
	//컨트롤러의 현재 회전 값들 (Yaw, Pitch, Roll)값을 가져옴
	FRotator controlRot = GetControlRotation();
	//Pitch자체를 0으로 설정 -> 기우는 현상 차단 -> 일정속도 이동
	controlRot.Pitch = 0.0f;
	controlRot.Roll = 0.0f;
	//원래도 0이긴함 -> 카메라 움직임에 따라에도 움직일 가능성 존재하여 사전 차단
	
	//Yaw 좌우 측값만 남은 벡터에 입력 -> 카메라의 회전과 관계엾이 수평면 위에서만 이동
	direction = FRotationMatrix(controlRot).TransformVector(direction);
	
	//플레이어 이동 처리
	//P(결과위치) = P0(초기위치) + v(속도) * t(시간)
	// FVector P0 = GetActorLocation();
	// FVector vt = direction * walkSpeed * DeltaTime;
	// FVector P = P0 + vt;
	// SetActorLocation(P);
	
	//direction = FTransform(GetControlRotation()).TransformFVector4(direction);
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput)
	{
		PlayerInput -> BindAction(ia_Move, ETriggerEvent::Triggered, this, &ATPSPlayer::Move);
		PlayerInput -> BindAction(ia_LookUP, ETriggerEvent::Triggered, this, &ATPSPlayer::LookUP);
		PlayerInput -> BindAction(ia_Turn, ETriggerEvent::Triggered, this, &ATPSPlayer::Turn);
		PlayerInput -> BindAction(ia_Jump, ETriggerEvent::Started, this, &ATPSPlayer::InputJump);
	}
}

void ATPSPlayer::Move(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	direction.X = value.X;//전후
	direction.Y = value.Y;//좌우
}

void ATPSPlayer::LookUP(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerPitchInput(value);// Y축 회전
}

void ATPSPlayer::Turn(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerYawInput(value);// Z축 회전
}

void ATPSPlayer::InputJump(const struct FInputActionValue& inputValue)
{
	//ACharater 내부의 기본 제공 점프 함수
	Jump();
}


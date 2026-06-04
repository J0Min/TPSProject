// // Fill out your copyright notice in the Description page of Project Settings.
//
//
// #include "TPSProject/Public/TPSPlayer.h"
//
// #include "EnhancedInputComponent.h"
// #include "EnhancedInputSubsystems.h"
// #include "Camera/CameraComponent.h"
// #include "GameFramework/SpringArmComponent.h"
//
//
// // Sets default values
// ATPSPlayer::ATPSPlayer()
// {
// 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
// 	PrimaryActorTick.bCanEverTick = true;
// 	
// 	//TPS 카메라를 SpringArm에 부착
// 	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
// 	springArmComp->SetupAttachment(RootComponent);//루트에 자식
// 	springArmComp->SetRelativeLocation(FVector(0.0f, 70.0f, 90.0f));//시작 위치 변경
// 	springArmComp->TargetArmLength = 400.0f; //암 길이
// 	
// 	//카메라 컴포넌트
// 	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
// 	cameraComp->SetupAttachment(springArmComp);
// 	
// }
//
// // Called when the game starts or when spawned
// void ATPSPlayer::BeginPlay()
// {
// 	Super::BeginPlay();
// 	
// 	//Enhanced Input 시스템이 IMC_TPS 사용하도록 설정
// 	auto pc = Cast<APlayerController>(Controller);
// 	if (pc)
// 	{
// 		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
// 		if (subsystem)
// 		{
// 			subsystem->AddMappingContext(imc_TPS, 0);
// 		}
// 	}
// 	
// }
//
// // Called every frame
// void ATPSPlayer::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// 	
// 	
// 	/* GetContolRotation() 함수는 좌우(Yaw) 말고도 상하(Pitch)까지 포함된 카메라 전체 회전
// 	Pitch 움직임에 X축을 기울이는 성질이 있어서 카메라가 아래를 향하는 상태에서 w입력시
// 	"앞으로 가는 입력" + "아래로 박는 입력"으로 섞임 -> 수평속도가 cos(Pitch)로 줄어듬 */
// 	
// 	
// 	//컨트롤러의 현재 회전 값들 (Yaw, Pitch, Roll)값을 가져옴
// 	FRotator controlRot = GetControlRotation();
// 	//Pitch자체를 0으로 설정 -> 기우는 현상 차단 -> 일정속도 이동
// 	controlRot.Pitch = 0.0f;
// 	controlRot.Roll = 0.0f;
// 	//원래도 0이긴함 -> 카메라 움직임에 따라에도 움직일 가능성 존재하여 사전 차단
// 	
// 	//Yaw 좌우 측값만 남은 벡터에 입력 -> 카메라의 회전과 관계엾이 수평면 위에서만 이동
// 	direction = FRotationMatrix(controlRot).TransformVector(direction);
// 	
// 	//플레이어 이동 처리
// 	//P(결과위치) = P0(초기위치) + v(속도) * t(시간)
// 	// FVector P0 = GetActorLocation();
// 	// FVector vt = direction * walkSpeed * DeltaTime;
// 	// FVector P = P0 + vt;
// 	// SetActorLocation(P);
// 	
// 	//direction = FTransform(GetControlRotation()).TransformFVector4(direction);
// 	AddMovementInput(direction);
// 	direction = FVector::ZeroVector;
// }
//
// // Called to bind functionality to input
// void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);
// 	
// 	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
// 	if (PlayerInput)
// 	{
// 		PlayerInput -> BindAction(ia_Move, ETriggerEvent::Triggered, this, &ATPSPlayer::Move);
// 		PlayerInput -> BindAction(ia_LookUP, ETriggerEvent::Triggered, this, &ATPSPlayer::LookUP);
// 		PlayerInput -> BindAction(ia_Turn, ETriggerEvent::Triggered, this, &ATPSPlayer::Turn);
// 		PlayerInput -> BindAction(ia_Jump, ETriggerEvent::Started, this, &ATPSPlayer::InputJump);
// 	}
// }
//
// void ATPSPlayer::Move(const struct FInputActionValue& inputValue)
// {
// 	FVector2D value = inputValue.Get<FVector2D>();
// 	direction.X = value.X;//전후
// 	direction.Y = value.Y;//좌우
// }
//
// void ATPSPlayer::LookUP(const struct FInputActionValue& inputValue)
// {
// 	float value = inputValue.Get<float>();
// 	AddControllerPitchInput(value);// Y축 회전
// }
//
// void ATPSPlayer::Turn(const struct FInputActionValue& inputValue)
// {
// 	float value = inputValue.Get<float>();
// 	AddControllerYawInput(value);// Z축 회전
// }
//
// void ATPSPlayer::InputJump(const struct FInputActionValue& inputValue)
// {
// 	//ACharater 내부의 기본 제공 점프 함수
// 	Jump();
// }
//
#include "TPSProject/Public/TPSPlayer.h"

#include "NiagaraFunctionLibrary.h"
#include "Bullet.h"
#include "EnemyFSM.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// TPS 카메라를 SpringArm 컴포넌트에 부착
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	springArmComp->SetupAttachment(RootComponent); // 계층 구조상 캡슐컴포넌트가 ROOT이며 스프링암을 자식으로 배치
	springArmComp->SetRelativeLocation(FVector(.0f, 70.0f, 90.0f)); // 암 컴포넌트의 시작점
	springArmComp->TargetArmLength = 400.f;
	
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	cameraComp->SetupAttachment(springArmComp);
	
	// C++에서 BP에서의 옵션들 직접 수정하는 경우 아래처럼 해당 옵션 변수들을 직접 코드로 제어 가능
	// springArmComp->bUsePawnControlRotation = true;
	// cameraComp->bUsePawnControlRotation = false;
	// bUseControllerRotationYaw = true;
	
	// 총 스켈레탈메시 컴포넌트 등록
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComponent"));
	// 캐릭터 메시 컴포넌트(GetMesh()) 부모에 부착
	gunMeshComp->SetupAttachment(GetMesh());
	
	//LineTrace가 총에 막히지 않도록 충돌 해제
	gunMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// 스켈레탈 메시 데이터 동적로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapons/GrenadeLauncher/Meshes/SKM_GrenadeLauncher.SKM_GrenadeLauncher'"));
	if (TempGunMesh.Succeeded())
	{
		// 해당 경로의 스켈레탈메시를 찾았다면, 메시 할당 + 임시 위치 보정
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(-14.0f, 52.0f, 120.0f));
	}
	
	// 스나이퍼 건 스태틱 메시 컴포넌트 등록
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGun StaticMeshComponent"));
	// 캐릭터 메시 컴포넌트(GetMesh()) 부모에 부착
	sniperGunComp->SetupAttachment(GetMesh());
	
	//LineTrace가 총에 막히지 않도록 충돌 해제
	sniperGunComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// 스태틱 메시 데이터 동적로드
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperGunMesh(TEXT("/Script/Engine.StaticMesh'/Game/Weapons/sniper/source/Meshes/sniper1.sniper1'"));
	if (TempSniperGunMesh.Succeeded())
	{
		// 해당 경로의 스태틱메시를 찾았다면, 메시 할당 + 임시 위치 보정
		sniperGunComp->SetStaticMesh(TempSniperGunMesh.Object);
		sniperGunComp->SetRelativeLocation(FVector(-14.0f, 52.0f, 120.0f));
		sniperGunComp->SetRelativeScale3D(FVector(0.8f));
	}
	
	//시작시 기본 무기로 스나이퍼 사용
	bUsingGrenadeGun  = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Enhanced Input 시스템이 IMC_TPS 사용하도록 설정
	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(imc_TPS, 0);
		}
	}
	
	//스나이퍼 UI 위젯 인스턴스 생성
	sniperUI = CreateWidget(GetWorld(), sniperFactory);
	//일반 조준 크로스헤어 UI 위젯 인스턴스 생성 -> AddViewport로 호출
	crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory); 
	if (crosshairUI)
	{
		crosshairUI->AddToViewport(); // 일반 조준선 가림
	}
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 캐릭터 이동처리에서 버그 예시)
	// GetControlRation() 함수는 좌우(YAW) 말고도 상하(PITCH)까지 포함된 카메라 전체 회전
	// PITCH 움직임에 X축을 기울이는 성질이 있어서 카메라가 아래를 향하는 상태에서 W를 누르면
	// "앞으로 가는 입력" + "아래로 박는 입력"으로 섞임 -> 수평 속도가 cos(Pitch)로 줄어듬
	
	// 컨트롤러의 현재 회전 값들(YAW, PITCH, ROLL) 값을 가져옴
	FRotator controlRot = GetControlRotation();
	// PITCH 자체를 0으로 - 기우는 현상 차단
	controlRot.Pitch = 0.f;
	// ROLL 도 0으로 고정 - 기우는 현상 차단
	controlRot.Roll = 0.f;
	
	// YAW좌우 측값만 남은 벡터를 넣어둠 -> 카메라가 어디를 보던지, 이동은 수평면 위에서만 이루어지도록 함
	direction = FRotationMatrix(controlRot).TransformVector(direction); // 자기를 기준으로 벡터 변환
	
	// 언리얼엔진에서 제공하는 위 등속 운동을 구현한 함수 AddMovementInput()
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
		PlayerInput->BindAction(ia_LookUp, ETriggerEvent::Triggered, this, &ATPSPlayer::LookUp);
		PlayerInput->BindAction(ia_Turn, ETriggerEvent::Triggered, this, &ATPSPlayer::Turn);
		PlayerInput->BindAction(ia_Move, ETriggerEvent::Triggered, this, &ATPSPlayer::Move);
		PlayerInput->BindAction(ia_Jump, ETriggerEvent::Started, this, &ATPSPlayer::InputJump);
		PlayerInput->BindAction(ia_Fire, ETriggerEvent::Started, this, &ATPSPlayer::InputFire);
		PlayerInput->BindAction(ia_GranedeGun, ETriggerEvent::Started, this, &ATPSPlayer::ChangeToGrenadeGun);
		PlayerInput->BindAction(ia_SniperGun, ETriggerEvent::Started, this, &ATPSPlayer::ChangeToSniperGun);
		PlayerInput->BindAction(ia_SniperZoom, ETriggerEvent::Started, this, &ATPSPlayer::SniperZoom);
		PlayerInput->BindAction(ia_SniperZoom, ETriggerEvent::Completed, this, &ATPSPlayer::SniperZoom);
	}
}

// 상하 회전 입력에 따른 콜백 함수 구현
void ATPSPlayer::LookUp(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerPitchInput(value); // PITCH(Y축) 회전
}

// 좌우 회전 입력에 따른 콜백 함수 구현
void ATPSPlayer::Turn(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerYawInput(value); // YAW(Z축) 회전
}

// 전후좌우 이동 입력에 따른 콜백 함수 구현
void ATPSPlayer::Move(const FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>(); // 전달받는 2D 값
	direction.X = value.X; // 전후
	direction.Y = value.Y; // 좌우
}

// 점프 입력에 따른 콜백 함수 구현
void ATPSPlayer::InputJump(const FInputActionValue& inputValue)
{
	Jump(); // ACharacter 클래스가 제공하는 기본 점프 함수 호출
}

void ATPSPlayer::InputFire(const FInputActionValue& inputValue)
{
	if (bUsingGrenadeGun)//유탄 사용
	{
		// 총 스켈레탈메시에 FirePosition 이란 이름의 소켓의 월드 트랜스폼(위치/회전)을 가져옴
		// FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		FVector spawnLoc = gunMeshComp->GetSocketLocation(TEXT("FirePosition"));
		FRotator spawnRot = cameraComp->GetComponentRotation();
		
		// 위 위치/회전으로 BulletFactory가 BP_Bullet 인스턴스를 월드에 스폰
		GetWorld()->SpawnActor<ABullet>(bulletFactory, spawnLoc, spawnRot);
	}else//스나이퍼 사용
	{
		//시작과 종료 위치 설정
		FVector startpos = cameraComp->GetComponentLocation();
		FVector endpos = startpos + cameraComp->GetForwardVector() * 5000.f;
		
		//충돌 결과 저장 / 자기 자신을 충돌 검사에서 제외
		FHitResult hitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		
		//LineTraceSingleByChannel(결과그릇, 시작위치, 종료위치, 트레이스채널, 충돌옵션)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, startpos, 
			endpos, ECC_Visibility, params);
		//Visibility 채널로 라인트레이스 수행 -> 처음 부딪힌 하나의 액터에서 정지
		//LineTrace 경로 시각화 / 초록 충돌, 빨강 미충돌
		DrawDebugLine(GetWorld(), startpos, endpos, bHit? FColor::Green : FColor::Red,
			false, 1.f, 0, .2f);
		
		if (bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit : Actor->%s, Comp->%s, Distance->%.1f, ImpactPoint->%s"),
				hitResult.GetActor()? *hitResult.GetActor()->GetName():TEXT("None"),
				hitResult.GetComponent()? *hitResult.GetComponent()->GetName():TEXT("None"),
				hitResult.Distance, *hitResult.ImpactPoint.ToString());

			// [DEBUG] 타격 위치 시각화
			DrawDebugSphere(GetWorld(), hitResult.ImpactPoint, 20.f, 12, FColor::Yellow, false, 2.f);
			
			// 타격 위치에 Niagara 이펙트 스폰
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bulletEffectFactory, hitResult.ImpactPoint);
			
			// 적 피격 처리 - FSM 컴포넌트의 OnDamageProcess() 호출
			auto enemy = hitResult.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy)
			{
				auto enemyFSM = Cast<UEnemyFSM>(enemy);
				enemyFSM->OnDamageProcess();
			}
			
			// 타격 물체에 물리 엔진 적용
			UPrimitiveComponent* hitComp = hitResult.GetComponent();
			if (hitComp && hitComp-> IsSimulatingPhysics())
			{
				// F=ma 방향 * 질량 * 가속도
				// 1. 조준 방향 - 시작점에서 종료점 방향
				FVector dir = (endpos - startpos).GetSafeNormal();
				// 2. 날려보낼 힘
				FVector force = dir * hitComp->GetMass() * 20000.f;
				// 3. 타격된 지점에 힘을 작용
				// AddForce(F) - 무게중심에 힘을 작용 -> 회전없고, 평행이동
				// AddForceAtlocation(F, pos) - 특정 위치에 힘-> 회전(토크)가 발생한다. ex: 모서리 맞으면 빙글빙글 돌면서 뒤로 밀림;
				hitComp->AddForceAtLocation(force, hitResult.ImpactPoint);
			}
			
		}
	}
}

void ATPSPlayer::ChangeToGrenadeGun(const struct FInputActionValue& inputValue)
{                                                                  
	bUsingGrenadeGun  = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}

void ATPSPlayer::ChangeToSniperGun(const struct FInputActionValue& inputValue)
{
	bUsingGrenadeGun  = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

void ATPSPlayer::SniperZoom()
{
	if (bUsingGrenadeGun)
	{
		return;
	}
	
	if (bsniperZoom == false)
	{
		bsniperZoom = true;
		sniperUI->AddToViewport();
		cameraComp->SetFieldOfView(45.f);
	}else
	{
		bsniperZoom = false;
		sniperUI->RemoveFromViewport();
		cameraComp->SetFieldOfView(90.f);
	}
}

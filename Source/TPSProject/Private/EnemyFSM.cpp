// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// 월드에서 ATPSPlayer 타겟 찾기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	// 찾은 actor를 ATPSPlayer 타입으로 캐스팅
	target = Cast<ATPSPlayer>(actor);
	// 소유 객체
	me = Cast<AEnemy>(GetOwner());
	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// [Debug] 
	if (GEngine)
	{
		FString logMsg = UEnum::GetValueAsString(mState);
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, logMsg);
	}
	
	// 현재 상태에 따라 해당 함수만 실행 - FSM
	switch (mState)
	{
	case EEnemyState :: Idle: IdleState(); break;
	case EEnemyState :: Move: MoveState(); break;
	case EEnemyState :: Attack: AttackState(); break;
	case EEnemyState :: Damage: DamageState(); break;
	case EEnemyState :: Die: DieState(); break;
	}
}

// 대기상태
void UEnemyFSM::IdleState()
{
	// 시간 누적
	currentTime += GetWorld()->GetDeltaSeconds();
	
	if (currentTime > idleDelayTime)
	{
		mState = EEnemyState::Move;
		// 경과 시간 초기화
		currentTime = 0.f;
		
	}
}

// 추적상태
void UEnemyFSM::MoveState()
{
	// 타겍 목적지
	FVector destination = target->GetActorLocation();
	// 타겟 방향 설정
	FVector dir = destination - me->GetActorLocation();
	// 타겟 방향으로 이동
	me->AddMovementInput(dir.GetSafeNormal());
	
	// Attack 상태로 전환
	// 공격범위 안으로 들어오면
	if ( dir.Size() < attackRange)
	{
		mState = EEnemyState::Attack;
	}
}

// 공격상태
void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->GetDeltaSeconds();
    
	if (currentTime > attackDelayTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack!"));
		currentTime = 0;
	}
    
	// 타겟이 공격 범위를 벗어나면 -> Move 상태로 전환
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if (distance > attackRange)
	{
		mState = EEnemyState::Move;
	}
}

// 피격상태
void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->GetDeltaSeconds();
	// 경과 시간이 대기시간을 초과했다면,
	if (currentTime > damageDelayTime)
	{
		mState = EEnemyState::Idle;
		currentTime = 0.f;
	}
}

void UEnemyFSM::OnDamageProcess()
{
	// 체력 감소
	hp--;
	// 체력이 남아있다면,
	if (hp > 0)
	{
		mState = EEnemyState::Damage;
	}
	else
	{
		mState = EEnemyState::Die;
		// 캡슐 콜리전 비활성화
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// 죽음상태
void UEnemyFSM::DieState()
{
	// 아래로 떨어뜨리기 
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieTime * GetWorld()->GetDeltaSeconds();
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	if (P.Z < -200.f)
	{
		me->Destroy();
	}
}


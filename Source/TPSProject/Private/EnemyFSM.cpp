// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"


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

	// ...
	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
	
}

// 추적상태
void UEnemyFSM::MoveState()
{
	
}

// 공격상태
void UEnemyFSM::AttackState()
{
	
}

// 피격상태
void UEnemyFSM::DamageState()
{
	
}

// 죽음상태
void UEnemyFSM::DieState()
{
	
}


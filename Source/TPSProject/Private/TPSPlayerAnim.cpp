// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerAnim.h"

#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

//매 프레임 자동 호출 - Tick 직접 구성안해도 UE가 자체 호출
void UTPSPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	//이 Animation Instance를 소유한 폰(TPSPlayer) 가져오기
	APawn* pawn = TryGetPawnOwner();
	if (pawn == nullptr) return;
	
	//APSPlayer 폰의 속도 측정
	ATPSPlayer* player = Cast<ATPSPlayer>(pawn);
	if (player)
	{
		//속도, 전후좡우 벡터 가져오기
		FVector velocity = player->GetVelocity();
		FVector forward = player->GetActorForwardVector();
		FVector right = player->GetActorRightVector();
		
		//DotProduct()로 전후 성분과 좌우 성분을 분리
		speed = FVector::DotProduct(velocity, forward); //앞-양수, 뒤-음수
		direction = FVector::DotProduct(velocity, right); //우-양수, 좌-음수
		
		//공중에 떠있는 상태인지
		UCharacterMovementComponent* movement = player->GetCharacterMovement();
		if (movement)
		{
			isInair = movement->IsFalling();
		}
	}
	
	//Debug - 프레임마다 편하는 speed 값 출력용
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,0.f,FColor::Green,
			FString::Printf(TEXT("Speed: %.1f direction: %.1f isInair=%s"),
				speed,direction,isInair?TEXT("true"):TEXT("false")));
	}
}

void UTPSPlayerAnim::PlayerAttackAnim()
{
	//UAnimInstance 내장 함수인 Montage_Play로 할당된 몽타주 재생
	Montage_Play(attcakAnimMontage);
	
}
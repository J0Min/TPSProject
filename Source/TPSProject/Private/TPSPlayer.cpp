// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSProject/Public/TPSPlayer.h"

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
	
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


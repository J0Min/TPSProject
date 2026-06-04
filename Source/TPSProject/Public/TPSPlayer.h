// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "GameFramework/Character.h"
// #include "TPSPlayer.generated.h"
//
// UCLASS()
// class TPSPROJECT_API ATPSPlayer : public ACharacter
// {
// 	GENERATED_BODY()
//
// public:
// 	// Sets default values for this character's properties
// 	ATPSPlayer();
//
// protected:
// 	// Called when the game starts or when spawned
// 	virtual void BeginPlay() override;
//
// public:
// 	// Called every frame
// 	virtual void Tick(float DeltaTime) override;
//
// 	// Called to bind functionality to input
// 	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
// 	
// 	//스프링 암 컴포넌트
// 	UPROPERTY(VisibleAnywhere, Category = Camera)
// 	class USpringArmComponent* springArmComp;
// 	
// 	//카메라 컴포넌트
// 	UPROPERTY(VisibleAnywhere, Category = Camera)
// 	class UCameraComponent* cameraComp;
//
// 	//입력부
// 	UPROPERTY(EditDefaultsOnly, Category = Input)
// 	class UInputMappingContext* imc_TPS;
// 	
// 	UPROPERTY(EditDefaultsOnly, Category = Input)
// 	class UInputAction* ia_LookUP;
// 	
// 	UPROPERTY(EditDefaultsOnly, Category = Input)
// 	class UInputAction* ia_Turn;
// 	
// 	UPROPERTY(EditDefaultsOnly, Category = Input)
// 	class UInputAction* ia_Move;
// 	
// 	UPROPERTY(EditDefaultsOnly, Category = Input)
// 	class UInputAction* ia_Jump;
// 	
// 	//이동 속도
// 	UPROPERTY(EditDefaultsOnly, Category = PlayerSetting)
// 	float walkSpeed = 600.f;
// 	
// 	//이동 방향
// 	FVector direction;
// 	
// 	void Move(const struct FInputActionValue& inputValue);
// 	void LookUP(const struct FInputActionValue& inputValue);
// 	void Turn(const struct FInputActionValue& inputValue);
// 	void InputJump(const struct FInputActionValue& inputValue);
// };

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// 스프링 암 컴포넌트 선언
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;
	
	// 카메라 컴포넌트 선언
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* cameraComp;
	
	// 총 스켈레탈 메시 컴포넌트 선언
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* gunMeshComp;
	
	// IMC 선택 필드 선언
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputMappingContext* imc_TPS;
	
	// 상하좌우 마우스 IA 선택 필드 선언
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* ia_LookUp;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* ia_Turn;
	
	// 전후좌우 키보드 이동 IA 필드 선언
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* ia_Move;

	// 키보드 점프 IA 필드 선언
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* ia_Jump;
	
	// 마우스 발사 IA 필드 선언
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* ia_Fire;
	
	// 이동 속도
	UPROPERTY(EditDefaultsOnly, Category = PlayerSetting)
	float walkSpeed = 600.f;
	
	// 총알 스폰 팩토리
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;
	
	// 스나이퍼 총 스태틱 메시 컴포넌트 선언
	UPROPERTY(VisibleAnywhere, Category=GunMesh)
	class UStaticMeshComponent* sniperGunComp;
	
	// 현재 유탕총을 사용중인지 여부 (true = 유탄총, false = 스나이퍼)
	bool bUsingGrenadeGun  = true;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* ia_GranedeGun;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* ia_SniperGun;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* ia_SniperZoom;
	
	//스나이퍼 UI 위젯 팩토리 (WBP_SniperUI에서 할당 필요)
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> sniperFactory;
	
	// 이동 방향
	FVector direction;
	
	//현재 줌 상태인지 체크
	bool bsniperZoom = false;
	
	//런타일에 생성될 UI 인스턴스 /GC보호용 빈 프로퍼티
	UPROPERTY()
	class UUserWidget* sniperUI;
	
	UPROPERTY()
	class UUserWidget* crosshairUI;
	
	UPROPERTY(EditDefaultsOnly, Category="SniperUI")
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	
	// 상하 회전 입력 함수 선언
	void LookUp(const struct FInputActionValue& inputValue);
	
	// 좌우 회전 입력 함수 선언
	void Turn(const struct FInputActionValue& inputValue);
	
	// 전후좌우 이동 입력 함수 선언
	void Move(const struct FInputActionValue& inputValue);
	
	// 점프 입력 함수 선언
	void InputJump(const struct FInputActionValue& inputValue);
	
	// 총알 발사 입력 함수 선언
	void InputFire(const struct FInputActionValue& inputValue);
	
	//유탄총 교체 함수
	void ChangeToGrenadeGun(const struct FInputActionValue& inputValue);
	
	//스나이퍼건 교체 함수
	void ChangeToSniperGun(const struct FInputActionValue& inputValue);
	
	void SniperZoom();
	
	//스나이퍼 사격 명중시 표시될 총알 파편 효과 선언
	UPROPERTY(EditDefaultsOnly, Category = BulletEffect)
	class UNiagaraSystem* bulletEffectFactory;
};
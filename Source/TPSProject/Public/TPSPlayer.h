// Fill out your copyright notice in the Description page of Project Settings.

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
	
	//스프링 암 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;
	
	//카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* cameraComp;

	//입력부
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputMappingContext* imc_TPS;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* ia_LookUP;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* ia_Turn;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* ia_Move;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* ia_Jump;
	
	//이동 속도
	UPROPERTY(EditDefaultsOnly, Category = PlayerSetting)
	float walkSpeed = 600.f;
	
	//이동 방향
	FVector direction;
	
	void Move(const struct FInputActionValue& inputValue);
	void LookUP(const struct FInputActionValue& inputValue);
	void Turn(const struct FInputActionValue& inputValue);
	void InputJump(const struct FInputActionValue& inputValue);
};

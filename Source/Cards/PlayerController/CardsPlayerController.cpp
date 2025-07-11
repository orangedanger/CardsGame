// Future do it


#include "CardsPlayerController.h"

#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "SAdvancedTransformInputBox.h"
#include "Cards/Character/CharacterBase.h"
#include "Cards/Pawn/CameraPawn.h"

void ACardsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//设置输入
	check(Context);
	UEnhancedInputLocalPlayerSubsystem* Subsystem =ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(Context,0);

	//设置鼠标显示
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputData;
	InputData.SetHideCursorDuringCapture(false);
	InputData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputData);
	
}

void ACardsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&ACardsPlayerController::Move);
	EnhancedInputComponent->BindAction(ZoomAction,ETriggerEvent::Triggered,this,&ACardsPlayerController::Zoom);
	EnhancedInputComponent->BindAction(RotateAction,ETriggerEvent::Triggered,this,&ACardsPlayerController::Rotate);
}

void ACardsPlayerController::Move(const FInputActionValue& Input)
{
	const FVector PlayerAxisVector = Input.Get<FVector>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0,Rotation.Yaw,0);

	//为了获得关于X,Y的旋转大小
	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControllerPawn = GetPawn())
	{
		constexpr float SpeedBonus = 0.5f;
		ControllerPawn->AddMovementInput(ForwardDir,SpeedBonus*PlayerAxisVector.X);
		ControllerPawn->AddMovementInput(RightDir,SpeedBonus*PlayerAxisVector.Y);
		ControllerPawn->AddMovementInput(FVector(0,0,1),PlayerAxisVector.Z);
	}
}

void ACardsPlayerController::Zoom(const FInputActionValue& Input)
{
	//鼠标滚轮控制缩放
	const float ZoomDir = Input.Get<float>();

	const ACameraPawn* CameraPawn = CastChecked<ACameraPawn>(GetPawn());
	if(CameraPawn)
	{
		constexpr float ZoomScale = 50.f;
		CameraPawn->AddSpringArmLength(ZoomScale*ZoomDir);
	}
}

void ACardsPlayerController::Rotate(const FInputActionValue& Input)
{
	//Q,E控制左右旋转
	const float RotateDir = Input.Get<float>();

	SetControlRotation(GetControlRotation()+FRotator(0,RotateDir,0));
}

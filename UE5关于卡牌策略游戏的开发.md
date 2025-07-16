# UE5关于卡牌策略游戏的开发

## 0.git的设置和项目设置

### 0.1创建git仓库

[github](https://github.com/)中新建repository
输入仓库名即可创建git

需要.gitignore文件

```c++
Binaries
DerivedDataCache
Intermediate
Saved
Build

.vscode
.vs
*.VC.db
*.opensdf
*.opendb
*.sdf
*.sln
*.suo
*.xcodeproj
*.xcworkspace
```



在git中新建git后需要在对于文件中打开GitBash

```c++
git init
git status
git add .
git status
git commit -m "这是一个提交的名字"
git branch -M main
复制你网站上的链接
如:
git remote add origin git@github.com:orangedanger/Aura.git
git push -u origin main
```

如果在修改后进行提交

```c++
git add .
git status
git commit -m "这是一个提交的名字"
git push -u orgin main
```



关于项目设置

### 0.2.设置为Idea启动项目

在编译器偏好中设置编译器为Idea uproject

关闭 实时代码编写
关闭 自动编译新添C++



打开项目文件夹对项目文件右键选择打开方式为idea即可

## 1.设置摄像头和鼠标选定角色进行移动

### 1.1.一些增强输入和输入模式的设置

需要在 Cards.Build.cs中添加[EnhancedInput](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Plugins/EnhancedInput)Module才能进行添加 UInputMappingContext

```c++
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
    
	//设置游戏类型显示鼠标
	FInputModeGameAndUI InputData;
	InputData.SetHideCursorDuringCapture(false);
	InputData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputData);
	
}
```

### 1.2.增强输入的绑定

绑定对应功能函数

```c++
void ACardsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInputComponent);
	//绑定对应功能函数
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&ACardsPlayerController::Move);
	EnhancedInputComponent->BindAction(ZoomAction,ETriggerEvent::Triggered,this,&ACardsPlayerController::Zoom);
	EnhancedInputComponent->BindAction(RotateAction,ETriggerEvent::Triggered,this,&ACardsPlayerController::Rotate);
}
```

### 1.3.摄像机功能实现

摄像机的移动实现

```c++
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

```

```c++
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
```



```c++
void ACardsPlayerController::Rotate(const FInputActionValue& Input)
{
	//Q,E控制左右旋转
	const float RotateDir = Input.Get<float>();

	SetControlRotation(GetControlRotation()+FRotator(0,RotateDir,0));
}
```

## 2.材质制作 





## 3.棋盘地形和棋子

### 3.1.棋盘

#### 3.1.1.棋盘网格制作

棋盘是一个Actor

我们需要一个绘制棋盘的函数DrawGrid()
棋盘的绘制需要中心点，网格的大小，网格的数量

```c++
UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess=true),Category="Settings")
FVector CenterLocation;
UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess=true),Category="Settings")
FVector GridSize;
UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess=true),Category="Settings")
FVector2D CountSize;
```

通c过这些即可简单的绘制一个棋盘

```c++
void AGridsActor::DrawGrid()
{
	//更新CenterLocation
	CenterLocation= GetActorLocation();
    
	//四舍五入长和宽
	const int32 Length =FMath::RoundToInt32(CountSize.X);
	const int32 Width =FMath::RoundToInt32(CountSize.Y);
	
	//计算最左端点
	LeftBottonLocation =CenterLocation -  FVector(Length * GridSize.X/2,Width * GridSize.Y/2,-1);
    
	//设置每个的缩放和位置
	FTransform Transform = FTransform();
	FVector Scale = GridSize /Mesh->GetStaticMesh()->GetBoundingBox().GetSize();
	Scale.Z =1;
	Transform.SetScale3D(Scale);
	FVector StartLocation = LeftBottonLocation;
	//遍历生成每个网格
	for (int i = 0; i < Length; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			//Grids的位置偏移
			StartLocation = FVector(LeftBottonLocation.X + i* GridSize.X, LeftBottonLocation.Y + j*GridSize.Y, LeftBottonLocation.Z);

			//检测是否碰撞来选择是否进行添加
			constexpr float OffsetGroundValue = 1.f;
			StartLocation.Z  +=  OffsetGroundValue;
			Transform.SetLocation(StartLocation);
			//设置bWorldSpace
			Mesh->AddInstance(Transform,true);
		}
	}
}
```

CalculateLeftBottonLocation()函数简单的计算左下角的位置需要减去多少

```c++
FVector AGridsActor::ProcessLeftBottonLocation(const int32 Length ,const int32 Width) const
{
	//当长度或宽度为奇数时 长度或宽度需要减去半个Size大小
	FVector Subtract = FVector(Length * GridSize.X/2,Width * GridSize.Y/2,0);
	if(Length%2 !=0)
	{
		Subtract.X -=GridSize.X/2;
	}
	if(Width%2 !=0)
	{
		Subtract.Y -=GridSize.Y/2;
	}
	return CenterLocation - Subtract;
}
```

#### 3.1.2.棋盘对齐

AddInstance需要的是WorldSpace

只有当Instance的位置为世界位置时对齐才有作用
不然CenterLocation一直是相对位置(0,0,0)没效果

```c++
FVector UISMGridSyncBlueprintLibrary::SnapToGrid(UObject* WorldContext,const FVector& StartCenter, const FVector& GridSize)
{
	const float X = UKismetMathLibrary::GridSnap_Float(StartCenter.X,GridSize.X);
	const float Y = UKismetMathLibrary::GridSnap_Float(StartCenter.Y,GridSize.Y);
	const float Z = UKismetMathLibrary::GridSnap_Float(StartCenter.Z,GridSize.Z);
	FVector Result;
	Result.X = X;
	Result.Y = Y;
	Result.Z = Z;

	return Result;
}
```

需要注意当长或者宽为偶数时的修改
当长或者宽为偶数时
由于CenterLocation需要对齐网格导致网格往一边偏移了半个GridSize需要进行处理

```c++
FVector AGridsActor::ProcessLeftBottonLocation(const int32 Length ,const int32 Width) const
{
	//当长度或宽度为奇数时 长度或宽度需要减去半个Size大小
	FVector Subtract = FVector(Length * GridSize.X/2,Width * GridSize.Y/2,-1);
	if(Length%2 !=0)
	{
		Subtract.X -=GridSize.X/2;
	}
	if(Width%2 !=0)
	{
		Subtract.Y -=GridSize.Y/2;
	}
	return CenterLocation - Subtract;
}
```



修改后

```
//同步到网格上
CenterLocation = UISMGridSyncBlueprintLibrary::SnapToGrid(this,CenterLocation ,GridSize);

//计算最左端点
LeftBottonLocation = ProcessLeftBottonLocation(Length ,Width);
```



```c++
void AGridsActor::DrawGrid()
{
	//更新CenterLocation
	CenterLocation= GetActorLocation();
    
    //同步到网格上
	CenterLocation = UISMGridSyncBlueprintLibrary::SnapToGrid(this,CenterLocation ,GridSize);
    
	//四舍五入长和宽
	const int32 Length =FMath::RoundToInt32(CountSize.X);
	const int32 Width =FMath::RoundToInt32(CountSize.Y);
	
	//计算最左端点
	LeftBottonLocation = ProcessLeftBottonLocation(Length ,Width);
	
	//设置每个的缩放和位置
	FTransform Transform = FTransform();
	FVector Scale = GridSize /Mesh->GetStaticMesh()->GetBoundingBox().GetSize();
	Scale.Z =1;
	Transform.SetScale3D(Scale);
	FVector StartLocation = LeftBottonLocation;
	//遍历生成每个网格
	for (int i = 0; i < Length; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			//Grids的位置偏移
			StartLocation = FVector(LeftBottonLocation.X + i* GridSize.X, LeftBottonLocation.Y + j*GridSize.Y, LeftBottonLocation.Z);

			//检测是否碰撞来选择是否进行添加
			constexpr float OffsetGroundValue = 1.f;
			StartLocation.Z  +=  OffsetGroundValue;
			Transform.SetLocation(StartLocation);
			//设置bWorldSpace
			Mesh->AddInstance(Transform,true);
		}
	}
}
```



#### 3.1.3.检测环境

判断是否可以AddInstance

需要增加碰撞检测

在Ue中自定义碰撞通道增加一个

```c++
//对碰撞的变量进行初始化
TArray<FHitResult> Hits;
const float Radius = GridSize.X/3;
FVector Start(StartLocation.X, StartLocation.Y, StartLocation.Z + 500);
FVector End(StartLocation.X, StartLocation.Y, StartLocation.Z - 500);
			
//进行碰撞检测
UKismetSystemLibrary::SphereTraceMulti(
				this,
				Start,
				End,
				Radius,
				ETraceTypeQuery(ECC_GameTraceChannel1),
				false,
				TArray<AActor*>(),
				EDrawDebugTrace::None,
				Hits,
				true);
```

添加一个Actor将不能走的位置添加上这个Actor

```c++
bool AGridsActor::IsWalkable(TArray<FHitResult> Hits)
{
	bool bIsWalkable = true;
	for(auto& Hit : Hits)
	{
		if(ARegionalActor* RegionalActor =  Cast<ARegionalActor>(Hit.GetActor()))
		{
			ERegionalActor RegionalType = RegionalActor->GetRegionalType();
			if(RegionalType == ERegionalActor::Mountain || RegionalType == ERegionalActor::River ||RegionalType == ERegionalActor::Ocean)
			{
				bIsWalkable = false;
				break;
			}
		}
	}
	return bIsWalkable;
}
```

并将检测判断用一个函数包裹

```c++
void AGridsActor::AddGridsToMesh(FTransform Transform, FVector& StartLocation, TArray<FHitResult> Hits, const float Radius) const
{
	//如果检测到碰撞且满足可以行走
	if(Hits.Num()>0 && IsWalkable(Hits))
	{
		constexpr float OffsetGroundValue = 1.f;
		StartLocation.Z = Hits[0].Location.Z  - Radius + OffsetGroundValue;
		Transform.SetLocation(StartLocation);
		//设置bWorldSpace
		Mesh->AddInstance(Transform,true);
	}
}
```

#### 3.1.4.检测鼠标位置



鼠标位置通常由PlayerController来获取

DeprojectMousePositionToWorld

```c++
if (const ACardsPlayerController* CardsPC = Cast<ACardsPlayerController>(UGameplayStatics::GetPlayerController(WorldContext,0)))
		{
			FVector StartLocation;
			FVector WorldDirection;
			//将2D的鼠标位置转换成3D的位置
			CardsPC->DeprojectMousePositionToWorld(StartLocation,WorldDirection);
			//设置一个远处的点
			WorldDirection *= 99999999;
			const FVector EndLocation = StartLocation + WorldDirection;

			//计算与Grids平面的交点
			HitLocation = FMath::LinePlaneIntersection(
				StartLocation,
				EndLocation,
				GridsCenter,
				FVector(0,0,1));
		}
```



#### 3.2.棋盘瓦片

#### 3.2.1.棋盘瓦片信息

需要给瓦片增加一些信息
如：Index,Shape,Transform





## 4.UI界面

### 4.1.DebugMenu

#### 4.1.1.创建一个Debug界面

通过创建Widget进行使用WidgetSwitcher

可以通过index来切换不同的界面
Index就是在WidgetSwitcher子项的排序

再通过HUD去CreateWidget -> AddViewport

#### 4.1.2.完善Debug的第一个按钮功能

##### 4.1.2.1.GridsMenu设置

需要完成对Grids的局内设置
将Grids可修改量添加进Widget

#### 4.1.4.完善Debug的第二个按钮功能







Today To DO：

完善 Index 和 ShapeType 

我需要在一个结构体中存储 Tile的信息



由于自定义结构体没加BlueprintType

导致参数 Tiles无法暴露给蓝图



完成 鼠标移动到Tile上进行显示



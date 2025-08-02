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

对齐公式

```c++
(Floor((Location + (Grid/(T)2)) / Grid) * Grid)
```

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



### 3.2.棋盘瓦片

#### 3.2.1.棋盘瓦片信息



```c++
USTRUCT(BlueprintType,Blueprintable)
struct FTileInfo
{
 GENERATED_BODY()
    public:
    UPROPERTY(BlueprintReadOnly)
    ETileShape TileShape = ETileShape::None ;  //形状
    UPROPERTY(BlueprintReadOnly)
    FTransform Transform = FTransform();   //位置旋转缩放信息
};
```

这是棋盘信息的结构体

主要存储形状和Tile的Shape

```c++
Tiles.Add(intPoint,FTileInfo(ShapeType ,Transform));
```

这样每次读取每个Tiles时都能获取相应的Transform和ShapeType

关于Index

对于三角形和正方形返回 x y即可

```c++
FIntPoint intPoint = FIntPoint(i,j);
```

对于六边形，我暂时采用双坐标

```c++
	//由于我们使用的是双坐标 宽度
	//TODO:后期需要进行修改
	if (ShapeType == ETileShape::Hexagon)
	{
		Length /=2;
		Width*=2;
	}
```



```c++
//六边形为双坐标宽度的
if (ShapeType == ETileShape::Hexagon)
{
	const int32 OffSetX = j%2==0?1:0; 
	intPoint.X *= 2;
	intPoint.X +=OffSetX;
}
```



#### 3.2.2.瓦片的形状

[对于六边形网格的研究资料][https://www.redblobgames.com/grids/hexagons/]

添加ShapeType

对于六边形网格，我们采用了双倍宽度坐标系
即计算距离时 abs(高度差) + max(0,abs(宽度差)/2)

即可求出距离

瓦片添加形状导致生成瓦片时不同

```c++
void AGridsActor::CalculateGridPositionAndRotation(FTransform &OutTransform,const bool IsLengthEven,const bool IsWidthEven,const double LengthOffSet,const double WidthOffset) const
{
	//不同形状的绘制不同
	FRotator OffsetRotation = FRotator::ZeroRotator;
	FVector OffsetLocation = FVector::ZeroVector;
	switch (ShapeType)
	{
	case ETileShape::None:
		break;
	case ETileShape::Square:
		//Grids的位置偏移
		OffsetLocation = FVector(LeftBottomLocation.X + LengthOffSet, LeftBottomLocation.Y + WidthOffset, LeftBottomLocation.Z);
		break;
	case ETileShape::Triangle:
		//Grids的位置偏移
		OffsetLocation = FVector(LeftBottomLocation.X + LengthOffSet, LeftBottomLocation.Y + WidthOffset/2, LeftBottomLocation.Z);
		//是偶数时翻转
		if (IsWidthEven)
		{
			OffsetRotation +=FRotator(0,180,0);
		}
		if (IsLengthEven)
		{
			OffsetRotation +=FRotator(0,180,0);
		}
		break;
	case ETileShape::Hexagon:
		//Grids的位置偏移
		OffsetLocation = FVector(LeftBottomLocation.X + LengthOffSet *1.5 , LeftBottomLocation.Y + WidthOffset/2, LeftBottomLocation.Z);
		if (IsWidthEven)
		{
			OffsetLocation += FVector(GridSize.X * 0.75, 0 ,0);
		}
		break;
	}
	OutTransform.SetRotation(OffsetRotation.Quaternion());
	OutTransform.SetLocation(OffsetLocation);
}


```

对于不同形状的Index获取

```c++
FIntPoint UISMGridSyncBlueprintLibrary::GetIndexFromWorldLocation(const UObject* WorldContext, const FVector& WorldLocation,
    const AGridsActor* Grids)
{

    //定义一些需要使用的数据
    FIntPoint Index =FIntPoint::ZeroValue;
    FVector SnapLocation = FVector::ZeroVector;
    FVector2D SizeVector2D = FVector2D::ZeroVector;
    FVector2D SnapVector2D = FVector2D::ZeroVector;
    FVector2D Result = FVector2D::ZeroVector;

    //转化一下数据到2D格式
    const FVector GridSize = Grids->GetGridSize();
    //本地坐标:相对网格左下角的坐标
    const FVector LocalLocation = WorldLocation - Grids->GetLeftBottomLocation();

    //需要根据Shape的不同采取不同的方法
    switch (Grids->GetShapeType())
    {
    case ETileShape::None:
       break;
    case ETileShape::Square:
       //对于正方形之间获取相对坐标然后除以GridSize即可
       SnapLocation= SnapToGrid(WorldContext,LocalLocation,GridSize);
       
       SizeVector2D = FVector2D(GridSize.X,GridSize.Y);
       SnapVector2D = FVector2D(SnapLocation.X,SnapLocation.Y);
       
       Result = SnapVector2D / SizeVector2D;
       Index =Result.IntPoint();
       break;
    case ETileShape::Triangle:
       
       //三角形处理
       //这里把三角形当中一半的正方形处理以快速运算
       SnapLocation = SnapToGrid(
          WorldContext,
          LocalLocation,
          GridSize*FVector(1,0.5,1)
          );
       
       SizeVector2D = FVector2D(GridSize.X,GridSize.Y / 2);
       
       SnapVector2D = FVector2D(SnapLocation.X,SnapLocation.Y);
       
       Result = SnapVector2D / SizeVector2D;
    
       Index =Result.IntPoint();
       break;
    case ETileShape::Hexagon:

       //六边形经过处理
       //GridSize.X表示的是六边形的垂直长度
       //GridSize.Y表示的是六边形的水平长度
       
       //LocalLocation对于Y上的乘2是因为我们采用的是 宽度的双坐标 需要对宽度乘以二做处理
       //GridSize。Y*0.25原因是由于
       
       //0.75 * GridSize.X 上下两个六边形中心的垂直长度
       //+FVector(25.f,0,0)的原因是由于Snap只会Snap到 -50 0 50，50倍数的点位
       //实际上X坐标需要的点位是-25 25 75 等点位

       //TODO: 我希望 0 ~ 50 Snap到 25   50 ~ 100 Snap 到 75
       //然而现在是 -25 ~ 25 Snap 0     25 ~ 75 Snap 50
       //Snap公式 (Floor((Location + (Grid/(T)2)) / Grid) * Grid)
       //关键点 Floor((Location + (Grid/(T)2)) 需要让它 原本是 (1<=X<2 ) == 1 ->    (0.5 <=X<1.5） ==   0.5
       
       
       SnapLocation = SnapToGrid(
          WorldContext,
          LocalLocation*FVector(1,2,1),
          GridSize* FVector(0.75,0.5,1)
          );
       
       SnapLocation.Y = FMath::CeilToInt(LocalLocation.Y / (GridSize.Y/2) )*GridSize.Y/2 - GridSize.Y/4;
          
       
       SizeVector2D = FVector2D(GridSize.X * 0.75 ,GridSize.Y*0.25);
       
       SnapVector2D = FVector2D(SnapLocation.X,SnapLocation.Y);

       Result = SnapVector2D / SizeVector2D;

       Index =Result.IntPoint();
       
       
       if (Index.X % 2 ==0)
       {
          const int Base = (Index.Y - 1) / 4;
          // 奇数参考值映射：1,3→1; 5,7→3; 9,11→5
          Index.Y =2 * Base + 1;
       }
       else
       {
          const int Base = (Index.Y - 3) / 4;
          // 偶数参考值映射：3,5→2; 7,9→4; 11,13→6
          Index.Y =2 * (Base + 1);
       }
       
       break;
    }

    
    return Index;
}
```



#### 3.2.3.瓦片的点击

当我鼠标触碰到这个Tile时去更换Material

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




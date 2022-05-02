#include "ProtoFECharacter.h"
#include "ProtoFEData.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoFEPlayerController.h"
#include "UI/StatsWindowParent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SnapToGrid.h"
#include "Components/GridOccupyComponent.h"
#include "tile.h"
#include "AI/ProtoFEAIController.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"
#include "UI/ToolTipParent.h"
#include "UI/HealthBarParent.h"
#include "Components/WidgetComponent.h"
#include "Items/Weapons/IronSword.h"
#include "Components/InventoryComponent.h"
#include "Abilities/Ability.h"
#include "Abilities/Ability_Slash.h"
#include "Abilities/MyAbilitySystemComponent.h"
#include "Abilities/MyAttributeSet.h"
#include "Components/AbilityComponent.h"


AProtoFECharacter::AProtoFECharacter()
{
	// RootComponent = GetMesh();
	// GetCapsuleComponent()->SetRelativeLocation(FVector(0, 0, 80));
	/** Capsule */
	GetCapsuleComponent()->InitCapsuleSize(95.0f, 40.0f);
	GetCapsuleComponent()->SetVisibility(false);

	/** Arrow Component */
	// GetArrowComponent()->SetVisibility(false);

	/** Mesh */
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -95.0f));

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// lul
	// GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	// GetCharacterMovement()->MaxAcceleration = 6000.0f;
	
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AIControllerClass = AProtoFEAIController::StaticClass();
	
	ConstructorHelpers::FClassFinder<UUserWidget> StatsWindow(TEXT("WidgetBlueprint'/Game/TopDownCPP/Blueprints/Widgets/CursorOver_StatsWindow'"));
	if (StatsWindow.Succeeded())
		StatsWindowClass = StatsWindow.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> ToolTip(TEXT("WidgetBlueprint'/Game/TopDownCPP/Blueprints/Widgets/ToolTip'"));
	if (ToolTip.Succeeded())
		ToolTipClass = ToolTip.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> HealthBar(TEXT("WidgetBlueprint'/Game/TopDownCPP/Blueprints/Widgets/HealthBar'"));
	if (HealthBar.Succeeded())
		HealthBarClass = HealthBar.Class;

	// terrain move cost
	TerrainMoveCost.Add(ETerrain::Normal, 1);
	TerrainMoveCost.Add(ETerrain::Foliage, 2);
	TerrainMoveCost.Add(ETerrain::Water, 3);
	TerrainMoveCost.Add(ETerrain::Mountains, 4);
	TerrainMoveCost.Add(ETerrain::Impossible, 999);

	/** new components */
	GridOccupyComponent = CreateDefaultSubobject<UGridOccupyComponent>(TEXT("Grid Occupy Component"));
	GridSnapComponent = CreateDefaultSubobject<USnapToGrid>(TEXT("Grid Snap Component"));
	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health bar"));
	HealthBarComponent->SetupAttachment(GetMesh());
	HealthBarComponent->SetWidgetClass(HealthBarClass);
	HealthBarComponent->SetRelativeLocation(FVector(0, 0, 200));
	HealthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarComponent->SetDrawSize(FVector2D(100, 100));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("Ability Component"));

	// inventory
	FWeaponStats Stats = FWeaponStats();
	Stats.Crit = 0.80;
	UIronSword* Weapon1 = NewObject<UIronSword>();
	Weapon1->SetStats(Stats);
	InventoryComponent->GiveItem(Weapon1);

	// abilities
	AbilityComponent->GiveAbility(UAbility_Slash::StaticClass());
	
}

void AProtoFECharacter::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (bFinished)
	{
		GridSnapComponent->SnapToClosestTile();
	}
}

void AProtoFECharacter::BeginPlay() 
{
	Super::BeginPlay();
	
	// on cursor over
	GetCapsuleComponent()->OnBeginCursorOver.AddDynamic(this, &AProtoFECharacter::OnCursorOver);
	GetCapsuleComponent()->OnEndCursorOver.AddDynamic(this, &AProtoFECharacter::EndCursorOver);
	
	if (UHealthBarParent* HB = Cast<UHealthBarParent>(HealthBarComponent->GetUserWidgetObject()))
	{
		HB->Char = this;
	}
}

void AProtoFECharacter::Tick(float DeltaSeconds)
{
   Super::Tick(DeltaSeconds);

}

void AProtoFECharacter::Destroyed()
{
	Super::Destroyed();

	DeleteFromCurrentTile();
}

void AProtoFECharacter::Select() {}

void AProtoFECharacter::Wait()
{
	TakeTerminalAction();
}

void AProtoFECharacter::TakeTerminalAction()
{
	Active = false;
}

void AProtoFECharacter::OnCursorOver(UPrimitiveComponent* Comp)
{
	DisplayStats();
	if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		GetWorld()->GetTimerManager().SetTimer(ToolTipTimer, this, &AProtoFECharacter::CreateToolTipWindow, PlayerController->ToolTipDelay, false);
	}
}

void AProtoFECharacter::EndCursorOver(UPrimitiveComponent* Comp)
{
	RemoveStats();
	GetWorld()->GetTimerManager().ClearTimer(ToolTipTimer);
	RemoveToolTip();
}

void AProtoFECharacter::BreadthSearch() {}

void AProtoFECharacter::DeleteFromCurrentTile() 
{
	if (GridOccupyComponent->OccupiedTile)
	{
		GridOccupyComponent->OccupiedTile->Data.OccupiedBy = nullptr;
		GridOccupyComponent->OccupiedTile = nullptr;
	}
}

void AProtoFECharacter::OccupyNewTile(UTile* NewTile)
{
	NewTile->Data.OccupiedBy = this;
	GridOccupyComponent->OccupiedTile = NewTile;
}

void AProtoFECharacter::DisplayStats() 
{
	if (!StatsWindowClass) return;
	if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		StatsWindowWidget = Cast<UStatsWindowParent>(PlayerController->DisplayWidget(StatsWindowClass));
		if (!StatsWindowWidget) return;
		StatsWindowWidget->Info = Information; // sets data to be displayed in widget
	}
}

void AProtoFECharacter::RemoveStats()
{
	if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerController->RemoveWidget(StatsWindowWidget);
	}
}

void AProtoFECharacter::CreateToolTipWindow()
{
	if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		ToolTipObj = Cast<UToolTipParent>(PlayerController->DisplayWidget(ToolTipClass));
		ToolTipObj->FocusedChar = this;
	}
}


void AProtoFECharacter::RemoveToolTip()
{
	if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerController->RemoveWidget(ToolTipObj);
	}
}

void AProtoFECharacter::TakeDamage(int Damage)
{
	Information.Stats.CurrentHealth = FMath::Clamp<int>(
		Information.Stats.CurrentHealth - Damage, 
		0, 
		Information.Stats.CurrentHealth
	);
}
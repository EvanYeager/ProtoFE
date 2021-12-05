#include "ProtoFECharacter.h"
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


AProtoFECharacter::AProtoFECharacter()
{
	/** Capsule */
	GetCapsuleComponent()->InitCapsuleSize(1.0f, 1.0f);
	// GetCapsuleComponent()->SetRelativeLocation(FVector(0, 0, 50));
	GetCapsuleComponent()->SetVisibility(false);

	/** Arrow Component */
	GetArrowComponent()->SetVisibility(false);

	/** Mesh */
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);

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
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	GetCharacterMovement()->MaxAcceleration = 6000.0f;


	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AIControllerClass = AProtoFEAIController::StaticClass();
	
	ConstructorHelpers::FClassFinder<UUserWidget> StatsWindow(TEXT("WidgetBlueprint'/Game/TopDownCPP/Blueprints/Widgets/CursorOver_StatsWindow'"));
	if (StatsWindow.Succeeded())
		StatsWindowClass = StatsWindow.Class;

	// terrain move cost
	TerrainMoveCost.Add(ETerrain::Normal, 1);
	TerrainMoveCost.Add(ETerrain::Foliage, 2);
	TerrainMoveCost.Add(ETerrain::Water, 3);
	TerrainMoveCost.Add(ETerrain::Mountains, 4);
	TerrainMoveCost.Add(ETerrain::Impossible, 999);

	/** new components */
	GridOccupyComponent = CreateDefaultSubobject<UGridOccupyComponent>(TEXT("Grid Occupy Component"));
	GridSnapComponent = CreateDefaultSubobject<USnapToGrid>(TEXT("Grid Snap Component"));
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
	GetMesh()->OnBeginCursorOver.AddDynamic(this, &AProtoFECharacter::OnCursorOver);
	GetMesh()->OnEndCursorOver.AddDynamic(this, &AProtoFECharacter::EndCursorOver);
}

void AProtoFECharacter::Tick(float DeltaSeconds)
{
   Super::Tick(DeltaSeconds);

}

void AProtoFECharacter::Select() {}

void AProtoFECharacter::OnCursorOver(UPrimitiveComponent* comp)
{
	DisplayStats();
}

void AProtoFECharacter::EndCursorOver(UPrimitiveComponent* comp)
{
	RemoveStats();
}

void AProtoFECharacter::BreadthSearch() {}

void AProtoFECharacter::OccupyTile(UTile* NewTile)
{
	if (GridOccupyComponent->OccupiedTile != nullptr) // if OccupiedTile is not the default
		GridOccupyComponent->OccupiedTile->Data.OccupiedBy = nullptr; // delete from old tile

	// add on new tile
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

 
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


AProtoFECharacter::AProtoFECharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(1.0f, 1.0f);

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

	GetCapsuleComponent()->SetVisibility(false);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	ConstructorHelpers::FClassFinder<UUserWidget> StatsWindow(TEXT("WidgetBlueprint'/Game/TopDownCPP/Blueprints/Widgets/CursorOver_StatsWindow'"));
	if (StatsWindow.Succeeded())
		StatsWindowClass = StatsWindow.Class;

	// terrain move cost
	TerrainMoveCost.Add(ETerrain::Normal, 1);
	TerrainMoveCost.Add(ETerrain::Foliage, 2);
	TerrainMoveCost.Add(ETerrain::Water, 3);
	TerrainMoveCost.Add(ETerrain::Mountains, 4);
	TerrainMoveCost.Add(ETerrain::Impossible, 999);

	GridOccupyComponent = CreateDefaultSubobject<UGridOccupyComponent>(TEXT("Grid Occupy Component"));
	GridSnapComponent = CreateDefaultSubobject<USnapToGrid>(TEXT("Grid Snap Component"));
}

void AProtoFECharacter::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (bFinished)
	{
		UE_LOG(LogTemp, Warning, TEXT("about to occupy tile. Occupied tile is %s"), *GridOccupyComponent->OccupiedTile.ToString());
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

void AProtoFECharacter::SelectCharacter() {}

void AProtoFECharacter::OnCursorOver(UPrimitiveComponent* comp)
{
	DisplayStats();
}

void AProtoFECharacter::EndCursorOver(UPrimitiveComponent* comp)
{
	RemoveStats();
}

void AProtoFECharacter::OccupyTile(FIntPoint NewTile)
{
	if (GridOccupyComponent->OccupiedTile.operator!=(FIntPoint(0, 0))) // if OccupiedTile is not the default
		AGridManager::GetGrid(GetWorld())->Find(GridOccupyComponent->OccupiedTile)->OccupiedBy = nullptr; // delete from old tile

	// add on new tile
	AGridManager::GetGrid(GetWorld())->Find(NewTile)->OccupiedBy = this;
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

 
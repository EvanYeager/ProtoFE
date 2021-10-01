#include "Tile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "ConstructorHelpers.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshObject(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	if (PlaneMeshObject.Succeeded())
		PlaneMesh = PlaneMeshObject.Object;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = Root;

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	Plane->SetupAttachment(Root);
	Plane->SetStaticMesh(PlaneMesh);
	Plane->SetRelativeScale3D(FVector(1, 1, .06f));

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	Plane->SetVisibility(false);
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


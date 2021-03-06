#include "TileActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/BoxComponent.h"

// Sets default values
ATileActor::ATileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshObject(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	if (PlaneMeshObject.Succeeded())
		PlaneMesh = PlaneMeshObject.Object;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = Root;

	HighlightPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Highlight Plane"));
	HighlightPlane->SetupAttachment(Root);
	HighlightPlane->SetStaticMesh(PlaneMesh);
	HighlightPlane->SetRelativeScale3D(FVector(1, 1, .06f));

	EnemyRangePlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Enemy Range Plane"));
	EnemyRangePlane->SetupAttachment(Root);
	EnemyRangePlane->SetStaticMesh(PlaneMesh);
	EnemyRangePlane->SetRelativeScale3D(FVector(1, 1, .06f));
	EnemyRangePlane->SetVisibility(false);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Root);
	Collision->InitBoxExtent(FVector(60, 60, .01));
	Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	Collision->bHiddenInGame = false;
}

// Called when the game starts or when spawned
void ATileActor::BeginPlay()
{
	Super::BeginPlay();

	HighlightPlane->SetVisibility(false);
 	HighlightMaterial = HighlightPlane->CreateDynamicMaterialInstance(0, HighlightPlane->GetMaterial(0));
	
}

// Called every frame
void ATileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileActor::SetColor(EHighlightColor HighlightColor)
{
	if (!HighlightMaterial) return;
	FLinearColor color = DefaultHighlight;
	switch (HighlightColor)
	{
	case EHighlightColor::EnemyRange:
		color = EnemyRange;
		break;
	case EHighlightColor::RedHighlight:
		color = RedHighlight;
		break;
	case EHighlightColor::BlueHighlight:
		color = BlueHighlight;
		break;
	
	default:
		break;
	}
	HighlightMaterial->SetVectorParameterValue("Color", color);
}

void ATileActor::SetStrength(EHighlightStrength Strength)
{
	if (!HighlightMaterial) return;
	float Opacity = NormalStrength;
	switch (Strength)
	{
	case EHighlightStrength::Strong:
		Opacity = StrongStrength;
		break;
	case EHighlightStrength::Weak:
		Opacity = WeakStrength;
		break;
	
	default:
		break;
	}
	HighlightMaterial->SetScalarParameterValue("Strength", Opacity);
}

void ATileActor::AddColorLayer(EHighlightColor Color, EHighlightStrength Strength)
{
	Layers.Add(FColorLayer(Color, Strength));
	SwitchToTopLayer();
}
	
void ATileActor::RemoveColorLayer()
{
	if (Layers.Num() == 0) return;
	Layers.RemoveAt(Layers.Num() - 1);
	SwitchToTopLayer();
}

void ATileActor::SwitchToTopLayer()
{
	if (Layers.Num() == 0) // if there aren't any layers, make it invisible
	{
		HighlightPlane->SetVisibility(false);
		return;
	}

	HighlightPlane->SetVisibility(true);
	SetColor(Layers.Last().Color);
	SetStrength(Layers.Last().Strength);
}

void ATileActor::SetAsEnemyRange()
{
	EnemyRangePlane->SetVisibility(true);
}

void ATileActor::ResetAsEnemyRange()
{
	EnemyRangePlane->SetVisibility(false);
}
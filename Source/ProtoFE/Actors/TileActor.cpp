#include "TileActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

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

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	Plane->SetupAttachment(Root);
	Plane->SetStaticMesh(PlaneMesh);
	Plane->SetRelativeScale3D(FVector(1, 1, .06f));
}

// Called when the game starts or when spawned
void ATileActor::BeginPlay()
{
	Super::BeginPlay();

	Plane->SetVisibility(false);
 	Material = Plane->CreateDynamicMaterialInstance(0, Plane->GetMaterial(0));
	
}

// Called every frame
void ATileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileActor::SetColor(EHighlightColor Color)
{
	if (!Material) return;
	FLinearColor _Color = DefaultHighlight;
	switch (Color)
	{
	case EHighlightColor::EnemyRange:
		_Color = EnemyRange;
		break;
	case EHighlightColor::RedHighlight:
		_Color = RedHighlight;
		break;
	case EHighlightColor::BlueHighlight:
		_Color = BlueHighlight;
		break;
	
	default:
		break;
	}
	Material->SetVectorParameterValue("Color", _Color);
	SetStrength(EHighlightStrength::Normal); // reset to normal strength; this might not be the best way to do this
}

void ATileActor::SetStrength(EHighlightStrength Strength)
{
	if (!Material) return;
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
	Material->SetScalarParameterValue("Strength", Opacity);
}
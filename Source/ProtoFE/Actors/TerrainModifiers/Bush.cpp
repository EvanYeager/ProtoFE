#include "Bush.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ABush::ABush()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Terrain = ETerrain::Foliage;

	ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshObj(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Bush.SM_Bush'"));
	if (StaticMeshObj.Succeeded())
		Mesh->SetStaticMesh(StaticMeshObj.Object);
	
}

// Called when the game starts or when spawned
void ABush::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABush::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


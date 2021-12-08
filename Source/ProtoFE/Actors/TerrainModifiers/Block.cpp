
#include "Block.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "ConstructorHelpers.h"

ABlock::ABlock() 
{
   Collision->SetBoxExtent(FVector(50));

   Terrain = ETerrain::Impossible;

   ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshObj(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube_Chamfer.1M_Cube_Chamfer'"));
	if (StaticMeshObj.Succeeded())
		Mesh->SetStaticMesh(StaticMeshObj.Object);

   Mesh->SetRelativeLocation(FVector(0, 0, Mesh->GetStaticMesh()->GetBoundingBox().GetSize().Z / 2)); // set the mesh to be on top of the scene component
}

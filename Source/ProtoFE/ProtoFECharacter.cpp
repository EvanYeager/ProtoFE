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


AProtoFECharacter::AProtoFECharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

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

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	ConstructorHelpers::FClassFinder<UUserWidget> StatsWindow(TEXT("WidgetBlueprint'/Game/TopDownCPP/Blueprints/Widgets/CursorOver_StatsWindow'"));
	if (StatsWindow.Succeeded())
		StatsWindowClass = StatsWindow.Class;


}

void AProtoFECharacter::BeginPlay() 
{
	Super::BeginPlay();
	// on cursor over
	// GetMesh()->OnBeginCursorOver.AddDynamic(this, &AProtoFECharacter::DisplayStats);
	// GetMesh()->OnEndCursorOver.AddDynamic(this, &AProtoFECharacter::RemoveStats);

	GetMesh()->OnBeginCursorOver.AddDynamic(this, &AProtoFECharacter::DisplayStats);
	GetMesh()->OnEndCursorOver.AddDynamic(this, &AProtoFECharacter::RemoveStats);
}

void AProtoFECharacter::Tick(float DeltaSeconds)
{
   Super::Tick(DeltaSeconds);

	
}


void AProtoFECharacter::DisplayStats(UPrimitiveComponent* comp) 
{
	if (!StatsWindowClass) return;
	if (AProtoFEPlayerController* Controller = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		StatsWindow = Cast<UStatsWindowParent>(Controller->DisplayWidget(StatsWindowClass));
		if (!StatsWindow) return;
		StatsWindow->Info = Information; // sets data to be displayed in widget
	}
}

void AProtoFECharacter::RemoveStats(UPrimitiveComponent* comp)
{
	if (AProtoFEPlayerController* Controller = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		Controller->RemoveWidget(StatsWindow);
	}
}
 
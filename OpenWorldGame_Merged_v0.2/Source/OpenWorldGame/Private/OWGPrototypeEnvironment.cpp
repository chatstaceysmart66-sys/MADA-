#include "OWGPrototypeEnvironment.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AOWGPrototypeEnvironment::AOWGPrototypeEnvironment()
{
    PrimaryActorTick.bCanEverTick = false;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    Ground = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground"));
    Ground->SetupAttachment(SceneRoot);

    MainRoad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainRoad"));
    MainRoad->SetupAttachment(SceneRoot);

    BuildingA = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingA"));
    BuildingA->SetupAttachment(SceneRoot);
    BuildingB = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingB"));
    BuildingB->SetupAttachment(SceneRoot);
    BuildingC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingC"));
    BuildingC->SetupAttachment(SceneRoot);

    if (CubeMesh.Succeeded())
    {
        Ground->SetStaticMesh(CubeMesh.Object);
        MainRoad->SetStaticMesh(CubeMesh.Object);
        BuildingA->SetStaticMesh(CubeMesh.Object);
        BuildingB->SetStaticMesh(CubeMesh.Object);
        BuildingC->SetStaticMesh(CubeMesh.Object);
    }

    Ground->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));
    Ground->SetRelativeScale3D(FVector(150.0f, 150.0f, 1.0f));
    Ground->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Ground->SetCollisionObjectType(ECC_WorldStatic);
    Ground->SetCollisionResponseToAllChannels(ECR_Block);

    MainRoad->SetRelativeLocation(FVector(0.0f, 0.0f, 59.0f));
    MainRoad->SetRelativeScale3D(FVector(150.0f, 3.5f, 0.12f));
    MainRoad->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MainRoad->SetCollisionObjectType(ECC_WorldStatic);
    MainRoad->SetCollisionResponseToAllChannels(ECR_Block);

    BuildingA->SetRelativeLocation(FVector(1000.0f, 850.0f, 250.0f));
    BuildingA->SetRelativeScale3D(FVector(5.0f, 5.0f, 6.0f));
    BuildingA->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BuildingA->SetCollisionResponseToAllChannels(ECR_Block);

    BuildingB->SetRelativeLocation(FVector(1550.0f, -900.0f, 180.0f));
    BuildingB->SetRelativeScale3D(FVector(6.0f, 6.0f, 4.5f));
    BuildingB->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BuildingB->SetCollisionResponseToAllChannels(ECR_Block);

    BuildingC->SetRelativeLocation(FVector(-1200.0f, 950.0f, 320.0f));
    BuildingC->SetRelativeScale3D(FVector(4.5f, 4.5f, 7.5f));
    BuildingC->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BuildingC->SetCollisionResponseToAllChannels(ECR_Block);
}

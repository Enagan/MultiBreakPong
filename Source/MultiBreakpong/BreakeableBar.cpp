// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiBreakpong.h"
#include "BreakeableBar.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABreakeableBar::ABreakeableBar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
  // The Break bar replicates to clients, using simple actor replication, since it does not contain any special behaviours
  SetReplicates(true);
  // Since the breakbars are instanced by the spawn volume, we set the spawn collision handler to ensure that we never spawn two bars overlapping
  SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

  // Fetch required mesh and material
  static ConstructorHelpers::FObjectFinder<UStaticMesh> BreakBarMesh(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"));
  static ConstructorHelpers::FObjectFinder<UMaterialInterface> BreakBarGlassMaterial(TEXT("/Game/Materials/ColorableGlass.ColorableGlass"));

  // Create the mesh component
  BreakeableBarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BreakeableBarMesh"));
  RootComponent = BreakeableBarMesh;
  BreakeableBarMesh->SetStaticMesh(BreakBarMesh.Object);
  BreakeableBarMesh->SetWorldScale3D(FVector(0.6, 2.0, 0.4));
  BreakeableBarMesh->OnComponentHit.AddDynamic(this, &ABreakeableBar::OnHit);		// set up a notification for when this component hits something
  BreakeableBarMesh->SetNotifyRigidBodyCollision(true);

  // Create the dynamic glass material, for property changing
  DynamicMaterial = UMaterialInstanceDynamic::Create(BreakBarGlassMaterial.Object, nullptr);
  BreakeableBarMesh->SetMaterial(0, DynamicMaterial);
  // Paint the glass material to be of a random color
  // This color is purposefully not replicated. Since it's a visual feature, it's not a big problem if different clients get a different color for the same block
  FLinearColor RandomColor(UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f), 
                            UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f), 
                            UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f));
  DynamicMaterial->SetVectorParameterValue(FName(TEXT("ColorGlass")), RandomColor);
}

void ABreakeableBar::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
  Destroy();
}


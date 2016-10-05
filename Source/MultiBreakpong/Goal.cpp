// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiBreakpong.h"
#include "Goal.h"


// Sets default values
AGoal::AGoal() : WhereToReSpawnAfterGoal(0.0f, 0.0f, 20.0f), DirectionToMoveAfterGoal(1.0f, 0.0f, 0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

  // Fetch required resources
  static ConstructorHelpers::FObjectFinder<UStaticMesh> GoalMeshObject(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"));

  // Create the mesh component
  GoalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GoalMesh"));
  RootComponent = GoalMesh;
  GoalMesh->SetStaticMesh(GoalMeshObject.Object);
  GoalMesh->SetWorldScale3D(FVector(20.0, 1.0, 2.25));
  // Goals, obviously, don't move
  GoalMesh->SetMobility(EComponentMobility::Static);
}
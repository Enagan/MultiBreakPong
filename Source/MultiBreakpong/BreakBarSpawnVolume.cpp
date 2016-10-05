// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiBreakpong.h"
#include "BreakBarSpawnVolume.h"
#include "BreakeableBar.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABreakBarSpawnVolume::ABreakBarSpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

  // BreakBarSpawnVolume is a server only class, as it does not make sense for clients to know how to spawn new objects.
  // Nor would it be usefull to transmit an instance of this class to them.
  bReplicates = false;
  bNetLoadOnClient = false;

  // Create the Box Component to represent the spawn volume
  WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
  RootComponent = WhereToSpawn;

  // Set the spawn delay range
  SpawnDelayRangeLow = 2.0f;
  SpawnDelayRangeHigh = 4.5f;
}

FVector ABreakBarSpawnVolume::GetRandomPointInVolume()
{
  FVector SpawnOrigin = WhereToSpawn->Bounds.Origin;
  FVector SpawnExtent = WhereToSpawn->Bounds.BoxExtent;

  return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

void ABreakBarSpawnVolume::SetSpawningActive(bool bShouldSpawn)
{
  if (bShouldSpawn)
  {
    // Set the timer on Spawn Pickup
    SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
    GetWorldTimerManager().SetTimer(SpawnTimer, this, &ABreakBarSpawnVolume::SpawnBreakBar, SpawnDelay, false);
  }
  else
  {
    // clear the timer on Spawn Pickup
    GetWorldTimerManager().ClearTimer(SpawnTimer);
  }
}

void ABreakBarSpawnVolume::SpawnBreakBar()
{
  // If we have set something to spawn:
  if (WhatToSpawn != nullptr)
  {
    // Check for a valid World: 
    UWorld* const World = GetWorld();
    if (World)
    {
      // Set the spawn parameters
      FActorSpawnParameters SpawnParams;
      SpawnParams.Owner = this;
      SpawnParams.Instigator = Instigator;

      // Get a random location to spawn at
      FVector SpawnLocation = GetRandomPointInVolume();
      
      // spawn the breakbar
      ABreakeableBar* const SpawnedPickup = World->SpawnActor<ABreakeableBar>(WhatToSpawn, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

      // Re-schedule timer
      SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
      GetWorldTimerManager().SetTimer(SpawnTimer, this, &ABreakBarSpawnVolume::SpawnBreakBar, SpawnDelay, false);
    }
  }
}


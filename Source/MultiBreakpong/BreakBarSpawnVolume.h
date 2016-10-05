// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BreakBarSpawnVolume.generated.h"

// The ABreakBarSpawnVolume class is reponsible to spawn the Brakout like blocks in the middle of the arena
// Due to it's responsibility, it's a class that only exists on the server (it isn't replicated to clients), 
// however the blocks it spawns will replicate back to the clients, ensuring both player see all generated blocks
UCLASS()
class MULTIBREAKPONG_API ABreakBarSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ABreakBarSpawnVolume();

  // This function toggles whether or not the spawner volume is actively spawning break bars
  void SetSpawningActive(bool bShouldSpawn);

private:
  // Find a random point within the BoxComponent
  UFUNCTION(BlueprintPure, Category = "BreakBarSpawnVolume")
  FVector GetRandomPointInVolume();

  // Handle spawning a new pickup
  void SpawnBreakBar();

protected:
  // The class of Breakeable bar to spawn, ready for any kind of future subclass with more behaviour
  UPROPERTY(EditAnywhere, Category = "BreakBarSpawnVolume")
  TSubclassOf<class ABreakeableBar> WhatToSpawn;

  // Timer kept to pace out break bar spawns
  FTimerHandle SpawnTimer;

  // Minimum spawn delay
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BreakBarSpawnVolume")
  float SpawnDelayRangeLow;

  // Maximum spawn delay
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BreakBarSpawnVolume")
  float SpawnDelayRangeHigh;

private:
  // Box Component to specify where pickups should be spawned
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BreakBarSpawnVolume", meta = (AllowPrivateAccess = "true"))
  class UBoxComponent* WhereToSpawn;
	
  // The current spawn delay, amount of time between break bar spawns
  float SpawnDelay;
};

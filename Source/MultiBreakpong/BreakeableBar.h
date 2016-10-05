// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BreakeableBar.generated.h"

// The breakeable bar is a breakout like volume, that self destroys when hit by the ball
UCLASS()
class MULTIBREAKPONG_API ABreakeableBar : public AActor
{
	GENERATED_BODY()
	
public:	
	ABreakeableBar();

private:
  // Function to handle logic related to collisions, the bar will self destroy upon colliding with something
  UFUNCTION()
  void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
  // Mesh used to represent the BreakBar
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BreakeableBar", meta = (AllowPrivateAccess = "true"))
  class UStaticMeshComponent* BreakeableBarMesh;

  // Dynamic material, to allow us to easily change the material color for a bit more variety on the play field
  UPROPERTY(VisibleAnywhere, Category = "BreakeableBar")
  UMaterialInstanceDynamic* DynamicMaterial;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Goal.generated.h"

// The Goal is a very simple class that represents the mesh that, when colliding with the ball, will end up as a goal event
// The goal solely contains the position and direction the ball should take when repositioning itself for the next play event
UCLASS()
class MULTIBREAKPONG_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal();

  // Public inline getters for the defined spawn position and directions.
  // The ball will call these upon collision with a goal
  FORCEINLINE const FVector& GetWhereToReSpawnAfterGoal() const { return WhereToReSpawnAfterGoal; }
  FORCEINLINE const FVector& GetDirectionToMoveAfterGoal() const { return DirectionToMoveAfterGoal; }

protected:
  // Properties to define the spawn position and direction of the ball that hits this goal
  // These properties are changed on the editor
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal", meta = (AllowPrivateAccess = "true"))
  FVector WhereToReSpawnAfterGoal;
	
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal", meta = (AllowPrivateAccess = "true"))
  FVector DirectionToMoveAfterGoal;

private:
  // Mesh used to represent the Goal
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Goal", meta = (AllowPrivateAccess = "true"))
  class UStaticMeshComponent* GoalMesh;
};

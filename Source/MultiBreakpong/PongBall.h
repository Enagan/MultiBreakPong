// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PongBall.generated.h"

// The Pong Ball, as the name indicates, is the class that controls the ball boucning around the playing field
// It's responsible for it's own movement, for the special bounce it takes when hitting a PongStickPawn,
// and to reposition itself after a goal (based on information from the goal class)
UCLASS()
class MULTIBREAKPONG_API APongBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APongBall();
  
  // Start Moving function is called by the Game Mode when it determines the game must begin and the ball should start moving
  // It's a server only function as this kind of directive should only matter for the authority of this object, since movemet will be replicated
  // by the ProjectileMovementComponent
  UFUNCTION(Reliable, Server, WithValidation, Category = "PongBall")
  void StartMoving();
  void StartMoving_Implementation();
  bool StartMoving_Validate();

private:
  // Function to handle logic related to ball collisions
  UFUNCTION()
  void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

  // After hitting a goal, the ball must respawn. It will wait a bit after respawning before starting to move again.
  // This function triggers that movement
  void ApplyVelocityOnRespawn();

protected:
  // Regular ball speed
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PongBall", meta = (AllowPrivateAccess = "true"))
  float fBallSpeed;

  // Time to wait after a goal before resuming movement
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PongBall", meta = (AllowPrivateAccess = "true"))
  float fGraceTimeAfterGoal;

  // Timer handle to manage the respawn grace period before movement
  FTimerHandle ReSpawnTimer;

  // Set movement direction to use when ApplyVelocityOnRespawn or when StartMoving
  FVector MovementDirectionAfterSpawn;

private:
  // Mesh for the pong Ball
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PongBall", meta = (AllowPrivateAccess = "true"))
  class UStaticMeshComponent* BallMesh;
	
  // Projectile Movement for the Pong Ball
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PongBall", meta = (AllowPrivateAccess = "true"))
  class UProjectileMovementComponent* ProjectileMovement;
};

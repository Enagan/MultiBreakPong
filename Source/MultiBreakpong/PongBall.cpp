// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiBreakpong.h"
#include "PongBall.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PongStickPawn.h"
#include "Goal.h"
#include "EngineUtils.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APongBall::APongBall() : fBallSpeed(1000.0f), fGraceTimeAfterGoal(2.5f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
  // We want the ball to replicate to it's heart content
  SetReplicateMovement(true);
  SetReplicates(true);

  // Fetch required resources
  static ConstructorHelpers::FObjectFinder<UStaticMesh> BallBaseMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
  static ConstructorHelpers::FObjectFinder<UMaterialInterface> BallBaseMaterial(TEXT("/Game/TwinStick/Meshes/OrangeMaterial.OrangeMaterial"));

  // Apply Mesh and basic mesh properties
  BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
  RootComponent = BallMesh;
  BallMesh->SetStaticMesh(BallBaseMesh.Object);
  BallMesh->SetMaterial(0, BallBaseMaterial.Object);

  // Prepare the OnHit notification
  BallMesh->SetNotifyRigidBodyCollision(true);
  BallMesh->OnComponentHit.AddDynamic(this, &APongBall::OnHit);	
  
  // Use a ProjectileMovementComponent to govern the ball movement
  ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
  ProjectileMovement->UpdatedComponent = BallMesh;
  ProjectileMovement->InitialSpeed = 0.0f;
  ProjectileMovement->MaxSpeed = fBallSpeed;
  ProjectileMovement->bRotationFollowsVelocity = true;
  ProjectileMovement->bShouldBounce = true;
  ProjectileMovement->Bounciness = 1.0f;
  ProjectileMovement->Friction = 0.0f;
  ProjectileMovement->BounceVelocityStopSimulatingThreshold = 0.01f;
  ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity

  // We want a physics simulationo, but we don't want gravity, and we want to restrict movement to a plane
  BallMesh->SetSimulatePhysics(true);
  BallMesh->SetEnableGravity(false);
  BallMesh->SetConstraintMode(EDOFMode::XYPlane);
}

void APongBall::StartMoving_Implementation()
{
  // Prepare MovementDirectionAfterSpawn, to randomly start either towards the blue player or the orange player
  bool RandomInitialDirection = UKismetMathLibrary::RandomBool();
  MovementDirectionAfterSpawn = FVector(RandomInitialDirection ? 1.0 : -1.0, 0.0, 0.0);

  // Schedule an ApplyVelocity so the beggining of the game isn't so sudden
  GetWorldTimerManager().SetTimer(ReSpawnTimer, this, &APongBall::ApplyVelocityOnRespawn, fGraceTimeAfterGoal, false);
}

bool APongBall::StartMoving_Validate()
{
  // Not much to validate really
  return true;
}

void APongBall::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
  // On a Collision, we first check if we hit a Pawn
  APongStickPawn* const TestIfPawn = Cast<APongStickPawn>(OtherActor);
  if (TestIfPawn)
  {
    // If so, we get the pawns center position and we get a direction vector from it to the center of the ball
    FVector CenterToCenterVec = GetActorLocation() - OtherActor->GetActorLocation();
    CenterToCenterVec.Normalize();

    // We then apply this direction vector multiplied by our speed to the ball
    // This gives the pawn some control over the direction of the ball, by how far from center it chooses to hit the ball from
    // Just like pong
    BallMesh->SetPhysicsLinearVelocity(CenterToCenterVec*fBallSpeed);
    
    // For added bells & whistles, the ball takes over the material of the pawn, so it will changes colors as it goes back and forth between the pawns
    BallMesh->SetMaterial(0, OtherComp->GetMaterial(0));

    // We're done with this collision event!
    return;
  }

  // If we didn't hit a pawn, then we might have hit a goal. Check for that.
  AGoal* const TestIfGoal = Cast<AGoal>(OtherActor);
  if (TestIfGoal)
  {
    // If so, we move the ball to the position to respawn at after goal, as dictated by the goal itself
    BallMesh->SetAllPhysicsPosition(TestIfGoal->GetWhereToReSpawnAfterGoal());

    // We kill all kind of velocity
    BallMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
    BallMesh->SetPhysicsAngularVelocity(FVector::ZeroVector);

    // We get the direction of movement to take after respawn
    MovementDirectionAfterSpawn = TestIfGoal->GetDirectionToMoveAfterGoal();

    // And we schedule an apply velocity, so that players have some breathing room between goals
    GetWorldTimerManager().SetTimer(ReSpawnTimer, this, &APongBall::ApplyVelocityOnRespawn, fGraceTimeAfterGoal, false);
  }
}

void APongBall::ApplyVelocityOnRespawn()
{
  // Simply multiply the vector by the ballspeed
  BallMesh->SetPhysicsLinearVelocity(MovementDirectionAfterSpawn*fBallSpeed);
}


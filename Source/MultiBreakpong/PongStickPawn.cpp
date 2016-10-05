// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiBreakpong.h"
#include "PongStickPawn.h"

// Sets default values
APongStickPawn::APongStickPawn() : fMoveSpeed(1000.0f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
  // We want the pawns to replicate as much as possible
  bReplicates = true;
  bReplicateMovement = true;
  bAlwaysRelevant = true;

  // Create the mesh component
  static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"));
  PongStickMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PongStickMesh"));
  RootComponent = PongStickMesh;
  PongStickMesh->SetStaticMesh(CubeMesh.Object);
  PongStickMesh->SetWorldScale3D(FVector(0.6, 3.5, 0.4));
  PongStickMesh->SetIsReplicated(true);
}


// Called every frame
void APongStickPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
  
  // Consumes the pending direction vector, setting it to zero
  const FVector MoveDirection = ConsumeMovementInputVector();

  // Calculate  movement based on speed and direction
  const FVector Movement = MoveDirection * fMoveSpeed * DeltaTime;

  // If non-zero size, move this actor
  if (Movement.SizeSquared() > 0.0f)
  {
    FHitResult Hit(1.f);
    PongStickMesh->MoveComponent(Movement, FRotator::ZeroRotator, true, &Hit);

    if (Hit.IsValidBlockingHit())
    {
      const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
      const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
      PongStickMesh->MoveComponent(Deflection, FRotator::ZeroRotator, true);
    }
  }
}

// Called to bind functionality to input
void APongStickPawn::SetupPlayerInputComponent(UInputComponent* PongStickInputComponent)
{
	Super::SetupPlayerInputComponent(PongStickInputComponent);
  PongStickInputComponent->BindAxis("MoveLaterally", this, &APongStickPawn::GetAxisInput);
}

void APongStickPawn::GetAxisInput(float Value)
{
  // If the axis input is valid we move forward
  if (Value != 0.0f)
  {
    // If we're not the authority over this pawn (i.e. we´re a client) then we need to both call the RPC to server.
    // We also update locally because movement is quite deterministic, so we most likely can very accurately predict where we'll end up
    // before the server broadcasts his result
    if (Role != ROLE_Authority)
    {
      MoveLaterally(Value);
      MoveLaterally_Implementation(Value);
    }
    else // If we´re the server, we don't need to RPC anything
    {
      MoveLaterally(Value);
    }
  }
}

void APongStickPawn::MoveLaterally_Implementation(float Value)
{
  if ((Controller != NULL) && (Value != 0.0f))
  {
    // find out which way is forward
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    // Get lateral movement vector
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
  }
}

bool APongStickPawn::MoveLaterally_Validate(float Value)
{
  // Since this is just a test project, no point in writting complex validation
  // "I trust the user" - Famous last words
  return true;
}


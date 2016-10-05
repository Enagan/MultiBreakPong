// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PongStickPawn.generated.h"

// The Pong stick pawn is the class responsible for modelling the controllable pong stick at each end of the play area.
// They will be controlled by the players and their logic pertains to their movement
UCLASS()
class MULTIBREAKPONG_API APongStickPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APongStickPawn();

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

private:
  // Get Player input to determine if movement happened
  void GetAxisInput(float Value);

  // RPC to server upon valid player input.
  // Clients naturally need to inform the server of a local input so that it can correctly update and propagate positions
  UFUNCTION(Reliable, Server, WithValidation, Category = "PongStickPawn")
  void MoveLaterally(float Value);
  void MoveLaterally_Implementation(float Value);
  bool MoveLaterally_Validate(float Value);

protected:
  // The speed the pong stick can move at
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PongStickPawn")
  float fMoveSpeed;

private:
  // Mesh used to represent the Pong Stick
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PongStickPawn", meta = (AllowPrivateAccess = "true"))
  class UStaticMeshComponent* PongStickMesh;
};

// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/GameMode.h"
#include "MultiBreakpongGameMode.generated.h"

// The gamemode is responsible for controlling joining players, and when to trigger the start of the game
UCLASS(minimalapi)
class AMultiBreakpongGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMultiBreakpongGameMode();

protected:
  // When called, will search the level for the objects to connect to the control variables
  void AttachWorldObjects();

  // Overrides to control behaviour when new players join the game (both locally and over the network)
  virtual void PreLogin(const FString & Options, const FString & Address, const TSharedPtr < const FUniqueNetId > & UniqueId, FString & ErrorMessage) override;
  virtual void PostLogin(APlayerController * NewPlayer) override;

private:
  // Variables to connect to Level objects, in order to allow the game mode to control game flow
  TArray<class APongStickPawn*> PlayerPawns;
  class ACameraActor* OverheadCamera;
  class APongBall* TheBall;
  class ABreakBarSpawnVolume* BreakBarSpawnVolume;
};




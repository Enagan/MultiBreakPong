// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MultiBreakpong.h"
#include "MultiBreakpongGameMode.h"
#include "PongStickPawn.h"
#include "PongBall.h"
#include "BreakBarSpawnVolume.h"
#include "EngineUtils.h"

AMultiBreakpongGameMode::AMultiBreakpongGameMode() : OverheadCamera(nullptr), TheBall(nullptr), BreakBarSpawnVolume(nullptr)
{
	// set default pawn class to our character class
	DefaultPawnClass = APongStickPawn::StaticClass();
  bStartPlayersAsSpectators = true;

  // Initialize the player pawn array to size two (only two pawns in the level)
  PlayerPawns.Init(nullptr, 2);
}

void AMultiBreakpongGameMode::PreLogin(const FString & Options, const FString & Address, const TSharedPtr < const FUniqueNetId > & UniqueId, FString & ErrorMessage)
{
  // If more than two players join the game, they will be rejected. This is a two player game.
  if (NumPlayers > 2)
  {
    ErrorMessage = "Player Rejected: Game Full";
  }
}

void AMultiBreakpongGameMode::PostLogin(APlayerController * NewPlayer) 
{
  Super::PostLogin(NewPlayer);

  // Since PostLogin can run before BeginPlay for the local player, we attach to the world objects here instead of on BeginPlay, 
  // otherwise the first player wont have the attachment variables valid
  if (PlayerPawns[0] == nullptr || PlayerPawns[1] == nullptr)
  {
    AttachWorldObjects();
  }

  // If the player controller was already attached to a pawn, we get it, and delete it
  APawn* previousPawn = NewPlayer->GetPawn();
  if (previousPawn != nullptr)
  {
    previousPawn->Destroy();
  }

  // We make the player controller possess the respective pawn, using NumPlayers to acess the correct pawn in the Array
  NewPlayer->Possess(PlayerPawns[NumPlayers-1]);
  // We set the player controller to view the world through the overhead camera. Both players will use this view
  NewPlayer->SetViewTargetWithBlend(OverheadCamera);

  // If Both players are in, start game (by making the ball move, and activating the BreakBar spawner)
  if (NumPlayers == 2 && BreakBarSpawnVolume != nullptr && TheBall != nullptr)
  {
    BreakBarSpawnVolume->SetSpawningActive(true);
    TheBall->StartMoving();
  }
}


void AMultiBreakpongGameMode::AttachWorldObjects()
{
  // Find Player Pawns
  for (TActorIterator<APongStickPawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
  {
    if (ActorItr->GetName() == "PongStickPawn1")
    {
      PlayerPawns[0] = *ActorItr;
    }
    else if (ActorItr->GetName() == "PongStickPawn2")
    {
      PlayerPawns[1] = *ActorItr;
    }
  }

  // Find the overhead camera
  for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
  {
    if (ActorItr->GetName() == "OverheadCamera")
    {
      OverheadCamera = *ActorItr;
    }
  }

  // Find the Ball
  for (TActorIterator<APongBall> ActorItr(GetWorld()); ActorItr; ++ActorItr)
  {
    TheBall = *ActorItr;
  }

  // Find the Break Volume
  for (TActorIterator<ABreakBarSpawnVolume> ActorItr(GetWorld()); ActorItr; ++ActorItr)
  {
    BreakBarSpawnVolume = *ActorItr;
  }
}

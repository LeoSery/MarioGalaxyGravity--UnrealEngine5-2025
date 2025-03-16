#include "MMGGameMode.h"

/**
 * @brief Constructor for the game mode class.
 *
 * @details Initializes the game mode with default settings for Mario Galaxy Gravity:
 * 1. Sets the default pawn class to the Mario character or a suitable alternative
 * 2. Configures the player controller class
 * 3. May set up other game-specific defaults like HUD, game state, etc.
 * 
 * This game mode serves as the central configuration point for the Mario Galaxy
 * Gravity gameplay experience, establishing the core rules and component classes.
 */
AMMGGameMode::AMMGGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BP_Mario"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}


#include "Game.hpp"

int main()
{
    Game game;

    // Game loop
    while (game.running()){

        // update
        game.updateGame();

        // render
        game.renderGame();
    }

    return 0;
}
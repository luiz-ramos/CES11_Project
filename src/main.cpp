
#include "Game.hpp"

int main()
{
    // Initialize game
    Game game;

    // Game loop
    while (game.running()){

        // update
        game.update();

        // render
        game.render();
    }

    return 0;
}
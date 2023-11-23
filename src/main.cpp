
#include "Game.hpp"

int main()
{
    // Initialize game
//    if (FILE * saveFile = fopen("../../src/save/statsSave.txt", "r")){
//        fclose(saveFile);
//    };
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
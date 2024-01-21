#include"game.h"
int main(int argc, char* argv[]){
    init();
    while(game.run){
        input();
        update();
        render();
        framerate();
    }
    quit();
    return 0;
}

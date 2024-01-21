#include"functions.h"
struct{
    bool A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
         _1, _2, _3, _4, _5, _6, _7, _8, _9, _0,
         UP, DOWN, LEFT, RIGHT,
         SPACE, SHIFT, ENTER, ESCAPE;
}edge;
struct item{
    char id;
    int number;
};
struct{
    struct{
        float width, height;
        SDL_Texture* texture;
    }null_entity;
    struct{
        float width, height;
        SDL_Texture* texture;
    }monster;
}type;
struct{
    float x, y, tx, ty, dx, dy, width, height, speed;
    struct item inventory[32];
    SDL_Texture* texture;
}player;
typedef struct{
    word type;
    float x, y;
}null_entity;
typedef struct{
    word type;
    float x, y;
}monster;
word entity_counter;
void* entity[entity_max];
void create_entity(int type){
    if(entity_counter >= entity_max) return;
    int typesize;
    switch(type){
    case 0:
        typesize = sizeof(null_entity);
        break;
    case 1:
        typesize = sizeof(monster);
        break;
    }
    entity[entity_counter] = malloc(typesize);
    *(word*)entity[entity_counter] = type;
    entity_counter++;
}
void destroy_entity(int id){
    if(id < 0) return;
    free((void*)entity[id]);
    for(int i = id; i < entity_counter; ++i){
        entity[i] = entity[i+1];
    }
    entity_counter--;
}
void init_objects(){
    player.x = 0;
    player.y = 0;
    player.width = 64;
    player.height = 64;
    player.speed = 2;
    player.texture = load_png("assets/face.png");
    type.null_entity.texture = load_png("assets/null.png");
    type.monster.texture = load_png("assets/monster.png");
}
void parse_event(){
    SDL_Event event;
    while(SDL_PollEvent(&event) > 0){
        switch(event.type){
        case SDL_QUIT:
            game.run = false;
        break;
        }
    }
}
void parse_key(){
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if(keystate[SDL_SCANCODE_ESCAPE]){
        game.run = false;
    }
    if(keystate[SDL_SCANCODE_D]){
        player.tx += 1;
    }
    if(keystate[SDL_SCANCODE_A]){
        player.tx -= 1;
    }
    if(keystate[SDL_SCANCODE_S]){
        player.ty += 1;
    }
    if(keystate[SDL_SCANCODE_W]){
        player.ty -= 1;
    }
    if(keystate[SDL_SCANCODE_N] && edge.N){
        create_entity(0);
        printf("[%3d] %4p\n", entity_counter, entity[entity_counter - 1]);
        edge.N = false;
    }
    if(!keystate[SDL_SCANCODE_N]){
        edge.N = true;
    }
    if(keystate[SDL_SCANCODE_M] && edge.M){
        create_entity(1);
        printf("[%3d] %4p\n", entity_counter, entity[entity_counter - 1]);
        edge.M = false;
    }
    if(!keystate[SDL_SCANCODE_M]){
        edge.M = true;
    }
    if(keystate[SDL_SCANCODE_B] && edge.B){
        destroy_entity(entity_counter - 1);
        edge.B = false;
    }
    if(!keystate[SDL_SCANCODE_B]){
        edge.B = true;
    }
}
void update_entities(){
    for(int i = 0; i < entity_counter; ++i){
        switch(*(word*)entity[i]){
        case 0:;
            null_entity* null_entityptr = (null_entity*)entity[i];
            move(&null_entityptr->x, &null_entityptr->y, player.x - null_entityptr->x, player.y - null_entityptr->y, 1);
            break;
        case 1:;
            monster* monsterptr = (monster*)entity[i];
            move(&monsterptr->x, &monsterptr->y, player.x - monsterptr->x, player.y - monsterptr->y, 1);
            break;
        }
    }
}
void render_entities(){
    for(int i = 0; i < entity_counter; ++i){
        switch(*(word*)entity[i]){
        case 0:;
            null_entity* null_entityptr = (null_entity*)entity[i];
            draw_texture(type.null_entity.texture, null_entityptr->x, null_entityptr->y, 64, 64);
            break;
        case 1:;
            monster* monsterptr = (monster*)entity[i];
            draw_texture(type.monster.texture, monsterptr->x, monsterptr->y, 64, 64);
        }
    }
}
void init(){
    frame.minimum = 1./60;
    game.window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_FULLSCREEN_DESKTOP);
    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);
    SDL_ShowCursor(false);
    init_objects();
    game.run = true;
}
void input(){
    parse_event();
    parse_key();
    SDL_GetWindowPosition(game.window, &screen.x, &screen.y);
    SDL_GetWindowSize(game.window, &screen.width, &screen.height);
}
void update(){
    normalise_vector(&player.tx, &player.ty, player.speed);
    player.x += player.dx + player.tx;
    player.y += player.dy + player.ty;
    player.tx = 0;
    player.ty = 0;
    update_entities();
}
void render(){
    SDL_SetRenderDrawColor(game.renderer, 0x00, 0x00, 0x00, 0);
    SDL_RenderClear(game.renderer);
    draw_texture(player.texture, player.x, player.y, player.width, player.height);
    render_entities();
    SDL_RenderPresent(game.renderer);
}
void framerate(){
    frame.time = (clock() - frame.start)/CLOCKS_PER_SEC;
    if(frame.time < frame.minimum){
        SDL_Delay((frame.minimum - frame.time)*1000);
    }
    frame.start = clock();
}
void quit(){
    SDL_DestroyWindow(game.window);
    SDL_Quit();
}

#include<stdio.h>
#include<time.h>
#include<math.h>
#include<stdlib.h>
#include<stdbool.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#define byte unsigned char
#define word unsigned short
#define dword unsigned int
#define qword unsigned long
#ifndef PI
    #define PI acos(-1)
#endif
#ifndef RAD
    #define RAD PI/180
#endif
#ifndef SQRT2
    #define SQRT2 sqrt(2)
#endif
#ifndef SQRT1_2
    #define SQRT1_2 sqrt(.5)
#endif
#define entity_max 256
struct{
    bool run;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event* event;
}game;
struct{
    unsigned start;
    float time, minimum;
}frame;
struct{
    int x, y, width, height;
}screen;
float hypot3(float x, float y, float z){
    return sqrt(x*x + y*y + z*z);
}
float xRotate(float x, float y, float degrees){
    return x*cos(degrees*RAD) - y*sin(degrees*RAD);
}
float yRotate(float x, float y, float degrees){
    return x*sin(degrees*RAD) + y*cos(degrees*RAD);
}
void rotate(float* x, float* y, float degrees){
    float y1 = yRotate(*x, *y, degrees);
    *x = xRotate(*x, *y, degrees);
    *y = y1;
}
void normalise_vector(float* x, float* y, float normal){
    if(!(*x||*y)) return;
    float mag = hypot(*x, *y);
    float coeff = normal/mag;
    *x *= coeff;
    *y *= coeff;
}
void move(float* x, float* y, float dx, float dy, float speed){
    if(!(dx||dy)) return;
    normalise_vector(&dx, &dy, speed);
    *x += dx;
    *y += dy;
}
void rectangle(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b){
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;
    SDL_SetRenderDrawColor(game.renderer, r, g, b, 0x00);
    SDL_RenderFillRect(game.renderer, &rectangle);
}
SDL_Texture* load_png(char* file){
    SDL_Surface* surface = IMG_Load(file);
    if(surface == NULL){
        surface = IMG_Load("assets/blank_texture.png");
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(game.renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
void draw_texture(SDL_Texture* texture, float x, float y, float w, float h){
    SDL_Rect rectangle = {x, y, w, h};
    SDL_RenderCopy(game.renderer, texture, NULL, &rectangle);
}

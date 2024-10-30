/*
    [X]* Make  a backgroung  for snake
    [X] Make a  Snake and move her
    [X] Collision Detection:
    [X] Food & Scoring System:
    [X]  Game Over Screen:
*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>

#define GAME_WIDTH 1000
#define SIZE_HEIGHT 600

using namespace std;

class Snake {
public:
    int x, y;
    int width, height;
    char direction;
    int speed;

    Snake(int startX, int startY, int w, int h, int s)
        : x(startX), y(startY), width(w), height(h), speed(s), direction('d') {}

    void render(SDL_Renderer *renderer) {
        SDL_Rect rect = {x, y, width, height};
        SDL_SetRenderDrawColor(renderer, 255, 0, 153, 255); // Pink color
        SDL_RenderFillRect(renderer, &rect);
    }

    void move() {
        switch (direction) {
        case 'w':
            y -= speed;
            break;
        case 's':
            y += speed;
            break;
        case 'a':
            x -= speed;
            break;
        case 'd':
            x += speed;
            break;
        }

        if (y < 0)
            y = 0;
        if (y > SIZE_HEIGHT - height)
            y = SIZE_HEIGHT - height;
        if (x < 0)
            x = 0;
        if (x > GAME_WIDTH - width)
            x = GAME_WIDTH - width;
    }
};

class Food {
public:
    int x, y;
    int size;

    Food() : size(20) {
        spawnFood();
    }

    void spawnFood() {
        x = (rand() % (GAME_WIDTH / size)) * size;
        y = (rand() % (SIZE_HEIGHT / size)) * size;
    }

    void draw(SDL_Renderer *renderer) {
        SDL_Rect foodRect = {x, y, size, size};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
        SDL_RenderFillRect(renderer, &foodRect);
    }

    bool checkFoodCollision(const Snake &snake) {
        return (snake.x == x && snake.y == y);
    }
};

void handleInput(Snake &snake) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_w:
                snake.direction = 'w';
                break;
            case SDLK_s:
                snake.direction = 's';
                break;
            case SDLK_a:
                snake.direction = 'a';
                break;
            case SDLK_d:
                snake.direction = 'd';
                break;
            case SDLK_UP:
                snake.direction = 'w';
                break;
            case SDLK_DOWN:
                snake.direction = 's';
                break;
            case SDLK_LEFT:
                snake.direction = 'a';
                break;
            case SDLK_RIGHT:
                snake.direction = 'd';
                break;
            }
        }
    }
}

void GameEnd(Snake &snake, SDL_Renderer *renderer) {
    if (snake.y <= 0 || snake.y >= SIZE_HEIGHT - snake.height || snake.x <= 0 || snake.x >= GAME_WIDTH - snake.width) {
        cout << "Game over. Try again." << endl;

        SDL_Texture *texture = IMG_LoadTexture(renderer, "/home/faizan/Downloads/Skull.jpg");
        if (!texture) {
            cout << "Error loading texture: " << IMG_GetError() << endl;
            return;
        }

        SDL_RenderClear(renderer);

        int imgW, imgH;
        SDL_QueryTexture(texture, NULL, NULL, &imgW, &imgH);

        SDL_Rect rect = {(GAME_WIDTH - imgW) / 2, (SIZE_HEIGHT - imgH) / 2, imgW, imgH};

        SDL_RenderCopy(renderer, texture, NULL, &rect);

        SDL_RenderPresent(renderer);

        SDL_Delay(3000);
        exit(0);

        SDL_DestroyTexture(texture);
    }
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "Error initializing SDL: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_UNDEFINED, GAME_WIDTH, SIZE_HEIGHT, 0);
    if (!win) {
        cout << "Error creating window: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Error creating renderer: " << SDL_GetError() << endl;
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    Snake player(40, 20, 20, 20, 20);
    Food foodItem;
    int score = 0;
    bool running = true;

    while (running) {
        handleInput(player);
        player.move();
        GameEnd(player, renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);
        player.render(renderer);
        foodItem.draw(renderer);

        if (foodItem.checkFoodCollision(player)) {
            score += 20;
            foodItem.spawnFood();
            cout << "Score: " << score << endl;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(100); // Frame rate
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}

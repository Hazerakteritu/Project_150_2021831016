#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Constants
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SNAKE_SIZE 20
#define INITIAL_SNAKE_LENGTH 3

// Structs
typedef struct {
    int x, y;
} Snack;

typedef struct {
    int x, y;
} SnakeSegment;

typedef struct {
    SnakeSegment *segments;
    int length;
    int direction; // 0: up, 1: right, 2: down, 3: left
} Snake;

// Function prototypes
void initialize();
void handleInput();
void update();
void render();
void cleanup();

// SDL variables
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// Game variables
Snake snake;
Snack snack;
int score = 0;

int main() {
    initialize();

    while (1) {
        handleInput();
        update();
        render();
        SDL_Delay(100); // Adjust the speed of the game
    }

    cleanup();
    return 0;
}

void initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Create window
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Initialize snake
    snake.length = INITIAL_SNAKE_LENGTH;
    snake.direction = 1; // Initial direction: right
    snake.segments = (SnakeSegment *)malloc(sizeof(SnakeSegment) * INITIAL_SNAKE_LENGTH);

    for (int i = 0; i < INITIAL_SNAKE_LENGTH; ++i) {
        snake.segments[i].x = SCREEN_WIDTH / 2 - i * SNAKE_SIZE;
        snake.segments[i].y = SCREEN_HEIGHT / 2;
    }

    // Initialize snack
    srand(SDL_GetTicks()); // Seed for random number generation
    snack.x = rand() % (SCREEN_WIDTH / SNAKE_SIZE) * SNAKE_SIZE;
    snack.y = rand() % (SCREEN_HEIGHT / SNAKE_SIZE) * SNAKE_SIZE;
}

void handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            fprintf(stderr, "Exiting...\n");
            exit(EXIT_SUCCESS);
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    if (snake.direction != 2)
                        snake.direction = 0;
                    break;
                case SDLK_RIGHT:
                    if (snake.direction != 3)
                        snake.direction = 1;
                    break;
                case SDLK_DOWN:
                    if (snake.direction != 0)
                        snake.direction = 2;
                    break;
                case SDLK_LEFT:
                    if (snake.direction != 1)
                        snake.direction = 3;
                    break;
            }
        }
    }
}

void update() {
    // Move the snake
    for (int i = snake.length - 1; i > 0; --i) {
        snake.segments[i] = snake.segments[i - 1];
    }

    switch (snake.direction) {
        case 0:
            snake.segments[0].y -= SNAKE_SIZE;
            break;
        case 1:
            snake.segments[0].x += SNAKE_SIZE;
            break;
        case 2:
            snake.segments[0].y += SNAKE_SIZE;
            break;
        case 3:
            snake.segments[0].x -= SNAKE_SIZE;
            break;
    }

    // Check for collision with snack
    if (snake.segments[0].x == snack.x && snake.segments[0].y == snack.y) {
        // Increase score
        score++;
        // Generate new snack location
        snack.x = rand() % (SCREEN_WIDTH / SNAKE_SIZE) * SNAKE_SIZE;
        snack.y = rand() % (SCREEN_HEIGHT / SNAKE_SIZE) * SNAKE_SIZE;
        // Increase snake length
        snake.length++;
        snake.segments =(SnakeSegment *) realloc(snake.segments, sizeof(SnakeSegment) * snake.length);
    }

    // Check for collision with walls or itself
    if (snake.segments[0].x < 0 || snake.segments[0].x >= SCREEN_WIDTH ||
        snake.segments[0].y < 0 || snake.segments[0].y >= SCREEN_HEIGHT) {
        fprintf(stderr, "Game over! Your score: %d\n", score);
        exit(EXIT_SUCCESS);
    }

    for (int i = 1; i < snake.length; ++i) {
        if (snake.segments[0].x == snake.segments[i].x && snake.segments[0].y == snake.segments[i].y) {
            fprintf(stderr, "Game over! Your score: %d\n", score);
            exit(EXIT_SUCCESS);
        }
    }
}

void render() {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render snake
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < snake.length; ++i) {
        SDL_Rect rect = {snake.segments[i].x, snake.segments[i].y, SNAKE_SIZE, SNAKE_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    // Render snack
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect snackRect = {snack.x, snack.y, SNAKE_SIZE, SNAKE_SIZE};
    SDL_RenderFillRect(renderer, &snackRect);

    // Update the window
    SDL_RenderPresent(renderer);
}

void cleanup() {
    // Clean up SDL
    free(snake.segments);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

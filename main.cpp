#include<bits/stdc++.h>
#include<SDL2/SDL.h>
using namespace std;
#include <vector>

// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 20;

// Snake structure
struct SnakeSegment {
    int x, y;
};

// Function prototypes
void render(SDL_Renderer* renderer, const std::vector<SnakeSegment>& snake, const SDL_Point& food);
void update(std::vector<SnakeSegment>& snake, SDL_Point& food, SDL_Keycode direction, bool& running);
bool checkCollision(const std::vector<SnakeSegment>& snake, int x, int y);

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window and renderer
    SDL_Window* window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Initialize variables
    std::vector<SnakeSegment> snake{{5, 5}}; // Snake starts with one segment at position (5, 5)
    SDL_Point food = {10, 10}; // Initial food position
    SDL_Keycode direction = SDLK_RIGHT; // Initial direction
    bool running = true;

    // Main game loop
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        if (direction != SDLK_DOWN)
                            direction = SDLK_UP;
                        break;
                    case SDLK_DOWN:
                        if (direction != SDLK_UP)
                            direction = SDLK_DOWN;
                        break;
                    case SDLK_LEFT:
                        if (direction != SDLK_RIGHT)
                            direction = SDLK_LEFT;
                        break;
                    case SDLK_RIGHT:
                        if (direction != SDLK_LEFT)
                            direction = SDLK_RIGHT;
                        break;
                    default:
                        break;
                }
            }
        }

        // Update game logic
        update(snake, food, direction, running);

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render objects
        render(renderer, snake, food);

        // Update screen
        SDL_RenderPresent(renderer);

        // Add a delay to control the speed of the game
        SDL_Delay(100);
    }

    // Clean up and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void render(SDL_Renderer* renderer, const std::vector<SnakeSegment>& snake, const SDL_Point& food) {
    // Render snake
    for (const auto& segment : snake) {
        SDL_Rect blockRect = {segment.x * BLOCK_SIZE, segment.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &blockRect);
    }

    // Render food
    SDL_Rect foodRect = {food.x * BLOCK_SIZE, food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &foodRect);
}

void update(std::vector<SnakeSegment>& snake, SDL_Point& food, SDL_Keycode direction, bool& running) {
    // Update snake's position based on the direction
    int headX = snake.front().x;
    int headY = snake.front().y;

    switch (direction) {
        case SDLK_UP:
            headY--;
            break;
        case SDLK_DOWN:
            headY++;
            break;
        case SDLK_LEFT:
            headX--;
            break;
        case SDLK_RIGHT:
            headX++;
            break;
        default:
            break;
    }

    // Check for collisions with food or the snake's body
    if (headX == food.x && headY == food.y) {
        // Snake ate the food, so increase its length and generate new food
        SnakeSegment newSegment = {headX, headY};
        snake.insert(snake.begin(), newSegment);
        food.x = rand() % (SCREEN_WIDTH / BLOCK_SIZE);
        food.y = rand() % (SCREEN_HEIGHT / BLOCK_SIZE);
    } else {
        // Check for collision with the snake's body
        if (checkCollision(snake, headX, headY)) {
            running = false;
            return;
        }

        // Move the snake by adding a new head and removing the tail
        SnakeSegment newHead = {headX, headY};
        snake.insert(snake.begin(), newHead);
        snake.pop_back();
    }
}

bool checkCollision(const std::vector<SnakeSegment>& snake, int x, int y) {
    // Check for collision with the snake's body
    for (size_t i = 1; i < snake.size(); ++i) {
        if (snake[i].x == x && snake[i].y == y) {
            return true;
        }
    }
    // Check if the snake collided with the screen edges
    return (x >= SCREEN_WIDTH / BLOCK_SIZE || x < 0 || y >= SCREEN_HEIGHT / BLOCK_SIZE || y < 0);
}

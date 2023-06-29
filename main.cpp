// for initializing and shutdown functions
#include <SDL2/SDL.h>
 
// for rendering images and graphics on screen
#include <SDL_image.h>
 
// for using SDL_Delay() functions
#include <SDL2/SDL_timer.h>

#include <vector>
#include <cstdint>
#include <fstream>

#define WIDTH 1200
#define HEIGHT 800
#define DELAY 40
constexpr uint64_t kRows = HEIGHT/10;
constexpr uint64_t kCols = WIDTH/10;
constexpr uint64_t kMaxSteps = 1;

class Renderer {
 public:
    Renderer() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());
            return;
        }

        SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window_, &renderer_);
    }

    ~Renderer() {
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

    void Draw(std::vector<std::vector<bool>> &world) {
        SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer_);
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);

        for (int y = 1; y < kRows; ++y) {
            for (int x = 1; x < kCols; ++x) {
                if (world[y][x]) {
                    SDL_SetRenderDrawColor(renderer_, x*2, y*2, x*2, 255);
                    SDL_Rect rect = {x*10, y*10, 10, 10};
                    SDL_RenderFillRect(renderer_, &rect);
                }
            }
        }
        
        SDL_RenderPresent(renderer_);
        SDL_Delay(DELAY);

        while (SDL_PollEvent(&event_)){
            if (event_.type == SDL_QUIT){
                quit_ = true;
            }
            if (event_.type == SDL_KEYDOWN){
                quit_ = true;
            }
            if (event_.type == SDL_MOUSEBUTTONDOWN){
                quit_ = true;
            }
        }

    }

    bool isRunning() {
        return !quit_;
    }

 private:
    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;
    SDL_Event event_;
    bool quit_ = false;

};

void UpdateWorld(std::vector<std::vector<bool>> &world, std::vector<std::vector<bool>> &next_step) {
    // Update the world
    for (size_t row = 1; row < kRows; ++row) {
        for (size_t col = 1; col < kCols; ++col) {
            uint8_t live_neighbors = 0;
            for (size_t i = row-1; i <= row+1; ++i) {
                for (size_t j = col-1; j <= col+1; ++j) {
                    if (i == row && j == col) {
                        continue;
                    }
                    live_neighbors += static_cast<uint8_t>(world[i][j]);
                }
            }
            next_step[row][col] = (live_neighbors == 3) || (live_neighbors == 2 && world[row][col]);
        }
    }

    world = next_step;
}

int main() {
    
    Renderer renderer;
    std::vector<std::vector<bool>> world(kRows+2, std::vector<bool>(kCols+2, false));
    std::vector<std::vector<bool>> next_step(kRows+2, std::vector<bool>(kCols+2, false));

    // Randomly initialize the world
    for (size_t row = 1; row < kRows; ++row) {
        for (size_t col = 1; col < kCols; ++col) {
            world[row][col] = rand() % 2;
        }
    }
    
    // Main loop
    while (renderer.isRunning()){
        UpdateWorld(world, next_step);
        renderer.Draw(world);
    }
    return 0;
}
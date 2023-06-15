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

void update_world(std::vector<std::vector<bool>> &world, std::vector<std::vector<bool>> &next_step) {
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

void print_map(std::vector<std::vector<bool>> &world) {
    std::ofstream ofs("out.txt", std::ofstream::trunc);
    std::string world_str;
    for (size_t row = 1; row < kRows; ++row) {
        for (size_t col = 1; col < kCols; ++col) {
            world_str += world[row][col] ? "x" : ".";
        }
        world_str += "\n";
    }
    ofs << world_str;
    ofs.close();
}

int main() {
    SDL_Window *window = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface * window_surface = nullptr;
    SDL_Surface * image_surface = nullptr;
    SDL_Renderer * renderer = nullptr;
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    std::vector<std::vector<bool>> world(kRows+2, std::vector<bool>(kCols+2, false));
    std::vector<std::vector<bool>> next_step(kRows+2, std::vector<bool>(kCols+2, false));

    // Randomly initialize the world
    for (size_t row = 1; row < kRows; ++row) {
        for (size_t col = 1; col < kCols; ++col) {
            world[row][col] = rand() % 2;
        }
    }
    
    /* Pauses all SDL subsystems for a variable amount of milliseconds */
    SDL_Event e;
    bool quit = false;
    
    // Main loop
    while (!quit){
        update_world(world, next_step);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        for (int y = 1; y < kRows; ++y) {
            for (int x = 1; x < kCols; ++x) {
                if (world[y][x]) {
                    SDL_SetRenderDrawColor(renderer, x*2, y*2, x*2, 255);
                    SDL_Rect rect = {x*10, y*10, 10, 10};
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }
        SDL_RenderPresent(renderer);

        SDL_Delay(DELAY);

        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }
        }
    }

    /* Frees memory */
    SDL_DestroyWindow(window);
    SDL_FreeSurface(image_surface);

    /* Shuts down all SDL subsystems */
    SDL_Quit(); 

 
   
    return 0;
}
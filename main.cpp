#include <vector>
#include <cstdint>
#include <fstream>

constexpr uint64_t kRows = 30;
constexpr uint64_t kCols = 30;
constexpr uint64_t kMaxSteps = 1000;

void print_map(std::vector<std::vector<bool>> &world) {
    std::ofstream ofs("test.txt", std::ofstream::trunc);
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

    std::vector<std::vector<bool>> world(kRows+2, std::vector<bool>(kCols+2, false));
    std::vector<std::vector<bool>> next_step(kRows+2, std::vector<bool>(kCols+2, false));

    // Randomly initialize the world
    for (size_t row = 1; row < kRows; ++row) {
        for (size_t col = 1; col < kCols; ++col) {
            world[row][col] = rand() % 2;
        }
    }
    // Update the world
    for (uint64_t step = 0; step < kMaxSteps; ++step) {
        bool has_live = false;
        
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
                has_live |= next_step[row][col];
            }
        }
        if (!has_live) {
            break;
        }
        world = next_step;
        print_map(world);
        getchar();

    }
    return 0;
}
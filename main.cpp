#include <iostream>
#include "game.hpp"
#include <random>
#include <array>

#define BOARD_ROWS 8
#define BOARD_COLS 8

int main(int argc, char** argv) 
{
    uint32_t board_rows = BOARD_ROWS;
    uint32_t board_cols = BOARD_COLS;
    std::array<uint32_t, 3> ship_arr = {2, 3, 4};

    Game game(board_rows, board_cols);

    std::random_device rand;
    std::default_random_engine rand_engine(rand());
    std::uniform_int_distribution<uint8_t> orientation_dist(0, 1);
    std::uniform_int_distribution<uint32_t> col_dist(0, board_cols - 1);
    std::uniform_int_distribution<uint32_t> row_dist(0, board_rows - 1);

    ShipOrientation orientation;
    uint32_t origin_x, origin_y, ship_len, hit_ship;
    bool success;

    for(int i = 0; i < 10; i++)
    {
        game.resetGame();
        for(int ship_idx = 0; ship_idx < ship_arr.size(); ship_idx++)
        {
            success = false;
            while(!success)
            {
                ship_len = ship_arr[ship_idx];
                orientation = (ShipOrientation)orientation_dist(rand_engine);
                origin_x = col_dist(rand_engine);
                origin_y = row_dist(rand_engine);
                success = game.placeShip(ship_len, ship_len, orientation, origin_x, origin_y);
            }
        }
        if(game.fireAt(0, 0, hit_ship))
        {
            std::cout << "Hit ship " << hit_ship << " at (0,0)" << std::endl;
        }
        std::cout << game.boardAsString() << std::endl;
    }
}
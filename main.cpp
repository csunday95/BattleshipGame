#include <iostream>
#include "game.hpp"
#include <random>
#include <array>
#include <fstream>
#include <iomanip>

#define BOARD_ROWS 8
#define BOARD_COLS 8

int main(int argc, char** argv) 
{
    uint32_t boards_to_gen = std::stoi(argv[1]);

    uint32_t board_rows = BOARD_ROWS;
    uint32_t board_cols = BOARD_COLS;
    std::array<uint32_t, 3> ship_arr = {2, 3, 4};

    Game game(board_rows, board_cols, true);

    std::random_device rand;
    std::default_random_engine rand_engine(rand());
    std::uniform_int_distribution<uint8_t> orientation_dist(0, 1);
    std::uniform_int_distribution<uint32_t> col_dist(0, board_cols - 1);
    std::uniform_int_distribution<uint32_t> row_dist(0, board_rows - 1);

    ShipOrientation orientation;
    uint32_t origin_x, origin_y, ship_len, hit_ship;
    bool success;

    //game.placeShip(1, 1, ShipOrientation::Vertical, 7, 7);

    for(uint32_t i = 0; i < boards_to_gen; i++)
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
            // std::cout << "x: " << origin_x << "; y: " << origin_y << "; len: " << ship_len;
            // std::cout << "; o: " << (int)orientation << std::endl;
        }
        game.startGame();
        //std::cout << game.boardAsString() << std::endl;
    }

    uint32_t hit;
    game.fireAt(3, 5, hit);
    SpaceStats stats = game.computeStatsForMatching(3, 6);
    std::cout << stats.games_played << std::endl;

    double* game_frac = new double[board_rows * board_cols];
    double avg_result;
    std::ofstream output_file("results.csv", std::ofstream::out);
    for(int x = 0; x < board_cols; x++)
    {
        std::cout << '|';
        for(int y = 0; y < board_rows; y++)
        {
            if(y > 0) output_file << ',';
            const SpaceStats& stats = game.getSpaceStats(x, y);
            uint32_t stat_idx = x + y * board_cols;
            uint32_t ship_total = 0;
            for(const auto& pair: stats.ship_id_count)
            {
                ship_total += pair.second;
            }
            avg_result = (double)ship_total / (double)stats.games_played;
            game_frac[stat_idx] = avg_result;
            std::cout << ' ' << avg_result << " |";
            output_file << std::setprecision(10) << avg_result;
        }
        std::cout << '\n';
        output_file << '\n';
    }

    delete[] game_frac;
}
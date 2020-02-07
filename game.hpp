#include "boardspace.hpp"
#include <cstdint>
#include <array>

class Game {
public:
    Game(uint32_t board_rows, uint32_t board_cols);
    ~Game();
    bool fireAt(uint32_t x_target, uint32_t y_target, uint32_t& hit_id);
    void resetGame();
    bool placeShip(uint32_t ship_id, uint32_t length, ShipOrientation orientation, 
                   uint32_t base_x, uint32_t base_y);
    GameStatus getGameStatus(uint32_t max_allowed_shots);
    std::string boardAsString();
private:

    inline uint32_t boardIndex(uint32_t x, uint32_t y);

    uint32_t board_rows;
    uint32_t board_cols;
    uint32_t board_size;
    uint32_t shots_taken;
    uint32_t hits_scored;
    uint32_t total_ship_spaces;
    BoardSpace* board;
};
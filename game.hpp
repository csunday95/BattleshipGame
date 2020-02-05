#include "spacetypes.hpp"
#include <cstdint>
#include <array>

class Game {
public:
    Game(uint32_t board_rows, uint32_t board_cols);
    ~Game();
    bool fire_at(uint32_t x_target, uint32_t y_target, SpaceType& type_out);
private:

    inline uint32_t boardIndex(uint32_t x, uint32_t y);

    uint32_t board_rows;
    uint32_t board_cols;
    SpaceType* board;
};
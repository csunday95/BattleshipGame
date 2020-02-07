
#include <cstdint>

enum struct ShipOrientation {
    Vertical,
    Horizontal
};

enum struct GameStatus {
    Incomplete,
    OutOfShots,
    Victory
};

struct BoardSpace {
    uint32_t ship_id;
    bool is_shot;

    char as_char()
    {
        if(ship_id)
        {
            if(is_shot)
            {
                return 'H';
            }
            return 'S';
        }
        if(is_shot)
        {
            return 'X';
        }
        return ' ';
    }
};
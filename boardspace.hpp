
#include <cstdint>
#include <unordered_map>

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

struct SpaceStats {
public:
    SpaceStats() : empty_count(0), fired_at_count(0), 
                   miss_count(0), hit_count(0), games_played(0) 
    {
        
    }

    std::unordered_map<uint32_t, uint32_t> ship_id_count;
    uint32_t empty_count;
    uint32_t fired_at_count;
    uint32_t miss_count;
    uint32_t hit_count;
    uint32_t games_played;
};
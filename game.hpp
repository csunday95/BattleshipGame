#include "boardspace.hpp"
#include <cstdint>
#include <array>
#include <vector>

class Game {
public:
    Game(uint32_t board_rows, uint32_t board_cols, bool compute_stats = false);
    ~Game();
    bool fireAt(uint32_t x_target, uint32_t y_target, uint32_t& hit_id);
    void resetGame();
    bool placeShip(uint32_t ship_id, uint32_t length, ShipOrientation orientation, 
                   uint32_t base_x, uint32_t base_y);
    bool startGame();
    GameStatus getGameStatus(uint32_t max_allowed_shots);
    std::string boardAsString();
    const SpaceStats& getSpaceStats(uint32_t x, uint32_t y);
    SpaceStats computeStatsForMatching(uint32_t x, uint32_t y);
private:

    inline uint32_t boardIndex(uint32_t x, uint32_t y);
    void populateMatchingBoards();

    uint32_t board_rows;
    uint32_t board_cols;
    bool compute_stats;
    bool first_game;
    SpaceStats* stats_board;
    uint32_t board_size;
    uint32_t shots_taken;
    uint32_t hits_scored;
    uint32_t total_ship_spaces;
    BoardSpace* board;
    std::vector<BoardSpace*> session_boards;
    BoardSpace* last_match_board;
    std::vector<BoardSpace*> last_matching_boards;
};
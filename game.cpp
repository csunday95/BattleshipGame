#include "game.hpp"
#include <sstream>

Game::Game(uint32_t board_rows, uint32_t board_cols, bool compute_stats)
    : board_rows(board_rows), board_cols(board_cols), shots_taken(0), 
    hits_scored(0), total_ship_spaces(0), compute_stats(compute_stats),
    first_game(true), board(nullptr), stats_board(nullptr), last_match_board(nullptr)
{
    board_size = board_rows * board_cols;
    board = new BoardSpace[board_size];
    if(compute_stats)
    {
        stats_board = new SpaceStats[board_size];
    }
    resetGame();
}

Game::~Game() 
{
    if(board)
    {
        delete[] board;
    }
    for(auto pBoard : session_boards)
    {
        delete[] pBoard;
    }
    if(stats_board)
    {
        delete[] stats_board;
    }
}

void Game::resetGame() {
    if(first_game)
    {
        first_game = false;
    }
    else if(compute_stats)
    {
        session_boards.push_back(board);
        board = new BoardSpace[board_size];
    }
    for(int idx = 0; idx < board_size; idx++)
    {
        board[idx].ship_id = 0;
        board[idx].is_shot = false;
    }
    shots_taken = 0;
    hits_scored = 0;
}

GameStatus Game::getGameStatus(uint32_t max_allowed_shots)
{
    if(shots_taken >= max_allowed_shots)
    {
        return GameStatus::OutOfShots;
    }
    if(hits_scored >= total_ship_spaces)
    {
        return GameStatus::Victory;
    }
    return GameStatus::Incomplete;
}

bool Game::fireAt(uint32_t x_target, uint32_t y_target, uint32_t& hit_id)
{
    if(x_target < 0 || x_target >= board_cols)
    {
        throw std::out_of_range("base_x out of bounds");
    }
    if(y_target < 0 || y_target >= board_rows)
    {
        throw std::out_of_range("base_y out of bounds");
    }
    BoardSpace& space = board[boardIndex(x_target, y_target)];
    if(space.is_shot) 
    {
        return false;
    }
    shots_taken++;
    space.is_shot = true;
    if(space.ship_id)
    {
        hits_scored++;
        hit_id = space.ship_id;
        return true;
    }
    return false;
}

bool Game::placeShip(uint32_t ship_id, uint32_t length, ShipOrientation orientation, uint32_t base_x, uint32_t base_y)
{
    if(ship_id <= 0)
    {
        throw std::invalid_argument("ship_id must be a positive integer");
    }
    if(length < 1)
    {
        throw std::invalid_argument("length must be at least 1");
    }
    if(base_x < 0 || base_x >= board_cols)
    {
        throw std::out_of_range("base_x out of bounds");
    }
    if(base_y < 0 || base_y >= board_rows)
    {
        throw std::out_of_range("base_y out of bounds");
    }
    uint32_t test_x = base_x, test_y = base_y;
    for(int idx = 0; idx < length; idx++)
    {
        if(test_x >= board_cols || test_y >= board_rows)
        {
            return false;
        }
        BoardSpace& candiate_space = board[boardIndex(test_x, test_y)];
        if(candiate_space.ship_id != 0 || candiate_space.is_shot){
            return false;
        }
        if(orientation == ShipOrientation::Horizontal)
        {
            test_x++;
        } 
        else 
        {
            test_y++;
        }
    }
    for(int idx = 0; idx < length; idx++)
    {
        uint32_t board_index = boardIndex(base_x, base_y);
        board[board_index].ship_id = ship_id;
        if(orientation == ShipOrientation::Horizontal)
        {
            base_x++;
        } 
        else 
        {
            base_y++;
        }
    }
    total_ship_spaces += length;
    return true;
}

bool Game::startGame()
{
    if(!compute_stats)
    {
        return shots_taken == 0;
    }
    for(int idx = 0; idx < board_size; idx++)
    {
        BoardSpace& space = board[idx];
        stats_board[idx].games_played++;
        if(space.ship_id)
        {
            if(stats_board[idx].ship_id_count.count(space.ship_id))
            {
                stats_board[idx].ship_id_count[space.ship_id]++;
            }
            else
            {
                stats_board[idx].ship_id_count.emplace(space.ship_id, 1);
            }
        }
        else
        {
            stats_board[idx].empty_count++;
        }
        
    }
    return shots_taken == 0;
}

const SpaceStats& Game::getSpaceStats(uint32_t x, uint32_t y)
{
    if(x < 0 || x >= board_cols) throw std::out_of_range("x out of bounds");
    if(y < 0 || y >= board_rows) throw std::out_of_range("y out of bounds");
    if(!compute_stats) throw std::runtime_error(
        "Cannot get stats as stats were disabled at game creation");
    return stats_board[boardIndex(x, y)];
}

void Game::populateMatchingBoards()
{
    last_matching_boards.clear();
    bool differ;
    for(auto candidateBoard : session_boards) 
    {
        differ = false;
        for(int idx = 0; idx < board_size; idx++)
        {
            if(board[idx].is_shot && (board[idx].ship_id != candidateBoard[idx].ship_id))
            {
                differ = true;
                break;
            }
        }
        if(!differ)
        {
            last_matching_boards.push_back(candidateBoard);
        }
    }
}

SpaceStats Game::computeStatsForMatching(uint32_t x, uint32_t y)
{
    //maybe just cache the whole array of results?
    SpaceStats result;
    if(!last_match_board || board != last_match_board)
    {
        populateMatchingBoards();
    }
    uint32_t space_id;
    uint32_t board_idx = boardIndex(x, y);
    for(auto matchingBoard : last_matching_boards)
    {
        space_id = matchingBoard[board_idx].ship_id;
        if(space_id == 0)
        {
            result.empty_count++;
        }
        else if(result.ship_id_count.count(space_id) == 0)
        {
            result.ship_id_count.emplace(space_id, 1);
        }
        else
        {
            result.ship_id_count[space_id]++;
        }
        result.games_played++;
    }
    return result;
}

std::string Game::boardAsString()
{
    std::stringstream ss;
    for(int row = 0; row < board_rows; row++)
    {
        ss << '|';
        for(int col = 0; col < board_cols; col++)
        {
            ss << ' ' << board[boardIndex(row, col)].as_char() << " |";
        }
        ss << '\n';
    }
    return ss.str();
}

inline uint32_t Game::boardIndex(uint32_t x, uint32_t y)
{
    return x + board_cols * y;
}

#include "game.hpp"
#include <sstream>

Game::Game(uint32_t board_rows, uint32_t board_cols)
    : board_rows(board_rows), board_cols(board_cols), shots_taken(0), 
    hits_scored(0), total_ship_spaces(0)
{
    board_size = board_rows * board_cols;
    board = new BoardSpace[board_size];
    resetGame();
}

Game::~Game() 
{
    delete[] board;
}

void Game::resetGame() {
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
        if(test_x >= board_cols || test_y >= board_rows)
        {
            return false;
        }
    }
    for(int idx = 0; idx < length; idx++)
    {
        board[boardIndex(base_x, base_y)].ship_id = ship_id;
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

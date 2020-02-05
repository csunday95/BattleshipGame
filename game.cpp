#include "game.hpp"

Game::Game(uint32_t board_rows, uint32_t board_cols)
    : board_rows(board_rows), board_cols(board_cols)
{
    uint32_t board_size = board_rows * board_cols;
    board = new SpaceType[board_size];
    for(int idx = 0; idx < board_size; idx++)
    {
        board[idx] = SpaceType::Empty;
    }
}

Game::~Game() 
{
    delete[] board;
}

bool Game::fire_at(uint32_t x_target, uint32_t y_target, SpaceType& type_out)
{
    SpaceType current = board[boardIndex(x_target, y_target)];
    if(current != SpaceType::Empty) 
    {
        return false;
    }
    
    return true;

}

inline uint32_t Game::boardIndex(uint32_t x, uint32_t y)
{
    return x + board_cols * y;
}

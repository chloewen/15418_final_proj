#include "Block.h"
#include "Board.h"
#include "globals.h"
#include <vector>
#include <tuple>
#include <cassert>
#include <iostream>

Board::Board(std::vector<Block> blocks, std::vector<std::tuple<int, char, int>> prevMoves)
{
    this->blocks = blocks;
    this->prevMoves = prevMoves;
}

Board::Board()
{
    std::vector<Block> blocks;
    std::vector<std::tuple<int, char, int>> prevMoves;
    this->blocks = blocks;
    this->prevMoves = prevMoves;
}

bool Board::isSolved()
{
    for (int i = 1; i < (this->blocks).size(); i++)
    {
        if ((this->blocks)[i].TL_y == HOLE_Y)
            return false;
    }
    return true;
}

bool Board::collideTwo(Block b1, Block b2)
{
    assert(b1.id != b2.id);
    if (b1.orientation == 'h')
    {
        if (b2.orientation == 'h')
        { // - -
            return b1.TL_y == b2.TL_y && ((b1.TL_x <= b2.TL_x && b2.TL_x < b1.TL_x + b1.length) 
                                       || (b2.TL_x <= b1.TL_x && b1.TL_x < b2.TL_x + b2.length));
        }
        else
        { // - |
            assert(b2.orientation == 'v');
            return (b1.TL_x <= b2.TL_x && b2.TL_x < b1.TL_x + b1.length /*x's collide*/
                 && b2.TL_y <= b1.TL_y && b1.TL_y < b2.TL_y + b2.length /*y's collide*/);
        }
    }
    else
    {
        assert(b1.orientation == 'v');
        if (b2.orientation == 'h')
        { // | -
            return (b2.TL_x <= b1.TL_x && b1.TL_x < b2.TL_x + b2.length /*x's collide*/
                 && b1.TL_y <= b2.TL_y && b2.TL_y < b1.TL_y + b1.length /*y's collide*/);
        }
        else
        { // | |
            return b1.TL_x == b2.TL_x && ((b1.TL_y <= b2.TL_y && b2.TL_y < b1.TL_y + b1.length)
                                       || (b2.TL_y <= b1.TL_y && b1.TL_y < b2.TL_y + b2.length));
        }
    }
}

bool Board::collideAny(Block b)
{
    for (int i = 0; i < (this->blocks).size(); i++)
    {
        if (b.id != i && collideTwo(b, (this->blocks)[i])) return true;
    }
    return false;
}

bool Board::blockInBounds(Block b)
{
    if (b.orientation == 'h')
    {
        return b.TL_x >= 0 && b.TL_x + b.length <= BOARD_WIDTH && b.TL_y >= 0 && b.TL_y < BOARD_HEIGHT;
    }
    else
    {
        assert(b.orientation == 'v');
        return b.TL_y >= 0 && b.TL_y + b.length <= BOARD_HEIGHT && b.TL_x >= 0 && b.TL_x < BOARD_WIDTH;
    }
}

// direction = U, D, R, L
// (0,0) is top left of the board
bool Board::canMove(int id, int dist, char direction)
{
    bool isValid;
    Block newB = move(id, dist, direction);
    // need check: doesn't run into a wall or another block 
    return blockInBounds(newB) && !collideAny(newB);
}

Block Board::move(int id, int dist, char direction)
{
    Block b = this->blocks[id];

    if (direction == 'U')
    {
        b.TL_y -= dist;
    }
    else if (direction == 'D')
    {
        b.TL_y += dist;
    }
    else if (direction == 'R')
    {
        b.TL_x += dist;
    }
    else if (direction == 'L')
    {
        b.TL_x -= dist;
    }

    return b;
}

// returns false if there was a collision, true otherwise
bool Board::getNextBoardsInOneDirection(int i, int dist, char direction, std::vector<Board> *nextBoards)
{
    if (canMove(i, dist, direction))
    {
        std::vector<Block> newBlocks = this->blocks;
        newBlocks[i] = move(i, dist, direction); // TODO: IS THIS ALIASED????????????????????????
        std::vector<std::tuple<int, char, int>> newPrevMoves = this->prevMoves;
        newPrevMoves.push_back(std::make_tuple(i, direction, dist)); // TODO: IS THIS ALIASED????????????????????????
        (*nextBoards).push_back(Board(newBlocks, newPrevMoves));
        return true;
    }
    return false;
}

void Board::printBoard() {
    for (int i = 0; i < this->blocks.size(); i++) {
        Block b = (this->blocks)[i];
        b.printBlock();
    }
}

std::vector<Board> Board::getNextBoards()
{
    std::vector<Board> nextBoards;
    for (int i = 0; i < (this->blocks).size(); i++)
    {
        Block b = (this->blocks)[i];
        if (b.orientation == 'h')
        {
            // try to go left
            for (int dx = 1; dx <= b.TL_x; dx++)
            {
                // increment distance by one. If collision, stop 
                if (!getNextBoardsInOneDirection(i, dx, 'L', &nextBoards)) break; 
            }
            // try to go right
            for (int dx = 1; dx <= BOARD_WIDTH - (b.TL_x + b.length); dx++)
            {
                // increment distance by one. If collision, stop 
                if (!getNextBoardsInOneDirection(i, dx, 'R', &nextBoards)) break; 
            }
        }
        else
        {
            assert(b.orientation == 'v');
            // try to go up
            for (int dy = 1; dy <= b.TL_y; dy++)
            {
                // increment distance by one. If collision, stop 
                if (!getNextBoardsInOneDirection(i, dy, 'U', &nextBoards)) break; 
            }
            // try to go down
            for (int dy = 1; dy <= BOARD_HEIGHT - (b.TL_y + b.length); dy++)
            {
                // increment distance by one. If collision, stop 
                if (!getNextBoardsInOneDirection(i, dy, 'D', &nextBoards)) break; 
            }
        }
    }
    // std::cout << (nextBoards.size()) << std::endl;
    // std::cout << "Current Board" << std::endl;
    // (this)->printBoard();
    
    // for (int i = 0; i < nextBoards.size(); i++) {
    //     std::cout << "Next Board " << i << std::endl;
    //     (nextBoards[i]).printBoard();
    // }
    return nextBoards;
}


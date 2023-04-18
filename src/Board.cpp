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
    // b1.printBlock(); b2.printBlock();
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
        if (b.id != i && collideTwo(b, (this->blocks)[i])){
            // std::cout << "b.id: " << b.id << ", i: " << i << std::endl; 
            return true;
        }
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
    // std::cout << "newB" << std::endl;
    // newB.printBlock(); 
    // std::cout << blockInBounds(newB) << !collideAny(newB) << std::endl;
    // need check: doesn't run into a wall or another block ON THE WAY (TODO)
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

bool Board::getNextBoardsInOneDirection(int i, int dist, char direction, std::vector<Board> *nextBoards)
{
    // std::cout << "direction: " << direction << ", dist: " << dist << ", i: " << i << std::endl; 
    if (canMove(i, dist, direction))
    {
        // std::cout << "can move true" << std::endl;
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
    int i = 2; 
    if (i == 2)
    // for (int i = 0; i < (this->blocks).size(); i++)
    {
        // std::cout << "moving block " << i << std::endl;
        Block b = (this->blocks)[i];
        if (b.orientation == 'h')
        {
            for (int dx = 1; dx < BOARD_WIDTH - b.length; dx++)
            {
                // std::cout << "dx " << dx << std::endl;
                if (!getNextBoardsInOneDirection(i, dx, 'L', &nextBoards)) break; // try to go left
                // std::cout << "going left " << std::endl;
                // for (int i = 0; i < nextBoards.size(); i++) {
                //     std::cout << "Next Board " << i << std::endl;
                //     (nextBoards[i]).printBoard();
                // }
            }
            for (int dx = 1; dx < BOARD_WIDTH - b.length; dx++)
            {
                if (!getNextBoardsInOneDirection(i, dx, 'R', &nextBoards)) break; // try to go right
                // std::cout << "going right " << std::endl;
                // for (int i = 0; i < nextBoards.size(); i++) {
                //     std::cout << "Next Board " << i << std::endl;
                //     (nextBoards[i]).printBoard();
                // }
            }
        }
        else
        {
            assert(b.orientation == 'v');
            // int dy = 1;
            // if (dy == 1)
            for (int dy = 1; dy < BOARD_HEIGHT - b.length; dy++)
            {
                // std::cout << "dy " << dy << std::endl;
                if (!getNextBoardsInOneDirection(i, dy, 'U', &nextBoards)) break; // try to go up
                // std::cout << "going up " << std::endl;
                // for (int i = 0; i < nextBoards.size(); i++) {
                //     std::cout << "Next Board " << i << std::endl;
                //     (nextBoards[i]).printBoard();
                // }
            }
            for (int dy = 1; dy < BOARD_HEIGHT - b.length; dy++)
            {
                if (!getNextBoardsInOneDirection(i, dy, 'D', &nextBoards)) break; // try to go down
                // std::cout << "going down " << std::endl;
                // for (int i = 0; i < nextBoards.size(); i++) {
                //     std::cout << "Next Board " << i << std::endl;
                //     (nextBoards[i]).printBoard();
                // }
            }
        }
    }
    std::cout << (nextBoards.size()) << std::endl;
    std::cout << "Current Board" << std::endl;
    (this)->printBoard();
    
    for (int i = 0; i < nextBoards.size(); i++) {
        std::cout << "Next Board " << i << std::endl;
        (nextBoards[i]).printBoard();
    }
    return nextBoards;
}


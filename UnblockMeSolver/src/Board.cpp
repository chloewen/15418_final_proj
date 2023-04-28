#include "Block.h"
#include "Board.h"
#include "globals.h"
#include <vector>
#include <tuple>
#include <cassert>
#include <iostream>
#include <fstream>

Board::Board(std::vector<Block> blocks, std::vector<std::tuple<int, int, char>> prevMoves)
{
    this->blocks = blocks;
    this->prevMoves = prevMoves;
}

Board::Board()
{
    std::vector<Block> blocks;
    std::vector<std::tuple<int, int, char>> prevMoves;
    this->blocks = blocks;
    this->prevMoves = prevMoves;
}

// returns true if any block to to the right of block 0 crosses the HOLE_Y axis
bool Board::isSolved()
{
    for (int i = 1; i < (this->blocks).size(); i++)
    {
        Block b = (this->blocks)[i];
        if (b.BL_x >= (this->blocks)[0].BL_x){
            if (b.orientation == 'v'){
                if (b.BL_y <= HOLE_Y && HOLE_Y < b.BL_y + b.length)
                    return false;
            } else {
                assert(b.orientation == 'h');
                if (b.BL_y == HOLE_Y) return false; 
            }
        }
    }
    return true;
}

// returns true if b1 and b2 collide
bool Board::collideTwo(Block b1, Block b2)
{
    assert(b1.id != b2.id);
    if (b1.orientation == 'h')
    {
        if (b2.orientation == 'h')
        { // - -
            return b1.BL_y == b2.BL_y && ((b1.BL_x <= b2.BL_x && b2.BL_x < b1.BL_x + b1.length) 
                                       || (b2.BL_x <= b1.BL_x && b1.BL_x < b2.BL_x + b2.length));
        }
        else
        { // - |
            assert(b2.orientation == 'v');
            return (b1.BL_x <= b2.BL_x && b2.BL_x < b1.BL_x + b1.length /*x's collide*/
                 && b2.BL_y <= b1.BL_y && b1.BL_y < b2.BL_y + b2.length /*y's collide*/);
        }
    }
    else
    {
        assert(b1.orientation == 'v');
        if (b2.orientation == 'h')
        { // | -
            return (b2.BL_x <= b1.BL_x && b1.BL_x < b2.BL_x + b2.length /*x's collide*/
                 && b1.BL_y <= b2.BL_y && b2.BL_y < b1.BL_y + b1.length /*y's collide*/);
        }
        else
        { // | |
            return b1.BL_x == b2.BL_x && ((b1.BL_y <= b2.BL_y && b2.BL_y < b1.BL_y + b1.length)
                                       || (b2.BL_y <= b1.BL_y && b1.BL_y < b2.BL_y + b2.length));
        }
    }
}

// returns true if b collides with any other block
bool Board::collideAny(Block b)
{
    for (int i = 0; i < (this->blocks).size(); i++)
    {
        if (b.id != i && collideTwo(b, (this->blocks)[i])) return true;
    }
    return false;
}

// returns false if b is out of bounds
bool Board::blockInBounds(Block b)
{
    if (b.orientation == 'h')
    {
        return b.BL_x >= 0 && b.BL_x + b.length <= BOARD_WIDTH && b.BL_y >= 0 && b.BL_y < BOARD_HEIGHT;
    }
    else
    {
        assert(b.orientation == 'v');
        return b.BL_y >= 0 && b.BL_y + b.length <= BOARD_HEIGHT && b.BL_x >= 0 && b.BL_x < BOARD_WIDTH;
    }
}

// direction = U, D, R, L
// (0,0) is top left of the board
bool Board::canMove(int id, int dist, char direction)
{
    bool isValid;
    Block newB = move(id, dist, direction);
    return blockInBounds(newB) && !collideAny(newB);
}

// returns a block moved d dist in direction direction
Block Board::move(int id, int dist, char direction)
{
    Block b = this->blocks[id];
    switch(direction) {
        case 'U':
            b.BL_y += dist;
            break;
        case 'D':
            b.BL_y -= dist;
            break;
        case 'R':
            b.BL_x += dist;
            break;
        case 'L':
            b.BL_x -= dist;
            break;
    }

    return b;
}

void printVectorMoves(std::vector<std::tuple<int, char, int> > V) {
    for (int i = 0; i < V.size(); i++) {
        std::cout << "id: " << std::get<0>(V[i]) << ", direction: " << std::get<1>(V[i]) << ", distance: " << std::get<2>(V[i]);
    }
}

// returns false if there was a collision, true otherwise
bool Board::getNextBoardsInOneDirection(int id, int dist, char direction, std::vector<Board> *nextBoards)
{
    if (canMove(id, dist, direction))
    {
        std::vector<Block> newBlocks = this->blocks;
        newBlocks[id] = move(id, dist, direction); 
        std::vector<std::tuple<int, int, char>> newPrevMoves = this->prevMoves;
        newPrevMoves.push_back(std::make_tuple(id, dist, direction));
        Board newB = Board(newBlocks, newPrevMoves);
        (*nextBoards).push_back(newB);
        return true;
    }
    return false;
}

void Board::printBoard(std::ofstream *outputFileP) {
    *outputFileP << "  "; 
    for (int x = 0; x < BOARD_WIDTH; x++) { *outputFileP << x << " "; }
    *outputFileP << std::endl; 
    *outputFileP << "  "; 
    for (int x = 0; x < BOARD_WIDTH; x++) { *outputFileP << "--"; }
    *outputFileP << std::endl; 
    // print rows out backward 
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        *outputFileP << y << "|";
        for (int x = 0; x < BOARD_WIDTH; x++) {
            bool hasBlock = false;
            for (int i = 0; i < this->blocks.size(); i++) {
                Block b = (this->blocks)[i];
                if (b.orientation == 'h') {
                    if (b.BL_y == y && b.BL_x <= x && x < b.BL_x + b.length) {
                        *outputFileP << i;
                        hasBlock = true;
                    }
                } else {
                    assert(b.orientation == 'v');
                    if (b.BL_x == x && b.BL_y <= y && y < b.BL_y + b.length) {
                        *outputFileP << i;
                        hasBlock = true;
                    }
                }
            }
            if (!hasBlock) *outputFileP << " ";
            *outputFileP << " ";
        }
        if (y != HOLE_Y) {*outputFileP << "|" << std::endl;}
        else {*outputFileP << std::endl;}
    }
    *outputFileP << "  "; 
    for (int x = 0; x < BOARD_WIDTH; x++) { *outputFileP << "--"; }
    *outputFileP << std::endl; 
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
            for (int dx = 1; dx <= b.BL_x; dx++)
            {
                // increment distance by one. If collision, stop 
                if (!getNextBoardsInOneDirection(i, dx, 'L', &nextBoards)) break; 
            }
            // try to go right
            for (int dx = 1; dx <= BOARD_WIDTH - (b.BL_x + b.length); dx++)
            {
                // increment distance by one. If collision, stop 
                if (!getNextBoardsInOneDirection(i, dx, 'R', &nextBoards)) break; 
            }
        }
        else
        {
            assert(b.orientation == 'v');
            // try to go down
            for (int dy = 1; dy <= b.BL_y; dy++)
            {
                // increment distance by one. If collision, stop 
                if (!getNextBoardsInOneDirection(i, dy, 'D', &nextBoards)) break; 
            }
            // try to go up
            for (int dy = 1; dy <= BOARD_HEIGHT - (b.BL_y + b.length); dy++)
            {
                // increment distance by one. If collision, stop 
                if (!getNextBoardsInOneDirection(i, dy, 'U', &nextBoards)) break; 
            }
        }
    }
    return nextBoards;
}


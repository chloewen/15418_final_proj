#include "Block.h"
#include "Board.h"
#include "globals.h"

Board::Board(std::vector<Block> blocks, std::vector<std::tuple<int, char, int>> prevMoves) {
    this->blocks = blocks;
    this->prevMoves = prevMoves;
}

bool isSolved() {
    for (int i = 1; i < (this->blocks).size(); i++) {
        if ((this->blocks)[i].TL_y == HOLE_Y) return false;
    }
    return true;
}

bool collideTwo(Block b1, Block b2) {
    if (b1.orientation == "h") {
        if (b2.orientation == "h") { // - -
            return b1.TL_y == b2.TL_y;
        } else { // - |
            assert(b2.orientation == "v");
            return (b1.TL_x <= b2.TL_x && b2.TL_x <= b1.TL_x+b1.length /*x's collide*/
                    && b2.TL_y <= b1.TL_y && b1.TL_y <= b2.TL_y+b2.length /*y's collide*/);
        }
    } else {
        assert(b1.orientation == "v");
        if (b2.orientation == "h") { // | - 
            return (b2.TL_x <= b1.TL_x && b1.TL_x <= b2.TL_x+b2.length /*x's collide*/
                    && b1.TL_y <= b2.TL_y && b2.TL_y <= b1.TL_y+b1.length /*y's collide*/);
        } else { // | |
            return b1.TL_x == b2.TL_x;
        }
    }
}

bool collideAny(Block b) {
    for (int i = 0; i < (this->blocks).size(); i++) {
        if (b.i != i && collideTwo(b, (this->blocks)[i])) return true;
    }
    return false;
}


bool blockInBounds(Block b) {
    if (b.orientation == 'h') {
        return b.TL_x >= 0 && b.TL_x + b.length < BLOCK_WIDTH && b.TL_y >= 0 && b.TL_y < BLOCK_HEIGHT;
    } else {
        assert(b.orientation == 'v');
        return b.TL_y >= 0 && b.TL_y + b.length < BLOCK_HEIGHT && b.TL_x >= 0 && b.TL_x < BLOCK_WIDTH;
    }
}

// direction = U, D, R, L
// (0,0) is top left of the board
bool canMove(Block b, int dist, char direction) {
    bool isValid; 
    Block newB = move(dist, direction);
    // need check: doesn't run into a wall or another block
    return blockInBounds(newB) && !collideAny(newB);
} 

Block move(int dist, char direction) {
    Block b = this->blocks[i];
    switch(direction) {
    case "U":
      b.TL_y -= dist;
      break;
    case "D":
      b.TL_y += dist;
      break;
    case "R":
      b.TL_x += dist;
      break;
    case "L":
      b.TL_x -= dist;
      break;
  }
  return b;
}

void getNextBoardsInOneDirection(char direction, int displ, int i, std::vector<std::tuple<int, char, int>> *nextBoards) {
    if (canMove(b, displ, direction)) {
        std::vector<Block> newBlocks = this->blocks;
        newBlocks[i] = move(displ, direction); // TODO: IS THIS ALIASED????????????????????????
        std::vector<std::tuple<int, char, int>> newPrevMoves = this->prevMoves;
        newPrevMoves.push_back(std::make_tuple(i, direction, displ)); // TODO: IS THIS ALIASED????????????????????????
        (*nextBoards).push_back(Board(newBlocks, newPrevMoves));
    }
}

std::vector<Board> getNextBoards() {
    std::vector<Board> nextBoards;
    for (int i = 1; i < (this->blocks).size; i++) {
        Block b = (this->blocks)[i];
        if (b.orientation == "h") {
            for (int dx = 0; dx < BOARD_WIDTH; dy++) {
                getNextBoardsInOneDirection("L", dx, i, nextBoards); // try to go left
                getNextBoardsInOneDirection("R", dx, i, nextBoards); // try to go right 
            }   
        } else {
            assert(b.orientation == "v");
            for (int dy = 0; dy < BOARD_HEIGHT) {
                getNextBoardsInOneDirection("U", dy, i, nextBoards); // try to go up
                getNextBoardsInOneDirection("D", dy, i, nextBoards); // try to go down
            }
        }
    }
}
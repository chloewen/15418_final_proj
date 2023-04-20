#ifndef UNBLOCK_ME_SOLVER_BOARD_H
#define UNBLOCK_ME_SOLVER_BOARD_H

#include <vector>
#include <tuple>
#include "Block.h"
#include <iostream>

class Board
{
public:
  Board(std::vector<Block> blocks, std::vector<std::tuple<int, int, char>> prevMoves);
  Board();
  bool isSolved();
  Block move(int id, int dist, char direction);
  std::vector<Board> getNextBoards();
  std::vector<Block> blocks;
  // prevMoves is vector of tuples (id, dist, direction)
  std::vector<std::tuple<int, int, char>> prevMoves;
  void printBoard(std::ofstream *outputFileP);

private:
  bool collideTwo(Block b1, Block b2);
  bool collideAny(Block b);
  bool blockInBounds(Block b);
  bool canMove(int id, int dist, char direction);

  bool getNextBoardsInOneDirection(int id, int dist, char direction, std::vector<Board> *nextBoards);
};
#endif //UNBLOCK_ME_SOLVER_BOARD_H

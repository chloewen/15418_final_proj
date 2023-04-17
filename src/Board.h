#ifndef UNBLOCK_ME_SOLVER_BOARD_H
#define UNBLOCK_ME_SOLVER_BOARD_H

#include <vector>
#include <tuple>
#include "Block.h"

class Board
{
public:
  // prevMoves is vector of tuples (id, direction, dist)
  Board(std::vector<Block> blocks, std::vector<std::tuple<int, char, int>> prevMoves);
  Board();
  bool isSolved();
  Block move(int id, int dist, char direction);
  std::vector<Board> getNextBoards();
  std::vector<std::tuple<int, char, int>> prevMoves;

private:
  bool collideTwo(Block b1, Block b2);
  bool collideAny(Block b);
  bool blockInBounds(Block b);
  bool canMove(int id, int dist, char direction);

  void getNextBoardsInOneDirection(char direction, int displ, int i, std::vector<Board> nextBoards);
  std::vector<Block> blocks;
};
#endif //UNBLOCK_ME_SOLVER_BOARD_H

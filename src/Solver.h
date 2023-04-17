#ifndef UNBLOCK_ME_SOLVER_SOLVER_H
#define UNBLOCK_ME_SOLVER_SOLVER_H

#include "Board.h"
#include <vector>
#include <deque>

class Solver
{
public:
  Solver(Board startingBoard);
  std::vector<std::tuple<int, char, int>> solveBFS();

private:
  Board startingBoard;
  bool isIn(std::deque<Board> Q, Board e);
  bool vecEqual(std::vector<Block> B1, std::vector<Block> B2);
};

#endif //UNBLOCK_ME_SOLVER_SOLVER_H

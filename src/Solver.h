#ifndef UNBLOCK_ME_SOLVER_SOLVER_H
#define UNBLOCK_ME_SOLVER_SOLVER_H

#include "Board.h"
#include <vector>

class Solver
{
public:
  Solver(Board startingBoard);
  std::vector<std::tuple<int, char, int>> solveBFS();

private:
  Board startingBoard;
};

#endif //UNBLOCK_ME_SOLVER_SOLVER_H

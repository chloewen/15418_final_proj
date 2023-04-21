#ifndef UNBLOCK_ME_SOLVER_SOLVER_H
#define UNBLOCK_ME_SOLVER_SOLVER_H

#include "Board.h"
#include <vector>
#include <deque>
#include <iostream>

class OpenMPSolver
{
public:
    OpenMPSolver(Board startingBoard, std::ofstream *outputFileP);
    std::vector<std::tuple<int, int, char>> solveBFS();
    std::ofstream *outputFileP;

private:
    Board startingBoard;

    bool isIn(std::deque<Board> Q, Board e);
    bool vecEqual(std::vector<Block> B1, std::vector<Block> B2);
    void printDeque(std::deque<Board> Q);
};

#endif //UNBLOCK_ME_SOLVER_SOLVER_H
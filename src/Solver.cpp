#include "Solver.h"
#include "Board.h"
#include "Block.h"
#include "globals.h"
#include <vector>
#include <tuple>
#include <set>
#include <deque>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

Solver::Solver(Board startingBoard, std::ofstream *outputFileP)
{
  this->startingBoard = startingBoard;
  this->outputFileP = outputFileP;
}

bool Solver::vecEqual(std::vector<Block> B1, std::vector<Block> B2) {
  if (B1.size() != B2.size()) return false;
  for (int i = 0; i < B1.size(); i++) {
    Block b1 = B1[i];
    Block b2 = B2[i];
    if (b1.TL_x != b2.TL_x 
        || b1.TL_y != b2.TL_y 
        || b1.length != b2.length
        || b1.orientation != b2.orientation) return false;
  }
  return true;
}

void Solver::printDeque(std::deque<Board> Q) {
  for (int i = 0; i < Q.size(); i++) {
    Q[i].printBoard(this->outputFileP);
  }
}

bool Solver::isIn(std::deque<Board> Q, Board x) {
  for (int i = 0; i < Q.size(); i++) {
    Board b = Q[i];
    if (vecEqual(b.blocks, x.blocks)) {
      return true;
    }
  }
  return false;
}

std::vector<std::tuple<int, char, int>> Solver::solveBFS()
{ 
  std::vector<std::tuple<int, char, int>> empty;
  std::deque<Board> explored;
  std::deque<Board> frontier;
  frontier.push_back(this->startingBoard);
  while (true) 
  {
    if (frontier.empty()) return empty;
    Board currBoard = frontier.front();
    frontier.pop_front();
    if (currBoard.isSolved()) {
      *this->outputFileP << "solution board" << std::endl;
      currBoard.printBoard(this->outputFileP);
      return currBoard.prevMoves;
    }
    if (!isIn(explored,currBoard))
    {
      explored.push_back(currBoard);
      std::vector<Board> nextBoards = currBoard.getNextBoards();
      for (int i = 0; i < nextBoards.size(); i++)
      {
        Board nextBoard = nextBoards[i];
        if (!isIn(explored, nextBoard) && !isIn(frontier,nextBoard))
        {
          frontier.push_back(nextBoard);
        }
      }
    }
  }
  return empty;
}

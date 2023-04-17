#include "Solver.h"
#include "Board.h"
#include <vector>
#include <tuple>

Solver::Solver(Board startingBoard) {
  this->startingBoard = startingBoard;
}

std::vector<std::tuple<int, char, int>> Solver::solveBFS() {
  std::set<Board> explored;
  std::set<Board> frontierSet; 
  std::queue<Board> frontier;
  frontier.push(this->startingBoard);
  while (true) {
    if (frontier.empty()) return NULL;
    Board currBoard = frontier.pop();
    if (currBoard.isSolved()) return currBoard.pastActions; 
    if (!explored.contains(currBoard)) {
      explored.insert(currBoard);
      std::vector<Board> nextBoards = currBoard.getNextBoards;
      for (int i = 0; i < nextBoards.size(); i++) {
        Board nextBoard = nextBoards[i];
        if (!explored.contains(nextBoard) && !frontierSet.contains(nextBoard)) {
          frontierSet.push(nextBoard);
          frontier.push(nextBoard);
        } 
      }
    }
  }
  return NULL;
}

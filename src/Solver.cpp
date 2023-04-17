#include "Solver.h"
#include "Board.h"
#include <vector>
#include <tuple>
#include <set>
#include <queue>

Solver::Solver(Board startingBoard)
{
  this->startingBoard = startingBoard;
}

std::vector<std::tuple<int, char, int>> Solver::solveBFS()
{
  std::vector<std::tuple<int, char, int>> empty;
  std::set<Board> explored;
  std::set<Board> frontierSet;
  std::queue<Board> frontier;
  frontier.push(this->startingBoard);
  while (true)
  {
    if (frontier.empty())
      return empty;
    Board currBoard = frontier.front();
    frontier.pop();
    if (currBoard.isSolved())
      return currBoard.prevMoves;
    if (explored.find(currBoard) == explored.end())
    {
      explored.insert(currBoard);
      std::vector<Board> nextBoards = currBoard.getNextBoards();
      for (int i = 0; i < nextBoards.size(); i++)
      {
        Board nextBoard = nextBoards[i];
        if (explored.find(nextBoard) == explored.end() && frontierSet.find(nextBoard) == frontierSet.end())
        {
          frontierSet.insert(nextBoard);
          frontier.push(nextBoard);
        }
      }
    }
  }
  return empty;
}

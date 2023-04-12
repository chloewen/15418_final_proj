#ifndef UNBLOCK_ME_SOLVER_BOARD_H
#define UNBLOCK_ME_SOLVER_BOARD_H

class Board {
public:
  // prevMoves is vector of tuples (id, direction, dist)
  Board(std::vector<Block> blocks, std::vector<std::tuple<int, char, int>> prevMoves);
  bool isSolved();
  Board move(int id, int dist);
  Board[] getNextBoards();
}

#endif //UNBLOCK_ME_SOLVER_BOARD_H

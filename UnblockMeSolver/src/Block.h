#ifndef UNBLOCK_ME_SOLVER_BLOCK_H
#define UNBLOCK_ME_SOLVER_BLOCK_H

class Block
{
public:
  Block(int i, int BL_x, int BL_y, int length, char orientation);
  int id;
  int BL_x; // col
  int BL_y; // row
  int length; // grows in right or upward direction, bottom left is (0,0)
  char orientation;
  void printBlock();
};
#endif //UNBLOCK_ME_SOLVER_BLOCK_H

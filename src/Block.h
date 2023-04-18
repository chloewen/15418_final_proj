#ifndef UNBLOCK_ME_SOLVER_BLOCK_H
#define UNBLOCK_ME_SOLVER_BLOCK_H

class Block
{
public:
  Block(int i, int TL_x, int TL_y, int length, char orientation);
  int id;
  int TL_x;
  int TL_y;
  int length;
  char orientation;
  void printBlock();
};
#endif //UNBLOCK_ME_SOLVER_BLOCK_H

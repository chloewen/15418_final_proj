#include "Block.h"

Block::Block(int id, int TL_x, int TL_y, int length, char orientation) {
    this->id = id;
    this->TL_x = TL_x;
    this->TL_y = TL_y;
    this->length = length;
    this->orientation = orientation;
}
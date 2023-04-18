#include "Block.h"
#include <iostream>

Block::Block(int id, int TL_x, int TL_y, int length, char orientation) {
    this->id = id;
    this->TL_x = TL_x;
    this->TL_y = TL_y;
    this->length = length;
    this->orientation = orientation;
}

void Block::printBlock() {
    std::cout << "id: " << this->id << ", TL_x: " << this->TL_x << ", TL_y: " << this->TL_y << ", length: " << this->length << ", orientation: " << this->orientation << std::endl;
}
#include "Block.h"
#include <iostream>

Block::Block(int id, int BL_x, int BL_y, int length, char orientation) {
    this->id = id;
    this->BL_x = BL_x;
    this->BL_y = BL_y;
    this->length = length;
    this->orientation = orientation;
}

void Block::printBlock() {
    std::cout << "id: " << this->id << ", BL_x: " << this->BL_x << ", BL_y: " << this->BL_y << ", length: " << this->length << ", orientation: " << this->orientation << std::endl;
}
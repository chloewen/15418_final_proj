#include <fstream>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include "Solver.h"
#include "Board.h"
#include "Block.h"

// std::ofstream outputFile("../data/output.txt");

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "wrong number of arguments" << std::endl;
        return -1;
    }
    // argv[1] is the input file
    std::ifstream inputFile(argv[1]);
    // argv[2] is the output file
    std::ofstream outputFile(argv[2]);
    if (!inputFile.is_open()) {
        std::cerr << "There was a problem with the input file, please verify that the input file is there." << std::endl;
        return -1;
    }
    if (!outputFile.is_open()) {
        inputFile.close();
        std::cerr << "There was a problem with the output file, please verify that the input file is there." << std::endl;
        return -1;
    }
    
    // construct board
    int TL_x, TL_y, length;
    int id = 0;
    char orientation;

    std::vector<Block> blocks;
    std::vector<std::tuple<int, char, int>> prevMoves(0);
    while (inputFile >> TL_x >> TL_y >> length >> orientation) {
        Block tempBlock = Block(id++, TL_x, TL_y, length, orientation);
        blocks.push_back(tempBlock);
    }
    Board startingBoard = Board(blocks, prevMoves);

    Solver s = Solver(startingBoard, &outputFile);
    std::vector<std::tuple<int, char, int>> soln = s.solveBFS();
    std::cout << "soln.size() " << soln.size() << std::endl;

    // write soln to console
    for (int i = 0; i < soln.size(); i++) {
        std::cout << "block id: " << std::get<0>(soln[i]) << ", direction: " << std::get<1>(soln[i]) << ", distance: " << std::get<2>(soln[i]) << "\n" << std::endl;
    }

    // TODO do that but better ^ (reconstruct the boards or smth)
    inputFile.close();
    outputFile.close();
    return 0; 
}
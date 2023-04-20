#include <fstream>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <chrono>
#include "Solver.h"
#include "Board.h"
#include "Block.h"

void printSoln(Board startingBoard, std::vector<std::tuple<int, int, char>> soln, std::ofstream *outputFileP) {
    if (soln.size() == 0) {
        *outputFileP << "NO SOLUTION FOUND :^(";
        return;
    }
    *outputFileP << "SOLUTION FOUND! :^)" << std::endl;
    Board b = startingBoard;
    for (int i = 0; i < soln.size(); i++) {
        int id = std::get<0>(soln[i]);
        int dist = std::get<1>(soln[i]);
        std::string distString;
        if (dist == 1) { distString = " step "; }
        else { distString = " steps "; }
        char direction = std::get<2>(soln[i]);
        std::string directionString;
        switch(direction) {
            case 'U':
                directionString = "up";
                break;
            case 'D':
                directionString = "down";
                break;
            case 'R':
                directionString = "to the right";
                break;
            case 'L':
                directionString = "to the left";
                break;
        }
        *outputFileP << "Step " << i+1 << ": Move block " << id << " " << dist << distString << directionString <<std::endl;
        b.blocks[id] = b.move(id, dist, direction);
        b.printBoard(outputFileP);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "wrong number of arguments" << std::endl;
        return -1;
    }
    // argv[1] is the input file
    std::ifstream inputFile(argv[1]);
    // argv[2] is the output file
    std::ofstream outputFile(argv[2]);
    if (!inputFile.is_open())
    {
        std::cerr << "There was a problem with the input file, please verify that the input file is there." << std::endl;
        return -1;
    }
    if (!outputFile.is_open())
    {
        inputFile.close();
        std::cerr << "There was a problem with the output file, please verify that the input file is there." << std::endl;
        return -1;
    }

    // construct Solver
    int TL_x, TL_y, length;
    int id = 0;
    char orientation;

    std::vector<Block> blocks;
    while (inputFile >> TL_x >> TL_y >> length >> orientation)
    {
        Block tempBlock = Block(id++, TL_x, TL_y, length, orientation);
        blocks.push_back(tempBlock);
    }
    std::vector<std::tuple<int, int, char>> prevMoves(0);
    Board startingBoard = Board(blocks, prevMoves);
    Solver s = Solver(startingBoard, &outputFile);

    // solve board
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::tuple<int, int, char>> soln = s.solveBFS();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> runningTime = end - start;

    std::cout << "soln.size() " << soln.size() << std::endl;
    std::cout << "Time taken " << runningTime.count() << std::endl;

    // write solution to output file 
    printSoln(startingBoard, soln, &outputFile);

    inputFile.close();
    outputFile.close();
    return 0;
}
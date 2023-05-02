#include <fstream>
#include <vector>
#include <tuple>
#include <iostream>
#include <chrono>
#include "UnblockMeSolver/src/Solver.h"
#include "UnblockMeSolver/src/OpenMPSolver.h"
#include <cstring>
#include "UnblockMeSolver/src/Board.h"
#include "UnblockMeSolver/src/Block.h"
#include "UnblockMeRenderer/render/cudaRenderer.h"
#include "UnblockMeRenderer/render/platformgl.h"
#include "globals.h"

// write solution to output file (pretty) output file raw (to pass into renderer)
void printSoln(Board startingBoard, std::vector<std::tuple<int, int, char>> soln, std::ofstream *outputFileP, std::ofstream *outputFileRawP) {
    if (soln.size() == 0) {
        *outputFileP << "NO SOLUTION FOUND :^(";
        return;
    }
    *outputFileP << "SOLUTION FOUND! :^)" << std::endl;
    *outputFileRawP << soln.size()+1 << std::endl;;
    Board b = startingBoard;
    for (int i = 0; i < soln.size(); i++) {
        int id = std::get<0>(soln[i]);
        int dist = std::get<1>(soln[i]);
        std::string distString;
        if (dist == 1) { distString = " step "; }
        else { distString = " steps "; }
        char direction = std::get<2>(soln[i]);
        std::string directionString;
        int cutoff;
        switch(direction) {
            case 'U':
                directionString = "up";
                cutoff = b.blocks[id].BL_y + dist;
                break;
            case 'D':
                directionString = "down";
                cutoff = b.blocks[id].BL_y - dist;
                break;
            case 'R':
                directionString = "to the right";
                cutoff = b.blocks[id].BL_x + dist;
                break;
            case 'L':
                directionString = "to the left";
                cutoff = b.blocks[id].BL_x - dist;
                break;
        }
        *outputFileP << "Step " << i+1 << ": Move block " << id << " " << dist << distString << directionString << std::endl;
        *outputFileRawP << id << " " << cutoff << " " << direction << std::endl; 
        b.blocks[id] = b.move(id, dist, direction);
        b.printBoard(outputFileP);
    }
    *outputFileRawP << "0 6 R";
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "wrong number of arguments" << std::endl;
        // TODO: make this better
        return -1;
    }
    std::string directory = argv[1];
    // argv[1] is the input file
    std::string inputBoardFileStr = directory + "/input.txt";
    std::ifstream inputFile(inputBoardFileStr);
    // argv[2] is the output file
    std::string outputSolnFileStr = directory + "/soln.txt";
    std::ofstream outputFile(outputSolnFileStr);
    std::string outputSolnRawFileStr = directory + "/soln-raw.txt";
    std::ofstream outputFileRaw(outputSolnRawFileStr);
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
    if (!outputFileRaw.is_open())
    {
        inputFile.close();
        outputFile.close(); 
        std::cerr << "There was a problem with the output rawfile, please verify that the input file is there." << std::endl;
        return -1;
    }

    // construct Solver
    int BL_x, BL_y, length;
    int id = 0;
    char orientation;

    std::vector<Block> blocks;
    int numBlocks; 
    inputFile >> numBlocks;

    while (inputFile >> BL_x >> BL_y >> length >> orientation)
    {
        Block tempBlock = Block(id++, BL_x, BL_y, length, orientation);
        blocks.push_back(tempBlock);
    }
    std::vector<std::tuple<int, int, char>> prevMoves(0);
    Board startingBoard = Board(blocks, prevMoves);
    // TODO: check if startingBoard is valid
    
    // solve board
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::tuple<int, int, char>> soln;
    // Solver s = Solver(startingBoard, &outputFile);
    // soln = s.solveBFS();

    // OpenMPSolver ompS = OpenMPSolver(startingBoard, &outputFile);
    // soln = ompS.solveBFS();
    
    if (!strcmp(argv[2], "BFS-seq")) {
        Solver s = Solver(startingBoard, &outputFile);
        soln = s.solveBFS();
    } else if (!strcmp(argv[2], "BFS-par-OMP")) {
        OpenMPSolver ompS = OpenMPSolver(startingBoard, &outputFile);
        soln = ompS.solveBFS();
    } else {
        std::cout << "Third argument invalid. Valid third arguments: BFS-seq, BFS-par-OMP" << std::endl;
        return -1;
    }


    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> runningTime = end - start;

    std::cout << "soln.size() " << soln.size() << std::endl;
    std::cout << "Time taken " << runningTime.count() << std::endl;

    // write solution to output file 
    printSoln(startingBoard, soln, &outputFile, &outputFileRaw);

    // CircleRenderer* cuda_renderer;
    // cuda_renderer = new CudaRenderer();
    // int imageSize = 1150; 
    // cuda_renderer->allocOutputImage(imageSize, imageSize);
    // cuda_renderer->loadScene(BLOCK);
    // cuda_renderer->setup();


    inputFile.close();
    outputFile.close();
    outputFileRaw.close();


    return 0;
}
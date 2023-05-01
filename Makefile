all: UnblockMeSolver/src/Block.cpp UnblockMeSolver/src/Board.cpp UnblockMeSolver/src/Solver.cpp main.cpp UnblockMeSolver/src/OpenMPSolver.cpp
	g++ -fopenmp -o a.out UnblockMeSolver/src/Block.cpp UnblockMeSolver/src/Board.cpp UnblockMeSolver/src/Solver.cpp main.cpp UnblockMeSolver/src/OpenMPSolver.cpp

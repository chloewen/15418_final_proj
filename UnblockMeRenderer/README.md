# CMU 15-418/618, Spring 2023

# Unblock Me Renderer 

To run solver: 
in the main directory, run:
make
./solver [input file directory] [parallel or sequential]
[input file directory] = 
data/board/easy1-6x6
data/board/easy2-6x6
data/board/easy3-6x6
data/board/med1-6x6
data/board/med2-6x6
data/board/med3-6x6
data/board/adv1-6x6
data/board/adv2-6x6
data/board/adv3-6x6
data/board/expert1-6x6
data/board/expert2-6x6
data/board/expert3-6x6

[parallel or sequential] = 
BFS-seq
BFS-par-OMP

To run renderer: 
change input file in cudaRenderer.cu to "../../[input file directory]/soln-raw.txt" on line 722
change input file in sceneLoader.cpp to "../../[input file directory]/input.txt"
in UnblockMeRenderer/render, run: 
make 
./render -r cuda block





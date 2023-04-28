#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <functional>
#include <iostream>
#include "sceneLoader.h"
#include "util.h"
#include <fstream>



void
loadCircleScene(
    SceneName sceneName,
    int& numCircles,
    float*& position,
    float*& velocity,
    float*& color,
    float*& radius)
{
    if (sceneName == BLOCK) {

        // simple test scene containing 3 circles. All circles have
        // 50% opacity
        //
        // farthest circle is red.  Middle is green.  Closest is blue.

        numCircles = 6;// 10;

        position = new float[3 * numCircles];
        velocity = new float[3 * numCircles];
        color = new float[3 * numCircles];
        radius = new float[numCircles];

        // for (int i=0; i<numCircles; i++)
        //     radius[i] = .3f;
        std::ifstream inputFile("input-1-6x6.txt");
        if (!inputFile.is_open())
        {
            std::cerr << "There was a problem with the input file, please verify that the input file is there." << std::endl;
        }

        int BL_x, BL_y, length;
        char orientation;
        int i = 0; 
        while (inputFile >> BL_x >> BL_y >> length >> orientation) {
            position[i] = BL_x; 
            position[i+1] = BL_y;
            position[i+2] = orientation == 'v' ? 1 : 0;
            color[i] = i == 0 ? .8f : .6f; 
            color[i+1] = i == 0 ? .4f : .5f; 
            color[i+2] = i == 0 ? .4f : .44f; 
            radius[i/3] = length; 
            i += 3; 
        }
        inputFile.close();

    } else {
        fprintf(stderr, "Error: cann't load scene (unknown scene)\n");
        return;
    }

    printf("Loaded scene with %d circles\n", numCircles);
}

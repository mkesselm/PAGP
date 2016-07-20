// #include "opencv2/opencv.hpp"
// #include <stdio.h>
// #include <iostream>
// #include "Imager.cpp"
// #include "setOffsets.cpp"
// #include "StageMapper.cpp"
#include "PAGP.h"

int main(int, char**) {
  /*
  *1. Call Imager.
  *2. Call stageMapper
  *3. Declare PathFinder queue
  *4. Enter core loop
  *******************
  * CORE LOOP
  *
  *1. Poll DataGate for inputVector
  *2. Pass inputVector and &stageMap to pathFinder
  *3. Pass pathFinder return to clawControl
  */

  lateralImageProduct sideCameraMapping;
  PullLateralImage(1, &sideCameraMapping);

  MappingVector2d xyMap, xzMap;
  setOffsets(&sideCameraMapping, &xyMap, &xzMap, 2);

  StageMap env;
  stageMapper(&env, &xyMap, &xzMap);
  /*****************************************************************************
  * THE BELOW IS DEBUG CODE--ONLY USE WHEN ABSOLUTELY NECESSARY AS IT IS HIGHLY
  * INTENSIVE. 3D VECTORS ARE NOT MEANT TO BE ITERATED THROUGH, ESPECIALLY NOT
  * WHEN THAT ITERATION INCLUDES PRINT CALLS. SERIOUSLY, ONLY USE TO DEBUG--
  * THIS BIT IS REALLY, REALLY GROSS.
  *****************************************************************************
  */
  // int height = env.stagemap.size();
  // int width = env.stagemap.at(0).size();
  // int depth = env.stagemap.at(0).at(0).size();
  // std::cout<<height<<"x"<<width<<"x"<<depth<<std::endl;
  // for(int i = 0; i <= height - 1; i++) {
  //   for(int j = 0; j <= width - 1; j++) {
  //     for(int k = 0; k <= depth - 1; k++) {
  //       if(env.stagemap.at(i).at(j).at(k).xOffset > 0 || env.stagemap.at(i).at(j).at(k).yOffset > 0 || env.stagemap.at(i).at(j).at(k).zOffset > 0) {
  //         printf("%d,%d,%d\n",i,j,k);
  //       }
  //     }
  //   }
  // }

  return 0;
}

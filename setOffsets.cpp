#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <iostream>

using namespace cv;
/*
* Populate two 2D plane 2dvectors
*
* *** The binary 2dvector from the lateral camera is used to calculate the
*   * xy offset MappingVector
*   * The 2dvector is then repurposed to store the top-down camera binary map
*   * The 2dvector is then used to calculate the xz offset MappingVector
*   * before it is deleted.
*
* @parameter MappingVector2d: a 2dvector of AxialOffsets2d structs--a struct
*            containing 2 offset ints
*/

void setOffsets (lateralImageProduct* lateralProduct, MappingVector2d* xy, MappingVector2d* xz, int cam){
  VideoCapture cap(cam); // open the camera

  Mat frame, edges0, edges1, edges2, output, tmpA, tmpB;

  /* MAPPINGVECTOR2DXY, LATERALPRODUCT->RESCALEDXY PASS TO OFFSETMAPPER HERE
  * We do this here so that we give the camera a bit of time to warm up and
  * also so that we can reuse our rescaling matrix from the lateral camera for
  * the overhead camera.
  */
  renderOffset(lateralProduct, xy);

  // map1, map2;

  // cap >> frame; // get a new frame from camera
  // cvtColor(frame, edges0, CV_BGR2GRAY);
  /*************************************************
  * These are hardcoded values to remove distortion effect
  * of the camera lens.
  *
  * These values could be generated automatically upon
  * initialization using known markers on the stage space.
  * See opencv's calibration tutorial source code for the
  * xml/yml matrix document generation scheme.
  *************************************************/
  // Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
  // cameraMatrix.at<double>(0,0) = 488.815627795;
  // cameraMatrix.at<double>(0,1) = 0.;
  // cameraMatrix.at<double>(0,2) = 319.5;
  // cameraMatrix.at<double>(1,0) = 0.;
  // cameraMatrix.at<double>(1,1) = 488.81562779465474;
  // cameraMatrix.at<double>(1,2) = 239.5;
  // cameraMatrix.at<double>(2,0) = 0.;
  // cameraMatrix.at<double>(2,1) = 0.;
  // cameraMatrix.at<double>(2,2) = 1.;
  //
  // Mat distCoeffs = Mat::zeros(5, 1, CV_64F);
  // distCoeffs.at<double>(0,0) = -0.69299687973847002;
  // distCoeffs.at<double>(0,1) = 5.3149622417099929;
  // distCoeffs.at<double>(0,2) = 0.;
  // distCoeffs.at<double>(0,3) = 0.;
  // distCoeffs.at<double>(0,4) = -19.2852819604873;
  //
  // initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
  //   getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, edges0.size(), 1, edges0.size(), 0),
  //   edges0.size(), CV_16SC2, map1, map2);

  cap >> frame; // get a new frame from camera
  cvtColor(frame, edges0, CV_BGR2GRAY);
  cap >> frame;
  cvtColor(frame, edges1, CV_BGR2GRAY);
  cap >> frame;
  cvtColor(frame, edges2, CV_BGR2GRAY);

  //after capturing 3 samples to ensure a solid image, we run a canny filter to grab just the edges
  Canny(edges0, edges0, 110, 200, 3);
  Canny(edges1, edges1, 110, 200, 3);
  Canny(edges2, edges2, 110, 200, 3);

  int rows = edges0.size().width;
  int cols = edges0.size().height;

  output = Mat(Size(rows, cols), CV_8U);
// std::cout << "START84" << std::endl;
  for(size_t i = 0; i < cols; i++) {
    for (size_t j = 0; j < rows; j++) {
      output.at<uchar>(i,j) = edges0.at<uchar>(i,j) + edges1.at<uchar>(i,j) + edges2.at<uchar>(i,j);
    }
  }

  Mat element = getStructuringElement(MORPH_DILATE,
    Size(11, 11),
    Point(5, 5));
    dilate(output, output, element);

    /*
    * Begin filling in silhouettes
    */
    tmpA = output.clone();
    tmpB = output.clone();
// std::cout << "START101" << std::endl;
    for(size_t i = 1; i < cols; i++) {
      for(size_t j = 1; j < rows; j++){
        if(tmpA.at<uchar>(i-1,j-1) && tmpA.at<uchar>(i-1, j) && tmpA.at<uchar>(i,j-1)) {
          tmpA.at<uchar>(i,j) = 255;
        }
      }
    }

// std::cout << "START110" << std::endl;
    for(size_t i = cols-2; i > 0; i--) {
      for(size_t j = rows-2; j > 0; j--){
        if(tmpB.at<uchar>(i+1,j+1) && tmpB.at<uchar>(i+1, j) && tmpB.at<uchar>(i,j+1)) {
          tmpB.at<uchar>(i,j) = 255;
        }
      }
    }
// std::cout << "START118" << std::endl;
    for(size_t i = 1; i < cols-1; i++) {
      for(size_t j=1;j<rows-1;j++)
      {
        output.at<uchar>(i,j) = tmpA.at<uchar>(i,j) && tmpB.at<uchar>(i,j) ? 255 : 0;
      }
    }

    /***************Silhouettes filled in*************/
    // std::cout << "START127" << std::endl;
    cols = output.size().width;
    rows = output.size().height;

    int far = 0;
    int near = 0;
    int newFar = 0;
    int newNear = 0;

    // rescale along z-axis (up and down each column) directly into 2dvector
    for(int i = 0; i <= cols - 1; i++) {
      far = 0;
      near = 0;
      // printf("%d\n", i);
      for(int j = 0; j <=  rows - 1; j++) {
        if(output.at<uchar>(j,i) == 255) {
          far = j;
          // printf("near: %d\n",near);
          while(output.at<uchar>(j,i) == 255 && j <= rows - 1){
            j++;
          }
          // std::cout << "START148" << std::endl;
          if(j <= rows - 1) {
            near = --j;
            newFar = far - round(0.5*(near - far)*lateralProduct->magMap.at(i));
            newNear = near + round(0.5*(near - far)*lateralProduct->magMap.at(i));
            if(newFar < 0) {
              newFar = 0;
            }
            // printf("%d,%d\n", newFar, newNear);
            while(newFar <= newNear && newFar <= (rows - 1)) {
              lateralProduct->rescaledXY.at(j).at(newFar++) = 1;
            }
          }
          far = 0;
          near = 0;
        } else {
          lateralProduct->rescaledXY.at(j).at(i) = 0;
        }
      }

      // width rescale
      int left = 0;
      int right = 0;
      int newRight = 0;
      int newLeft = 0;
      int midpoint = 0;

      for(int i = 0; i <= rows - 1; i++) {
        for(int j = 0; j <= cols - 1; j++) {
          if(lateralProduct->rescaledXY.at(i).at(j) == 1) {
            left = j++;
            while(j <= (cols - 1) && lateralProduct->rescaledXY.at(i).at(j) == 1) {
              j++;
            }
            right = --j;
            midpoint = round(0.5*(right-left));
            newRight = right + round(0.5*(right - left)*lateralProduct->magMap.at(midpoint));
            newLeft = left - round(0.5*(right - left)*lateralProduct->magMap.at(midpoint));
            if (newLeft < 0) newLeft = 0;
            if(newLeft < left) {
              // printf("newLeft %d\n", newLeft);
              while(newLeft < left) {
                lateralProduct->rescaledXY.at(i).at(newLeft) = 1;
                newLeft++;
              }
            }
            if(newRight < right) {
              // printf("newRight %d--", newRight);
              while(right <= newRight && right <= (cols - 1)) {
                lateralProduct->rescaledXY.at(i).at(right) = 1;
                right++;
              }
            }
          }
          left = 0;
          right = 0;
          newRight = 0;
          newLeft = 0;
          midpoint = 0;
        }
      }
      // Create projection matrix for top-down camera
      renderOffset(lateralProduct, xz);
      // Free up memory as we no longer need this struct--while a lot of opencv objects have smart wrappers, any structs we bring in need to be manually collected
      delete lateralProduct;
    }
  }

  void renderOffset(lateralImageProduct* a, MappingVector2d* b) {
  enum {FREE_SPACE = 0, OCCUPIED};
  int rows = a->rescaledXY.size();
  int cols = a->rescaledXY.at(0).size();
  b->dimOffset.resize(rows);
  // printf("%d,%d\n", rows, cols);
  for(int i=0; i < rows; i++) {
    // printf("%d\n",i);
    for(int j=0; j < cols; j++) {
      b->dimOffset.at(i).resize(cols);
      // printf("%d\n",j);
      if(a->rescaledXY.at(i).at(j) == FREE_SPACE){
        b->dimOffset.at(i).at(j).xOffset = FREE_SPACE;
        b->dimOffset.at(i).at(j).yzOffset = FREE_SPACE;
      } else {
        int temp = j-1;

        while(temp + 1 < cols && a->rescaledXY.at(i).at(++temp)==OCCUPIED);
        //find where does it start to get unoccupied
        if(temp + 1 == cols)//hit the end of the boundary
        {
          int k = j;
          for(; j < cols; j++){
            b->dimOffset.at(i).at(j).xOffset = k - j - 1;
          }
        } else {
          int k=j;
          for(; j < temp; j++){
            b->dimOffset.at(i).at(j).xOffset = std::abs(k - j - 1) < temp - j ? k - j - 1 : temp - j;
          }
        }
      }//close else loop for the case of element is occupied space
    }//close the loop for finding x offset
  }
}

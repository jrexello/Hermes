/*
 * ImgFunctions.hpp
 *
 *  Created on: 15 jul. 2017
 *      Author: root
 */

#ifndef IMGFUNCTIONS_HPP_
#define IMGFUNCTIONS_HPP_

#include "Main.hpp"

using namespace cv;

///Main function. Executes all the different modifications to the image.
struct linkedMat *full(Mat in_mat);

///Loads the img in MAT format.
Mat loadInColor();
Mat toHSV(Mat in_img);
Mat dilate(Mat in_img);
struct linkedMat *detectCircles(Mat in_img, Mat original_img);
Mat crop(Mat in_img, int in_x, int in_y);
Pix *mat2Pix(Mat *in_mat);
Mat pix2Mat(Pix *in_pix);



#endif /* IMGFUNCTIONS_HPP_ */

/*
 * OCR.hpp
 *
 *  Created on: 3 sept. 2017
 *      Author: root
 */

#ifndef OCR_HPP_
#define OCR_HPP_

/*#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>*/
#include "../hpp/ImgFunctions.hpp"
#include <iostream>

using namespace cv;

Mat adaptImg(Mat in_mat);
int recoNumber(Mat in_mat);
bool OCRCompare(char *in_string);


#endif /* OCR_HPP_ */

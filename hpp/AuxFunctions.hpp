/*
 * Aux.hpp
 *
 *  Created on: 31 mar. 2017
 *      Author: Jaime Revello
 */

#ifndef AUXFUNCTIONS_HPP_
#define AUXFUNCTIONS_HPP_

#include "Main.hpp"
#include <sys/stat.h>

cv::Mat loadImgColor(std::string);

cv::Mat loadAndShowImg(std::string in_title, int in_modo);

bool checkConf();



#endif /* AUXFUNCTIONS_HPP_ */

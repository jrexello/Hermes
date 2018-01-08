/*
 * Settings.hpp
 *
 *  Created on: 15 jul. 2017
 *      Author: root
 */

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#include <string>
#include "ImgFunctions.hpp"

///The settings struct. It will be called from everywhere to check the given configuration.
struct settings_st{
	bool draw;
	std::string path;
};

///Mat linked list. To store multiple mats
struct linkedMat{
	cv::Mat image;
	linkedMat *next;
};


///Custom exception struct
struct hException : public std::exception
{
   std::string s;
   hException(std::string ss) : s(ss) {}
   ~hException() throw () {}
   const char* what() const throw() { return s.c_str(); }
};




#endif /* SETTINGS_HPP_ */

/*
 * IO.hpp
 *
 *  Created on: 20 sept. 2017
 *      Author: Jaime Revello
 */

#ifndef IO_HPP_
#define IO_HPP_

#include "Main.hpp"
#include <opencv2/videoio/videoio.hpp>
#include <string>

using namespace std;


linkedMat *frameCatcherFile(string in_file, int in_delay);
linkedMat *frameCatcher(VideoCapture in_video, int delay);
linkedMat *frameCatcherLive(int in_delay);




#endif /* IO_HPP_ */

/**
 * @file Aux.cpp
 *
 * @brief This file will have all the auxiliary functions we might use.
 *
 * @author Jaime Revello
 *
 * @date 31/03/2017
 *
 */

#include "../hpp/AuxFunctions.hpp"

using namespace cv;
//The OpenCv library namespace.
using namespace std;
//The standard namespace.

//Extern declaration of the configuration struct.
extern struct settings_st conf;

/**
 * @brief Default loader.
 * Loads and shows an image in color. The final running version of Hermes WON'T use this
 * function. It meant for debugging proposes.
 * @param in_title Title of the window
 * @param in_s the path to the image.
 * @returns The Mat variable shown.
 */
Mat loadImgColor(string in_title, string in_s) {

	Mat ret;

	if (in_title.empty()) {
		in_title = "No title";
	}
	//If the file doesn't exists, the function throws an exception.
	if (!boost::filesystem::exists(in_s)) {
		string err("The file " + in_s + " does not exists.");
		throw hException(err);
	}

	else {
		ret = imread(in_s, IMREAD_COLOR); //The image is loaded in color.
		imshow(in_title, ret);
		waitKey(0);
	}

	return ret;

}

/**
 * @brief Image loader.
 *
 * Image loader. Creates a Mat with the path given in the settings and loads it depending in the given number in in_modo
 * The mode is the same as given in the OpenCV documentation.
 *
 * @param in_title Title for the window.
 * @param in_modo The load mode.
 * @returns The Mat element.
 */
Mat loadAndShowImg(string in_title, int in_modo) {
	Mat ret;

	if (conf.path.empty()) {
		throw hException("No path given.");
	}

	else {
		ret = imread(conf.path, in_modo);
	}

	return ret;
}

/**
 * @brief Checks if the information saved in the configuration struct is valid.
 * If the return value is TRUE, the configuration is valid.
 *
 * @returns True if the configuration is valid (There is an input file).
 */
bool checkConf() {
	bool ret = true;

	if (conf.path.empty()) {
		throw hException("No path given.");
	}
	return ret;
}


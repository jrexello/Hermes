/**
 * @file ImgFunctions.cpp
 * @brief File with the image manipulation functions.
 *
 * This file will contain all the functions required to edit the image.
 * However, the image modifications for the OCR is in the OCR file.
 *
 *  @date 15 jul. 2017
 *  @author Jaime Revello
 */

#include "../hpp/Main.hpp"

using namespace std;
using namespace cv;

#define CROP_RADIUS 2.2 //The radius multiplier for the crop function.
#define CROP_POSITION 1 //The x, y position offset for the crop.

extern struct settings_st conf;
linkedMat *lMat;

/**
 * @brief The whole process. If you put an image here, it will return the detected circles.
 * The whole process is:
 * 1- BGR -> HSV Change the color space from BGR (Image in Color) to HSV.
 * 2- Filter bny red. We ignore all the non-red elements from the image.
 * 3- Search for circles.
 * @return the first linkedMat from the linked list of circles.
 */
linkedMat *full(Mat in_mat) {

	//Asuming the input is a mat of the road in color.
	Mat original_mat = in_mat.clone();
	return detectCircles(toHSV(in_mat), original_mat);
}

/**
 * @brief Loads the image in the conf path into a new Mat in color and returns it.
 * This function reads the path given by the configuration struct and creates a Mat.
 * If there is no path in the struct, it will throw an exception.
 * @returns The Mat with the image.
 */
Mat loadInColor() {
	checkConf();
	Mat imgColor = imread(conf.path, IMREAD_COLOR);
	if (imgColor.empty())
		throw hException("Mat is empty.");
	return imgColor;

}

/**
 * @brief Transforms the image to HSV and shows only the red color.
 *
 * We need to detect the signals in almost any situation.
 * To do this, first of all, we select only the red "things" in the image, to filter.
 * To do so, we gonna use the HSV pallete of colors, where the "red" is H between 0 and 10 and between 170 and 180.
 * @param in_img The input image in Mat format.
 * @returns A Mat with the image in HSV showing only the red things in the image.
 */
Mat toHSV(Mat in_img) {
	Mat imgHSV; //The HSV converted version
	Mat imgFilt; //The filtered version
	Mat red_mat1, red_mat2; //The detected red objects
	cvtColor(in_img, imgHSV, COLOR_BGR2HSV);
	if (conf.draw) {
		imshow("En HSV", imgHSV);
		waitKey(0);
	}
	if (imgHSV.empty())
		throw hException("Error converting to HSV. Mat is empty.");
	///Now we filter to get ONLY the red sections of the image.
	inRange(imgHSV, Scalar(0, 120, 0, 0), Scalar(10, 255, 255, 0), red_mat1);
	inRange(imgHSV, Scalar(170, 120, 0, 0), Scalar(180, 255, 255, 0), red_mat2);
	//Now we merge both filtered images and returns it.
	imgFilt = red_mat1 + red_mat2;
	return imgFilt;
}

/**
 * @deprecated Not used anymore.
 * @brief Dilates the image.
 *
 * Now we only have the red elements of the image, we need to apply a second filter: The circles.
 * The first step to do this is to dilate the element we found so we make sure to detect the signals
 * even in weird angles.
 * Dilating an image makes all the "tiny stains" or noises dissapear.
 *
 * @param in_img Input Mat.
 * @returns The image dilated.
 */
Mat dilate(Mat in_img) {
	Mat out_img; ///Output Mat
	///First we create a kernel for the dilation.
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(2, 2),
			Point(-1, -1));
	dilate(in_img, out_img, kernel);
	//in_img = out_img.clone();
	if (conf.draw) {
		imshow("dilate", out_img);
		waitKey(0);
	}
	return out_img;
}

/**
 * @brief Detects the circles inside an image.
 * First we apply a Gaussian Blur filter to "smooth" the borders. 
 * After that, using the HOUGH_GRADIENT algorithm, we detect the circles, filtering those with a very small radius
 * and so with the big ones. The recorded signals will always have a certain size, and the algorithm can mess the road
 * and the horizon with a circle, ignoring what's inside it.
 * @param in_img The image with the "detectable" circles.
 * @param original_img The original image. With no filters nor modifications.
 * @returns The pointer to the first linkedMat of detected circles.
 */
linkedMat* detectCircles(Mat in_img, Mat original_img) {
	lMat = 0; //First pointer to linkedMat.
	linkedMat *aux2; //Auxiliary pointer, used for sort.
	linkedMat *aux = lMat; //Auxiliary pointer. Will point to the last linkedMat.
	Mat circles_img = original_img.clone(); //We clone the original image so any change we make to it, won't change the original image.
	vector<Vec3f> circles; ///Vector to save the detected circles.
	Mat gauss; //The Mat with the gaussian blur filter.
	GaussianBlur(in_img, gauss, Size(1, 1), 0, 0);
	if (conf.draw) {
		imshow("Gauss", gauss);
		waitKey(0);
	}
	/*
	 The "core" of the function.
	 This will detect every circle in the image and save the coordinates of it in the vector.
	 */
	HoughCircles(gauss, circles, HOUGH_GRADIENT, 2, gauss.cols / 8, 100, 50, 5,
			0 );
	cout << "\nDetected " << circles.size() << " circles\n";
	///Now make the linked list of detected cricles.
	if (circles.size() > 0) { //If there's ANY circle.
		lMat = new linkedMat;
		lMat->next = 0; //The last linkedMat->next will always point to 0. That's our "stop" signal.
		aux = lMat;
	}

	for (size_t i = 0; i < circles.size(); i++) {
		try { //Some "detected" circles are not big enough to crop them. We add the try to solve it.
			int radius = cvRound(circles[i][2]); //We take the crop radius
			Rect myROI(circles[i][0] - (radius / CROP_POSITION),
					circles[i][1] - (radius / CROP_POSITION),
					(radius * CROP_RADIUS), (radius * CROP_RADIUS));
			aux->image = original_img(myROI);
			if (conf.draw) {
				imshow("Crop", aux->image);
				waitKey(0);
			}
		} catch (exception& e) {

		}
		if (i == circles.size() - 1) //If that was the last detected circle...
			aux->next = 0;
		else {
			aux2 = new linkedMat;
			aux = aux2;
			aux->next = 0;
		}

	}

	return lMat;

}

/**
 * @brief Converts an OpenCv's Mat image to Leptonica's Pix
 *
 * The OCR recognition system we gonna use uses the Pix format from the Leptonica library.
 * This function allow us to convert any Mat (Of 8 bits) to a Pix element.
 *
 * @param in_mat The Mat that's about to be converted.
 * @returns A pix variable with the same input image.
 */
Pix *mat2Pix(Mat *in_mat) {
	Pix *pixd = pixCreate(in_mat->size().width, in_mat->size().height, 8);
	for (int y = 0; y < in_mat->rows; y++) {
		for (int x = 0; x < in_mat->cols; x++) {
			pixSetPixel(pixd, x, y, (l_uint32) in_mat->at<uchar>(y, x));
		}
	}
	return pixd;
}

/**
 * @brief Converts a Leptonica's Pix to OpenCv's Mat
 *
 * On the initial design, this function is not meant to be used.
 * However, if we have a Mat -> Pix function is always recommended to have a Pix -> Mat.
 *
 * @param in_pix The Pix that's about to be converted.
 * @returns A Mat variable with the same input image.
 */
Mat pix2Mat(Pix *in_pix) {

	cv::Mat mat(cv::Size(in_pix->w, in_pix->h), CV_8UC1);
	uint32_t *line = in_pix->data;
	for (uint32_t y = 0; y < in_pix->h; ++y) {
		for (uint32_t x = 0; x < in_pix->w; ++x) {
			mat.at<uchar>(y, x) = GET_DATA_BYTE(line, x);
		}
		line += in_pix->wpl;
	}
	return mat;
}


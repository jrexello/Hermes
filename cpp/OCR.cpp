/*
 * OCR.cpp
 *
 *  Created on: 3 sept. 2017
 *      Author: root
 */

#include "../hpp/OCR.hpp"

extern struct settings_st conf;

/**
 *@brief Adapts and image to be readed by the OCR. Is suposed the image has numbers in it.
 * Before using the OCR we must be sure we have an image "readable".
 * The best case scenario is a black image with only the numbers in white on it.
 * This function tries to make that happen.
 * @param in_mat The Mat with the numbers in it.
 * @returns A new Mat, filtered and ready to be read.
 *
 */
Mat adaptImg(Mat in_mat) {
	//On this funtion we gonna use multiple filters, so we need multiple Mats variables.
	Mat black_mat, imgHSV, ero_mat, baw_mat, red_mat2;
	/*The first step is to transform the image to HSV.
	 * In theory, the input is the image of a traffict signal.
	 * We gonna erase the red from it to simplify the read function.
	 */
	cvtColor(in_mat, imgHSV, COLOR_BGR2HSV);
	if (conf.draw) {
		imshow("MiniHSV", imgHSV);
		waitKey(0);
	}
	/*
	 * We are going to use also a Black and white filter.
	 * This filter is used to clarify the differences between black elements (The numbers)
	 * and white elements (The background). Is expected to get the input from a low resolution
	 * camera, so it's important to "clean" the image.
	 */
	cvtColor(in_mat, baw_mat, COLOR_BGR2GRAY);
	if (conf.draw) {
		imshow("Black and white", baw_mat);
		waitKey(0);
	}
	/*
	 * With this we make the black things completely black and the white things completely white.
	 */
	threshold(baw_mat, black_mat, 150, 255, THRESH_BINARY_INV);

	/*
	 * This part is for the red color. We detect it and make a new filter were only the red elements are drawn.
	 */
	Mat red_image;
	inRange(imgHSV, Scalar(0, 80, 0), Scalar(10, 255, 255), red_image);
	inRange(imgHSV, Scalar(175, 0, 0), Scalar(180, 255, 255), red_mat2);
	//inRange(imgHSV, Scalar(0, 0, 120), Scalar(180, 120, 255), white_mat);
	red_image = red_image + red_mat2;
	if (conf.draw) {
		imshow("Thresholded", black_mat);
		imshow("red1", red_image);
		imshow("red2", red_mat2);
		imshow("redone", red_image);
		waitKey(0);
	}

	/*
	 * Now we substract the red elements from the black-filtered image.
	 * This way, the final image only has the black elements. AKA it has just the numbers.
	 */
	Mat filtered;
	subtract(black_mat, red_image, filtered);
	/*
	 * Light and shadows from the original image leads some stains or noise in the filtered image.
	 * Eroding it, the image get cleaner, however the numbers may become less readable.
	 * Erode the number makes it "shrink"; after eroding it we dilate, so we fix the shrink.
	 * However this may not be the best thing to do, every modification in the number makes it harder to read.
	 */
	//Declaring the kernel...
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(2, 2),
			Point(-1, -1));
	erode(filtered, filtered, kernel, Point(-1, -1), 1);
	dilate(filtered, filtered, kernel, Point(-1, -1), 1);
	/*
	 * The final step is apply a GaussianBlur filter to make the contours "smoothers". Helps reading.
	 */
	GaussianBlur(filtered, filtered, Size(1, 1), 0, 0);
	if (conf.draw) {
		imshow("filtered", filtered);
		waitKey(0);
	}

	return filtered;
}

int recoNumber(Mat in_mat) {
	int ret = 0;
	if (conf.draw) {
		imshow("Imagen a leer", in_mat);
		waitKey(0);
	}

	char *outText;
	//First of all, we check the Tesseract API will work.
	tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
	//Initialized the Tess API with english dictionary.
	if (ocr->Init(NULL, "num")) {
		throw hException("Tesseract is not initilized.");
	}

	in_mat = adaptImg(in_mat);

	//Convert the Mat to Pix
	Mat *p_mat = &in_mat;
	Pix *img = mat2Pix(p_mat);

	ocr->SetImage(img);
	//Get the result
	outText = ocr->GetUTF8Text();
	//Cleaning the number. The OCR gives \n by default.
	char *p = strchr(outText, '\n');  // finds first, if any, \n
	if (p != NULL) *p = '\0';

	if(OCRCompare(outText)) {
		printf("\nOCR:\n%s", outText);
		ret = atoi(outText);
	}
	//Clean the memory
	ocr->End();
	pixDestroy(&img);

	return ret;

}
/**
 * @brief Checks that the given number is a valid speed limit.
 * This functions checks if the given number is in the range of valid speed signals 10-120
 * @param in_str The string with the detected number.
 * @returns true if the number is valid.
 */
bool OCRCompare(char *in_str){
	bool ret = false;
	char const *valids[] = {"10", "20", "30", "40", "50", "60", "70", "80", "90", "100", "110", "120"};
	int len = sizeof(valids)/sizeof(valids[0]);
	for(int i=0; i<len; ++i){
		if(!strcmp(in_str, valids[i])){
			ret = true;
			break;
		}
	}

	return ret;
}


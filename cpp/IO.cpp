/**
 * @file IO.cpp
 * @brief This file contains the Input/Output operations with the system as snapshots capture.
 * @date 20 sept. 2017
 * @author Jaime Revello
 */

#include "../hpp/IO.hpp"

extern struct settings_st conf;

/** 
 * @brief Method used to split an video input to different frames.
 *
 * This method is for file inputs only. It takes the video input, 
 * @param in_file The path to the input file.
 * @param in_delay The delay in ms between snapshots.
 * @return A linked list of Mats with each capture.
 *
 */
linkedMat *frameCatcherFile(string in_file, int in_delay) {
	VideoCapture capture(in_file);
	return frameCatcher(capture, in_delay);


}

/**
* @brief Method to catch frames from default camera.
* This Method is used to summon the default camera feed. The capture parameters are sent to the frameCatcher method.
* @param in_delay The delay between snapchots.
* @returns The first linkedMat of Snaps.
*/
linkedMat *frameCatcherLive(int in_delay){
	VideoCapture capture(0);
	return frameCatcher(capture, in_delay);
}

/**
* @brief Catches frames from an VideoCapture parameters.
*
* The "core" function of I/O. Uses the info stored in the VideoCapture input to catch and send the snaps.
* @param in_video The VideoCapture with the capture parameters.
* @param delay The delay between snapshots.
* @returns The first linkedMat of Snaps.
*/
linkedMat *frameCatcher(VideoCapture in_video, int in_delay){
	linkedMat *initMat = 0; // Initial Mat of the linked list.
	linkedMat *matAux = initMat; //Auxiliary Mat
	linkedMat *circleMat; //The linked mat of circles inside a image
	bool iniciado = false; //To check we do not overwrite the first member of the linked list.
	
	if (in_video.isOpened())
		cout << "Video Opened..." << endl;

	//Start catching frames

	try {
		Mat snap;
		cv::_OutputArray oA(snap);
		int i = 0;
		while (in_video.read(oA)) {


			if (i % in_delay == 0) {
				/*
				 * This part inits the linked list.
				 */
				if (!iniciado) {
					initMat = new linkedMat;
					matAux = initMat;
					iniciado = true;
				}
				matAux->image = snap.clone();
				imshow("Snap", matAux->image);
				if( waitKey(10) == 27 ) break; // stop capturing by pressing ESC

				circleMat = full(matAux->image);
				while (circleMat) {
					if (!circleMat->image.empty())
						recoNumber(circleMat->image);
					circleMat = circleMat->next;
				}
				matAux->next = new linkedMat;
				matAux = matAux->next;
				matAux->next = 0;
			}
			i++;
		}

	} catch (Exception &e) {
		throw e;
	}
	in_video.release();

	return initMat;
	}


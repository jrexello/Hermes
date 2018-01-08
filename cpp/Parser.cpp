/*! File with the parser methods.
 *
 * This file will contain the parsing information. To do that, we are going to use the
 * boost library feature "Program Options". The main function will call here at first instance.
 * This file will also handle the configuration struct (Created in main)
 *
 * @author Jaime Revello
 *
 * @date 31/03/2017
 *
 */

#include "../hpp/Parser.hpp"


namespace po = boost::program_options;
using namespace std;
using namespace cv;

extern struct settings_st conf;

/**
 * @brief Parser of the program.
 *
 * It takes the arguments from the commands line, and parser it.
 *
 * The -1, -2, -3... sets at which step the process stops.
 * -full makes the full process.
 * -draw draws the final image.
 * -live Uses the default camera.
 *
 *
 * @param ac The number of arguments given by the user.
 * @param av array of char* with the arguments given
 */
void parse_options(int ac, char* av[]) {

	po::options_description cmd_line("Allowed Options");
	po::options_description od_functions("Available options");
	po::options_description od_config("Available configuration");
	od_functions.add_options()
			("help", "Shows help message")("file,f", po::value<string>(), "Set the image path")
			("full", "Starts the whole process")
			("loadColor,1", "Only executes the load in Color")
			("toHSV,2", "Loads the image and transforms it to HSV")
			("toDilate,3", "Load, HSV and Dilate")
			("toCircles,4", "Load, HSV, Dilate and Detect circles")
			("toOCR,5", "Detects the numbers")
			("frames", "Catch the frames from the file")
			("live,0", "Catchs images from a camera")
			;
	od_config.add_options()("draw", "Activate the drawings (For debug)");
	cmd_line.add(od_functions);
	cmd_line.add(od_config);

	po::variables_map vm;
	po::store(po::parse_command_line(ac, av, cmd_line), vm);
	po::notify(vm);

	if (ac < 2 || vm.count("help")) {
		cout << cmd_line << "\n";
		return;
	}

	//Options

	if (vm.count("draw")) {
		conf.draw = true;
	}

	//Functions
	
	if(vm.count("live")) {
		frameCatcherLive(20);
		exit(1);
	}

	if (vm.count("file")) {
		string miau = (vm["file"].as<string>());
		conf.path = miau;
	}

	if (vm.count("full")) {
		linkedMat *circleMat = full(loadInColor());
		while(circleMat){
		if(!circleMat->image.empty())recoNumber(circleMat->image);
		circleMat = circleMat->next;
							}
	}

	if (vm.count("loadColor")) {
		Mat img = loadInColor();
		if (conf.draw) {
			///If draw is enabled, we show the image.
			imshow("Image Loaded", img);
			waitKey(0);
		}
	}

	if(vm.count("toHSV")) {
		Mat img = loadInColor();
		Mat imgHSV = toHSV(img);
		if (conf.draw) {
					///If draw is enabled, we show the image.
					imshow("Image in HSV", imgHSV);
					waitKey(0);
				}
	}

	if (vm.count("toDilate")) {
		Mat img = loadInColor();
		Mat imgHSV = toHSV(img);
		Mat imgDil = dilate(imgHSV);
		if (conf.draw) {
			///If draw is enabled, we show the image.
			imshow("Image dilated", imgDil);
			waitKey(0);
		}
	}

	if (vm.count("toCircles")) {
		Mat img = loadInColor();
		Mat imgHSV = toHSV(img);
		Mat imgDil = dilate(imgHSV);
		linkedMat *lMat = detectCircles(imgDil, img);
		if (conf.draw) {
			///If draw is enabled, we show the image.
			while(lMat){
			imshow("Image with detected circles", lMat->image);
			waitKey(0);
			lMat = lMat->next;
			}
		}
	}

	if (vm.count("toOCR")) {
			Mat img = loadInColor();
			Mat imgHSV = toHSV(img);
			Mat imgDil = dilate(imgHSV);
			linkedMat *lMat = detectCircles(imgDil, img);
			while(lMat){
			if (conf.draw) {
				///If draw is enabled, we show the image.
				imshow("Image with detected circles", lMat->image);
				waitKey(0);
				lMat = lMat->next;
			}
			}
		}

	if (vm.count("frames")) {
		frameCatcherFile(conf.path, 20);
		}



	return;
}


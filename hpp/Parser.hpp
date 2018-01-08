/**
 * @file Parser.hpp
 *
 * The parser header. We will only hold the functions references and the includes.
 *
 * @author Jaime Revello
 *
 * @date 31/03/2017
 */
#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <iostream>
#include "../hpp/Main.hpp"
#include <boost/program_options.hpp>
#include <opencv2/opencv.hpp>
#include "../hpp/AuxFunctions.hpp"
#include "../hpp/Settings.hpp"
#include "../hpp/ImgFunctions.hpp"
#include "../hpp/OCR.hpp"
#include "IO.hpp"

void parse_options(int, char*[]);



#endif /* PARSER_HPP_ */

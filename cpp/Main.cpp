/**
 * @file Main.cpp
 *
 * The main function. The idea is to keep the least code as possible.
 * This file will also keep the configuration struct, but the parser will modify it.
 *
 * @brief Main function
 *
 * @author Jaime Revello
 *
 * @date 31/03/2017
 */

#include "../hpp/Main.hpp"
#include "../hpp/Parser.hpp"
#include "../hpp/Settings.hpp"
#include <iostream>

///We declare the shared variables here.
struct settings_st conf;
struct settings_st* p_conf;

/**
 * @brief Main function. It made the basic checks (if any) and calls to parser.
 *
 * This initial function calls to the parser, who is going to decide which image function call to.
 * It also handles the possible exceptions.
 *
 */
int main(int argc, char* argv[]) {
	int ret = 0;
	try {
		parse_options(argc, argv);
	} catch (hException& e) {
		std::cout << "Hermes: An error occurred:\n" << e.what() << "\n";
		ret = -1;
	} catch (std::exception& e) {
		std::cout << "Hermes: An unexpected error occurred:\n" << e.what()
				<< "\n";
		ret = -1;
	}
	return ret;
}


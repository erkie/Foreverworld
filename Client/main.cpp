#include <iostream>

#include <guichan/guichan.hpp>
#include <guichan/sfml.hpp>

#include "Gaem/gaem.h"

using namespace std;

int main(int argc, char **argv)
{
	Gaem::Gaem *game;
	try {
		game = new Gaem::Gaem;
		game->init();
		game->main();
	}
	catch (Gaem::NonFatalException e)
	{
		game->errorMain(e.getMessage());
	}
	catch (Gaem::Exception e)
	{
		std::cerr << "Fatal error: " << e.getMessage();
		std::cerr << " in file " << e.getFilename();
		std::cerr << " on line " << e.getLine();
		std::cerr << " in function " << e.getFunction();
		std::cerr << std::endl;
	}
	
    return EXIT_SUCCESS;
}

#include <iostream>
#include <sstream>

#include "Gaem/gaem.h"
#include "Common/messages.h"

using namespace std;

int main(int argc, char **argv)
{
	Gaem::Gaem *game;
	//try {
		game = new Gaem::Gaem;
		game->init();
		game->main();
	try {
	}
	catch (Gaem::NonFatalException e)
	{
		std::stringstream ss;
		ss << "(" << e.getFilename() << " " << e.getLine() << ")";
		std::cout << e.getMessage() << " " << ss.str() << std::endl;
		game->errorMain(e.getMessage() + " " + ss.str(), e);
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

/*
 *  main.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-05.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#include "server.h"

int main(int argc, char *argv[])
{
	Server server(20, 1000);
	server.run();
	
	return 0;
}

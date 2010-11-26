/*
 *  main.cpp
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-05.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 *  SOURCES
 *  http://developer.valvesoftware.com/wiki/Latency_Compensating_Methods_in_Client/Server_In-game_Protocol_Design_and_Optimization
 *  http://developer.valvesoftware.com/wiki/Source_Multiplayer_Networking
 *  http://www.gamasutra.com/view/feature/3230/dead_reckoning_latency_hiding_for_.php
 *  http://www.gamedev.net/reference/articles/article1370.asp
 */

#include "server.h"

int main(int argc, char *argv[])
{
	Server server(6010, 1000);
	server.run();
	
	return 0;
}

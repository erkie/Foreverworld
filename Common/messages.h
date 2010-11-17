/*
 *  messages.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-05.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __COMMON_MESSAGES_H__
#define __COMMON_MESSAGES_H__

#include <SFML/System.hpp>
#include "MessageIdentifiers.h"

namespace inet
{
	typedef unsigned long id_type;

	enum Message
	{
		MESS_START_GAME = ID_USER_PACKET_ENUM+1,
		MESS_SUCCESSFULLY_ADDED,
		MESS_NEW_PLAYER,
		MESS_REMOVED_PLAYER,
		MESS_PLAYER_STATE_PING,
		MESS_EVENT
	};
	
	enum Event
	{
		EVENT_RUN,
		EVENT_JUMP,
		EVENT_CHANGE_DIR,
		EVENT_DEFENCE,
		EVENT_ATTACK1,
		EVENT_ATTACK2
	};
	
	enum PlayerActionState
	{
		STATE_WAITING,
		STATE_RUNNING,
		STATE_JUMPING,
		STATE_DEFENDING,
		STATE_ATTACKING1,
		STATE_ATTACKING2
	};
	
#pragma pack(push, 1)
	struct Packet
	{
		unsigned char type;
	};
	
	struct Player
	{
		int dir[2];
		int left;
		float depth;
		float elevation;
		float velocity[2];
		char character[50];
	};
	
	struct PlayerState
	{
		int dir[2];
		int left;
		float depth;
		float elevation;
		float velocity[2];
		PlayerActionState state;
	};
	
	// Tell the server what player I am so I can get my unique ID back
	struct StartGame: Packet
	{
		char character[50];
	};
	
	// I have been added to the game and my ID is back
	struct SuccessfullyAdded: Packet
	{
		id_type id;
		Player player;
	};
	
	// A player has joined the game (it could even be me!)
	struct PlayerAdded: Packet
	{
		id_type id;
		Player player;
	};
	
	// A player has left the game
	struct PlayerRemoved: Packet
	{
		id_type id;
	};
	
	struct EventBasic: Packet
	{
		PlayerState state;
	};
	
	struct EventUpdate: EventBasic
	{
		id_type id;
	};
#pragma pack(pop)

}

#endif

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

#include <stdint.h>
#include "MessageIdentifiers.h"

namespace inet
{
	typedef uint32_t id_type;

#define STR2USERNAME(x, y) memset(x, 0, 21); memcpy(x, std::string(y).c_str(), 20);

	enum Message
	{
		MESS_START_GAME = ID_USER_PACKET_ENUM+1,
		MESS_SUCCESSFULLY_ADDED,
		MESS_NEW_PLAYER,
		MESS_REMOVED_PLAYER,
		MESS_PLAYER_STATE_PING,
		MESS_EVENT,
		MESS_REGISTER_PLAYER,
		MESS_LOGIN_PLAYER,
		MESS_REGISTER_STATUS,
		MESS_LOGIN_STATUS,
		MESS_VERSION_CHECK,
		MESS_CHARACTER_DATA,
		MESS_CHARACTER_UPDATE,
		MESS_NEWS,
		MESS_CHAT_MESSAGE
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
	
	// A member's public data
	struct LoggedInMemberData
	{
		id_type id;
		char username[21];
		int32_t character_id;
	};
	
	struct Version
	{
		short major;
		short minor;
		char type; // 'b' == beta, 's' == stable
	};
	
	struct PlayerState
	{
		int32_t dir[2];
		int32_t left;
		float depth;
		float elevation;
		float velocity[2];
		PlayerActionState state;
	};
	
	struct Character
	{
		int32_t id;
		char name[21];
		float scale;
		bool can_jump;
		int32_t speed;
		int32_t up_speed;
	};
	
	// Register me as a member
	// In the future this might also contain the player I want to be
	struct MemberCredentials
	{
		char username[21];
		char password[41];
		char email[101];
	};
	
	// Log in using username password
	struct LoginCredentials
	{
		char username[21];
		char password[41];
	};
	
	// PACKETS
	// --
	
	struct Packet
	{
		unsigned char type;
	};
	
	struct VersionCheck: Packet
	{
		Version version;
	};
	
	// Generic packet for a YES/NO answer
	struct StatusResponse: Packet
	{
		bool status;
	};
	
	// Login a member with a username/password
	// Responds with a LoginStatus containing my id
	// If if failed the id will be 0
	struct LoginPlayer: Packet
	{
		LoginCredentials login;
	};
	
	// Response to a login, id will be 0 on failure
	// This will be shortly followed by a PlayerAdded message
	// If the IDs are the same I know it is my data
	struct LoginStatus: Packet
	{
		id_type id;
	};
	
	// Information about a character
	struct CharacterData: Packet
	{
		Character character;
	};
	
	// A player has joined the game (it could even be me!)
	struct PlayerAdded: Packet
	{
		id_type id;
		PlayerState player;
		LoggedInMemberData member;
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
	
	// Register a new member/player
	// Responds with a MESS_REGISTER_STATUS
	struct RegisterPlayer: Packet
	{
		MemberCredentials member;
	};
	
	// I registered OK, now I can log on
	struct SuccessfullyRegistered: Packet
	{
		bool succeeded;
	};
	
	// Update character
	struct CharacterUpdate: Packet
	{
		LoggedInMemberData member;
	};
	
	// News message
	struct News: Packet
	{
		char posted[20];
		char text[1000];
	};
	
	// Chat message, both ingoing and outgoing
	struct ChatMessage: Packet
	{
		char message[300];
		id_type user_id;
	};
#pragma pack(pop)
	
	inet::Version getVersion();
	std::string getVersionString(const inet::Version &);
}

#endif

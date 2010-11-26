/*
 *  types.h
 *  Spel
 *
 *  Created by Erik Andersson on 2010-11-24.
 *  Copyright 2010 Åva gymnasium. All rights reserved.
 *
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#include <ssqls.h>

namespace sql
{
	sql_create_6(player, 1, 6,
		mysqlpp::sql_int, id,
		mysqlpp::sql_varchar, username,
		mysqlpp::sql_varchar, password,
		mysqlpp::sql_varchar, email,
		mysqlpp::sql_datetime, created,
		mysqlpp::sql_int, character_id
	)

	sql_create_6(character, 1, 6,
		mysqlpp::sql_int, id,
		mysqlpp::sql_varchar, name,
		mysqlpp::sql_float, scale,
		mysqlpp::sql_bool, can_jump,
		mysqlpp::sql_int, speed,
		mysqlpp::sql_int, up_speed
	)
}

#endif

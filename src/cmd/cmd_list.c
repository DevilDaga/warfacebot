/**
* WarfaceBot, a blind XMPP client for Warface (FPS)
* Copyright (C) 2015, 2016 Levak Borok <levak92@gmail.com>
*
* This program is free software: you can redistribute it and/or modify it
* under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include <wb_session.h>
#include <wb_cmd.h>

void cmd_list_add ( const char *fmt, ... )
{
	va_list args;
	char buffer[ 256 ];

	va_start ( args, fmt );
	vsprintf ( buffer, fmt, args );

	for ( unsigned i = 0; i < session.commands.cmd_list_size; ++i )
		if ( !strcmp ( buffer, session.commands.cmd_list[ i ] ) )	/* Duplicate */
			return;

	session.commands.cmd_list = realloc ( session.commands.cmd_list, ( session.commands.cmd_list_size + 1 ) * sizeof ( char* ) );
	session.commands.cmd_list[ session.commands.cmd_list_size ] = strdup ( buffer );
	session.commands.cmd_list_size++;

	va_end ( args );
}
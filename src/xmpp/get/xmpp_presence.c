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

#include <wb_room.h>
#include <wb_tools.h>
#include <wb_stream.h>
#include <wb_session.h>
#include <wb_xmpp.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct args
{
	int leave;
	char *room_jid;
	f_presence_cb cb;
	void *args;
};

static void xmpp_presence_cb_ ( const char *msg,
enum xmpp_msg_type type,
	void *args )
{
	/* Answer :
	   <presence to='xxxxx@warface/GameClient' type='unavailable'>
		<x xmlns='http://jabber.org/protocol/muc#user'>
		 <item affiliation='none' role='none'/>
		</x>
	   </presence>
	 */

	struct args *a = ( struct args * ) args;

	if ( type ^ XMPP_TYPE_ERROR )
	{
		if ( a->leave )
		{
			LOGPRINT ( "%-20s" KRST BOLD " %s\n", "Left room", a->room_jid );
			//xmpp_iq_join_channel(NULL, NULL, NULL);
			room_list_remove ( a->room_jid );
		}
		else
		{
			LOGPRINT ( "%-20s" KRST BOLD " %s\n", "Joined room", a->room_jid );
			room_list_add ( a->room_jid );
		}

		if ( a->cb != NULL )
			a->cb ( a->room_jid, a->leave, a->args );
	}
	else
	{
		if ( a->leave )
			LOGPRINT ( KRED "Failed leaving room  " KRST BOLD "%s\n", a->room_jid );
		else
			LOGPRINT ( KRED "Failed joining room  " KRST BOLD "%s\n", a->room_jid );
	}

	free ( a->room_jid );
	free ( a );
}

void xmpp_presence ( const char *room_jid, int leave,
					 f_presence_cb cb, void *args )
{
	if ( room_jid == NULL )
		return;

	int r = list_get ( session.rooms, room_jid ) != NULL;

	if ( ( leave && !r ) || ( !leave && r ) )
	{
		if ( leave )
			LOGPRINT ( "We wanted to leave a room we were not in: " BOLD
					   "%s\n", room_jid );
		else
			LOGPRINT ( "We wanted to join a room we were already in: " BOLD
					   "%s\n", room_jid );
		return;
	}

	struct args *a = calloc ( 1, sizeof ( struct args ) );

	a->leave = leave;
	a->room_jid = strdup ( room_jid );
	a->cb = cb;
	a->args = args;

	t_uid id;

	idh_generate_unique_id ( &id );
	idh_register ( &id, 0, xmpp_presence_cb_, a );

	if ( leave )
	{
		/* Leave the XMPP room */
		send_stream_format ( session.wfs,
							 "<presence id='%s' to='%s' type='unavailable'/>",
							 &id, room_jid );
	}
	else
	{
		/* Join the XMPP room */
		send_stream_format ( session.wfs,
							 "<presence id='%s' to='%s/%s'/>",
							 &id, room_jid, session.nickname );
	}
}
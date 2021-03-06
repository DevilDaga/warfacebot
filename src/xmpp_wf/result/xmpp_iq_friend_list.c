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

#include <wb_tools.h>
#include <wb_stream.h>
#include <wb_xmpp.h>
#include <wb_xmpp_wf.h>
#include <wb_friend.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void xmpp_iq_friend_list_cb ( const char *msg_id,
									 const char *msg,
									 void *args )
{
	/* Record friends to list
	   <iq from='masterserver@warface/pve_12' type='get'>
		<query xmlns='urn:cryonline:k01'>
		 <friend_list>
		  <friend jid='XXX' profile_id='XXX' nickname='XXX'
			   status='XXX' experience='XXX' location='XXX'/>
		  <friend jid='XXX' profile_id='XXX' nickname='XXX'
			   status='XXX' experience='XXX' location='XXX'/>
		 </friend_list>
		</query>
	   </iq>
	*/

	char *data = wf_get_query_content ( msg );

#if 0
	printf ( "\n\nDECODED:\n%s\n\n", data );
#endif

	//friend_list_empty();
	unsigned int old_friends = session.profile.friends->length;
	unsigned int new_friends = 0;

	const char *m = strstr ( data, "<friend_list" );

	const char *tmp = m + sizeof ( "<friend_list" );
	while ( ( tmp = strstr ( tmp, "<friend" ) ) )
	{
		new_friends++;
		tmp += sizeof ( "<friend" );
	}
	if ( new_friends < old_friends )
		friend_list_empty ( );

	if ( m != NULL )
	{
		m += sizeof ( "<friend_list" );

		while ( ( m = strstr ( m, "<friend" ) ) )
		{
			m += sizeof ( "<friend" );

			char *jid = get_info ( m, "jid='", "'", NULL );
			char *nick = get_info ( m, "nickname='", "'", NULL );
			char *pid = get_info ( m, "profile_id='", "'", NULL );
			int status = get_info_int ( m, "status='", "'", NULL );
			int exp = get_info_int ( m, "experience='", "'", NULL );

			if ( list_get ( session.profile.friends, nick ) )
				friend_list_update ( jid, nick, pid, status, exp, "", "", "", "" );
			else
			{
				if ( jid && *jid )
					if ( !( status & ( STATUS_AFK | STATUS_PLAYING ) ) )
						LOGPRINT ( "Friend: " KGRN BOLD "%s" KWHT "\n", nick );
					else if ( status & STATUS_PLAYING )
						LOGPRINT ( "Friend: " KMAG BOLD "%s" KWHT "\n", nick );
					else
						LOGPRINT ( "Friend: " KYEL BOLD "%s" KWHT "\n", nick );
				else
					LOGPRINT ( "Friend: " KCYN BOLD "%s" KWHT "\n", nick );

				struct friend *f = friend_list_add ( jid, nick, pid, status, exp );

				if ( f->jid )
					xmpp_iq_peer_status_update ( f );
			}

			free ( jid );
			free ( nick );
			free ( pid );
		}
	}

	if ( old_friends != new_friends )
		LOGPRINT ( "Friend count: " KWHT BOLD "%u/50\n", new_friends );

	free ( data );
}

void xmpp_iq_friend_list_r ( void )
{
	qh_register ( "friend_list", 1, xmpp_iq_friend_list_cb, NULL );
}
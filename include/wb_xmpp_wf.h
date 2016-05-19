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

#ifndef WB_XMPP_WF_H
#define WB_XMPP_WF_H

#include <wb_xmpp.h>
#include <wb_friend.h>
#include <wb_clanmate.h>

enum e_notif_type
{
	NOTIF_ACHIEVEMENT = 4,
	NOTIF_UNLOCK_MISSION = 8,
	NOTIF_CLAN_INVITE = 16,
	NOTIF_FRIEND_REQUEST = 64,
	NOTIF_STATUS_UPDATE = 128,
	NOTIF_GIVE_ITEM = 256,
	NOTIF_ANNOUNCEMENT = 512,
	NOTIF_CONS_LOGIN = 2048,
	NOTIF_GIVE_RANDOM_BOX = 8192,
	NOTIF_NEW_RANK = 131072
};

char *wf_get_query_content ( const char *msg );

char *wf_compress_query ( const char *iq );
char *wf_decompress_query ( const char *iq );


void xmpp_connect ( const char *login, const char *pass );
void xmpp_close ( void );


void xmpp_send_message ( const char *to_login, const char *to_jid,
						 const char *msg );

void xmpp_ack_message ( const char *from_login, const char *from_jid,
						const char *msg, const char *answer_id );


/* Sent Queries */

void xmpp_print_number_of_occupants ( int wfs, char *room );
void xmpp_iq_account ( char *login );
void xmpp_iq_create_profile ( void );
void xmpp_iq_confirm_notification ( const char *notif );
void xmpp_iq_get_account_profiles ( void );
void xmpp_iq_get_master_server ( const char *channel );
typedef void ( *f_join_channel_cb )( void *args );
void xmpp_iq_join_channel ( const char *channel, f_join_channel_cb f, void *args );
void xmpp_iq_peer_status_update ( const struct friend *f );
void xmpp_iq_peer_status_update_friend ( const struct friend *f, void *args );
void xmpp_iq_peer_clan_member_update_clanmate ( const struct clanmate *c, void *args );
void xmpp_iq_peer_clan_member_update ( const struct clanmate *c );
typedef void ( *f_peer_player_info_cb )( const char *info, void *args );
void xmpp_iq_peer_player_info ( const char *online_id,
								f_peer_player_info_cb f, void *args );
void xmpp_promote_room_master ( const char *nickname );
void xmpp_iq_player_status ( int status );
typedef void ( *f_profile_info_get_status_cb )( const char *info, void *args );
void xmpp_iq_profile_info_get_status ( const char *nickname,
									   f_profile_info_get_status_cb f,
									   void *args );
void xmpp_iq_gameroom_leave ( void );
void xmpp_iq_gameroom_join ( const char *channel, const char *room_id );
typedef void ( *f_gameroom_open_cb )( const char *room_id, void *args );

enum e_room_type
{
	ROOM_PVE_PRIVATE = 1,
	ROOM_PVP_PUBLIC = 2,
	ROOM_PVP_CLANWAR = 4,
	ROOM_PVP_QUICKPLAY = 8,
	ROOM_PVE_QUICKPLAY = 16,
};

void xmpp_iq_gameroom_open ( const char *mission_key, enum e_room_type type,
							 f_gameroom_open_cb fun, void *args );
void xmpp_iq_remove_friend ( const char *nickname );


typedef void ( *f_list_cb )( struct list *, void *args );
void xmpp_iq_missions_get_list ( f_list_cb fun, void *args );

void xmpp_iq_gameroom_setplayer ( int team_id, int room_status, int class_id,
								  f_id_callback cb, void *args );
void xmpp_iq_gameroom_askserver ( f_id_callback cb, void *args );
void xmpp_iq_invitation_send ( const char *nickname, int is_follow,
							   f_query_callback cb, void *args );
void xmpp_iq_follow_send ( const char *online_id,
						   f_id_callback cb, void *args );
void xmpp_iq_gameroom_setinfo ( const char *mission_key,
								f_id_callback cb, void *args );
void xmpp_iq_gameroom_setname ( const char *room_name,
								f_id_callback cb, void *args );

enum kick_reason
{
	KICK_NOREASON = 0,
	KICK_INACTIVITY = 2,
	KICK_VOTE = 3,
	KICK_RANK = 6,
	KICK_CLAN_ROOM = 7,
	KICK_CHEATING = 8,
	KICK_GAME_VERSION = 9,
	KICK_NOTOKEN = 10
};

void xmpp_iq_gameroom_kick ( unsigned int profile_id,
							 f_id_callback cb, void *args );

void xmpp_iq_send_invitation ( const char *nickname, enum e_notif_type type );

enum pvp_mode
{
	PVP_DEFAULT = 0,
	PVP_PRIVATE = 1 << 0,
	PVP_FRIENDLY_FIRE = 1 << 1,
	PVP_ENEMY_OUTLINES = 1 << 2,
	PVP_AUTOBALANCE = 1 << 3,
	PVP_DEADCHAT = 1 << 4,
	PVP_ALLOWJOIN = 1 << 5
};

void xmpp_iq_gameroom_update_pvp ( const char *mission_key, enum pvp_mode flags,
								   int max_players, int inventory_slot,
								   f_id_callback cb, void *args );

/* Received Queries */

void xmpp_iq_friend_list_r ( void );
void xmpp_iq_peer_status_update_r ( void );
void xmpp_message_r ( void );
void xmpp_iq_gameroom_sync_r ( void );
void xmpp_iq_gameroom_on_kicked_r ( void );
void xmpp_iq_gameroom_offer_r ( void );
void xmpp_iq_gameroom_loosemaster_r ( void );
void xmpp_iq_invitation_request_r ( void );
void xmpp_iq_follow_send_r ( void );
void xmpp_iq_sync_notifications_r ( void );
void xmpp_iq_notification_broadcast_r ( void );
void xmpp_iq_p2p_ping_r ( void );
void xmpp_iq_preinvite_invite_r ( void );
void xmpp_iq_clan_info_r ( void );
void xmpp_iq_peer_clan_member_update_r ( void );
void xmpp_iq_peer_player_info_r ( void );
void xmpp_iq_presence_r ( void );
void xmpp_iq_broadcast_session_result_r ( void );

#define XMPP_WF_REGISTER_QUERY_HDLR()              \
    do {                                            \
                                                    \
        xmpp_iq_friend_list_r();                    \
        xmpp_iq_peer_status_update_r();             \
        xmpp_message_r();                           \
        xmpp_iq_gameroom_sync_r();                  \
        xmpp_iq_gameroom_on_kicked_r();             \
        xmpp_iq_gameroom_offer_r();                 \
        xmpp_iq_gameroom_loosemaster_r();           \
        xmpp_iq_invitation_request_r();             \
        xmpp_iq_follow_send_r();                    \
        xmpp_iq_sync_notifications_r();             \
        xmpp_iq_notification_broadcast_r();         \
        xmpp_iq_p2p_ping_r();                       \
        xmpp_iq_preinvite_invite_r();               \
        xmpp_iq_clan_info_r();                      \
        xmpp_iq_peer_clan_member_update_r();        \
        xmpp_iq_peer_player_info_r();               \
		xmpp_iq_presence_r();						\
		xmpp_iq_broadcast_session_result_r();		\
    } while (0);

#endif /* !WB_XMPP_WF_H */

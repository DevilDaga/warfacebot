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

#ifndef WB_SESSION_H
#define WB_SESSION_H

#include <wb_list.h>

#include <stdio.h>
#include <time.h>

enum e_status
{
	STATUS_OFFLINE = 0,
	STATUS_ONLINE = 1 << 0,
	STATUS_LEFT = 1 << 1,
	STATUS_AFK = 1 << 2,
	STATUS_LOBBY = 1 << 3,
	STATUS_ROOM = 1 << 4,
	STATUS_PLAYING = 1 << 5,
	STATUS_SHOP = 1 << 6,
	STATUS_INVENTORY = 1 << 7
};

enum e_clan_role
{
	CLAN_MASTER = 1,
	CLAN_OFFICER = 2,
	CLAN_MEMBER = 3
};

enum e_class
{
	CLASS_RIFLEMAN = 0,
	CLASS_SNIPER = 2,
	CLASS_MEDIC = 3,
	CLASS_ENGINEER = 4
};

struct session
{
	int wfs;
	char active;

	struct
	{
		char *jid;
		time_t last_query;
		struct list *rooms;
	} xmpp;

	struct
	{
		char *id;
		char *active_token;
		char *channel;
	} online;

	struct
	{
		struct list *missions;
	} wf;

	struct
	{
		char *jid;
		char *group_id;
		int curr_team;
		int is_safemaster;
		char leaving;
		char joined;
	} gameroom;

	struct
	{
		char *id;
		char *nickname;
		unsigned int experience;
		enum e_class curr_class;
		struct list *friends;
		enum e_status status;
		unsigned int coins;
		unsigned int login_streak;

		struct
		{
			unsigned int game;
			unsigned int crown;
			unsigned int cry;
		} money;

		char *primary_weapon;

		struct
		{
			unsigned int badge;
			unsigned int mark;
			unsigned int stripe;
		} banner;

		struct
		{
			unsigned int items_unlocked;
			unsigned int challenges_completed;
			unsigned int playtime_seconds;
			float leavings_percentage;

			struct
			{
				unsigned int rating_points;
				unsigned int wins;
				unsigned int loses;
				unsigned int kills;
				unsigned int deaths;
				unsigned int favorite_class; /* TODO */
			} pvp;

			struct
			{
				unsigned int missions_completed;
				unsigned int climbs_performed; /* TODO */
				unsigned int assists_performed; /* TODO */
				unsigned int favorite_class; /* TODO */
			} pve;
		} stats;
	} profile;

	struct 
	{
		unsigned int id;
		char *name;
		unsigned int joined;
		enum e_clan_role role;
		unsigned int points;
		int own_position;
		int leaderboard_position;
		struct list *clanmates;
	}clan;

	struct 
	{
		char farming;
		char silent;
		char notify;
		char *whitelist;
		char **cmd_list;
		unsigned int cmd_list_size;
	}commands;

	struct
	{
		FILE *output;
		FILE *debug;
	}log;
};

extern struct session session;

void session_init ( int fd, FILE *fLog, FILE *fDebug );
void session_free ( void );

#endif /* WB_SESSION_H */

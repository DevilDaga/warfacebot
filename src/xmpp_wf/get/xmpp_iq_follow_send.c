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
#include <wb_session.h>

#include <stdlib.h>
#include <stdio.h>

static void xmpp_iq_invitation_result_cb(const char *msg_id,
                                         const char *msg,
                                         void *args)
{
    /* Answer:
       <iq from='xxxxx@warface/GameClient' id='uid0002d87c' type='get'>
        <query xmlns='urn:cryonline:k01'>
         <invitation_result result='17' user='xxxxxxx' is_follow='1' user_id='xxxxxx'/>
        </query>
       </iq>
     */

    const char *reason = "Unknown reason";

    if (msg != NULL)
    {
        int result = get_info_int(msg, "result='", "'", NULL);

        switch (result)
        {
            case 6:
                reason = "User not in a room";
                break;
            case 14:
                reason = "Room is full";
                break;
            case 17:
                reason = "Room is private";
                break;
            default:
                break;
        }
    }

    LOGPRINT(KRED "Failed to follow " KRST BOLD "(%s).\n", reason);
}

void xmpp_iq_follow_send(const char *online_id, f_id_callback cb, void *args)
{
    t_uid id;

    idh_generate_unique_id(&id);
    idh_register(&id, 0, cb, args);

    qh_register("invitation_result", 0, xmpp_iq_invitation_result_cb, NULL);

    send_stream_format(session.wfs,
                       "<iq id='%s' to='%s' type='get'>"
                       " <query xmlns='urn:cryonline:k01'>"
                       "  <follow_send nickname='%s' profile_id='%s'/>"
                       " </query>"
                       "</iq>",
                       &id, online_id, session.nickname, session.profile_id);
}

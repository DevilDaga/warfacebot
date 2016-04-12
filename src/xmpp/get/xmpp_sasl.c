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

#include <wb_stream.h>
#include <wb_session.h>
#include <wb_xmpp.h>

struct cb_args
{
	f_sasl_cb f;
	void *args;
};

static void xmpp_sasl_cb_ ( const char *msg_id, const char *msg, void *args )
{
	/* Answer :
	   <success xmlns="urn:ietf:params:xml:ns:xmpp-sasl"/>
	 */

	struct cb_args *a = ( struct cb_args * ) args;

	send_stream_format ( session.wfs,
						 "<stream:stream to='warface'"
						 " xmlns='jabber:client'"
						 " xmlns:stream='http://etherx.jabber.org/streams'"
						 " xml:lang='en' version='1.0'>" );

	xmpp_bind ( "GameClient", a->f, a->args );

	free ( a );
}

void xmpp_sasl ( const char *login, const char *password,
				 f_sasl_cb cb, void *args )
{
	if ( login == NULL || password == NULL )
		return;

	struct cb_args *a = calloc ( 1, sizeof ( struct cb_args ) );

	a->f = cb;
	a->args = args;

	qh_register ( "success", 0, xmpp_sasl_cb_, a );

	/* SASL Authentification */

	char *logins_b64 = sasl_combine_logins ( login, password );

	send_stream_format ( session.wfs,
						 "<auth xmlns='urn:ietf:params:xml:ns:xmpp-sasl'"
						 " mechanism='WARFACE'>"
						 "%s"
						 "</auth>",
						 logins_b64 );

	free ( logins_b64 );
}
/*
 *
 * Copyright (c) 2011-2016 The University of Waikato, Hamilton, New Zealand.
 * All rights reserved.
 *
 * This file is part of libprotoident.
 *
 * This code has been developed by the University of Waikato WAND
 * research group. For further information please see http://www.wand.net.nz/
 *
 * libprotoident is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * libprotoident is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

#include <string.h>

#include "libprotoident.h"
#include "proto_manager.h"
#include "proto_common.h"

static inline bool match_spdy_syn(uint32_t payload) {

        if (MATCH(payload, 0x80, 0x03, 0x00, 0x01))
                return true;
        return false;

}

static inline bool match_spdy_settings(uint32_t payload) {

        if (MATCH(payload, 0x80, 0x03, 0x00, 0x04))
                return true;
        return false;

}

static inline bool match_spdy_syn_reply(uint32_t payload) {

        if (MATCH(payload, 0x80, 0x03, 0x00, 0x02))
                return true;
        return false;

}

static inline bool match_spdy_ping(uint32_t payload, uint32_t len){

  	if (MATCH(payload, 0x80, 0x03, 0x00, 0x06) && len == 12)
    		return true;
  	return false;

}



static inline bool match_spdy(lpi_data_t *data, lpi_module_t *mod UNUSED) {

        if (match_spdy_syn(data->payload[0])) {
                if (match_spdy_settings(data->payload[1]))
                        return true;
                if (match_spdy_syn_reply(data->payload[1]))
                        return true;
        }

        if (match_spdy_syn(data->payload[1])) {
                if (match_spdy_settings(data->payload[0]))
                        return true;
                if (match_spdy_syn_reply(data->payload[0]))
                        return true;
        }

	if (match_spdy_settings(data->payload[0]) && data->payload_len[0] == 28){
		if (match_spdy_ping(data->payload[1], data->payload_len[1]))
			return true;
	}

	return false;
}

static lpi_module_t lpi_spdy = {
	LPI_PROTO_SPDY,
	LPI_CATEGORY_WEB,
	"SPDY",
	10,
	match_spdy
};

void register_spdy(LPIModuleMap *mod_map) {
	register_protocol(&lpi_spdy, mod_map);
}


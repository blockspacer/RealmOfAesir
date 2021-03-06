/*
    Realm of Aesir backend
    Copyright (C) 2016  Michael de Lang

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "gateway_get_characters_response_handler.h"
#include <macros.h>
#include <easylogging++.h>
#include <messages/error_response_message.h>

using namespace std;
using namespace roa;

gateway_get_characters_response_handler::gateway_get_characters_response_handler(Config config)
        : _config(config) {

}

void gateway_get_characters_response_handler::handle_message(std::unique_ptr<binary_message const> const &msg,
                                                       STD_OPTIONAL<std::reference_wrapper<user_connection>> connection) {
    if(unlikely(!connection)) {
        LOG(ERROR) << NAMEOF(gateway_get_characters_response_handler::handle_message) << " received empty connection";
        return;
    }

    if (auto response_msg = dynamic_cast<binary_get_characters_response_message const *>(msg.get())) {
        LOG(DEBUG) << NAMEOF(gateway_get_characters_response_handler::handle_message) << " Got response message from backend";

        for(auto& plyr : response_msg->players) {
            connection->get().player_characters.push_back({plyr.player_id, response_msg->sender.server_origin_id, plyr.player_name, plyr.map_name, response_msg->world_name});
        }

        json_get_characters_response_message response{{false, 0, 0, 0}, response_msg->players, response_msg->world_name};
        auto response_str = response.serialize();
        connection->get().ws->send(response_str.c_str(), response_str.length(), uWS::OpCode::TEXT);
    } else {
        LOG(ERROR) << NAMEOF(gateway_get_characters_response_handler::handle_message) << " Couldn't cast message to get_characters_response_message";
        json_error_response_message response{{false, 0, 0, 0}, -1, "Something went wrong."};
        auto response_str = response.serialize();
        connection->get().ws->send(response_str.c_str(), response_str.length(), uWS::OpCode::TEXT);
    }
}

uint32_t constexpr gateway_get_characters_response_handler::message_id;

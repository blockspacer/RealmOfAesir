/*
    Realm of Aesir
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

#include "login_message.h"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <sstream>

using namespace std;
using namespace roa;

template <bool UseJson>
login_message<UseJson>::login_message(message_sender sender, string username, string password, std::string ip) noexcept
        : message<UseJson>(sender), username(username), password(password), ip(ip) {}

template <bool UseJson>
login_message<UseJson>::~login_message() noexcept {

}

template <bool UseJson>
string const login_message<UseJson>::serialize() const {
    stringstream ss;
    {
        typename conditional<UseJson, cereal::JSONOutputArchive, cereal::BinaryOutputArchive>::type archive(ss);

        archive(cereal::make_nvp("id", login_message<UseJson>::id),
                cereal::make_nvp("sender", this->sender),
                cereal::make_nvp("username", this->username),
                cereal::make_nvp("password", this->password),
                cereal::make_nvp("ip", this->ip));
    }

    return ss.str();
}

template<bool UseJson> uint32_t constexpr login_message<UseJson>::id;
template class login_message<false>;
template class login_message<true>;
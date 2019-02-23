/*
    gebaar
    Copyright (C) 2019   coffee2code
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GEBAAR_CONFIG_H
#define GEBAAR_CONFIG_H

#include <cpptoml.h>
#include <filesystem>
#include <pwd.h>
#include <iostream>

namespace gebaar::config {
    class Config {
    public:
        Config();

        bool loaded = false;

        void load_config();

        std::string swipe_three_commands[10];
        std::string swipe_four_commands[10];
        std::string pinch_in_command;
        std::string pinch_out_command;

    private:
        bool config_file_exists();

        bool find_config_file();

        std::string config_file_path;
        std::shared_ptr<cpptoml::table> config;
    };
}
#endif //GEBAAR_CONFIG_H

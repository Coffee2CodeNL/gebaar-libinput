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

        std::string swipe_four_up_command;
        std::string swipe_four_down_command;
        std::string swipe_four_left_command;
        std::string swipe_four_right_command;
        std::string swipe_three_up_command;
        std::string swipe_three_down_command;
        std::string swipe_three_left_command;
        std::string swipe_three_right_command;
    private:
        bool find_config_file();

        bool find_home_folder();

        std::string config_file_path;
        std::shared_ptr<cpptoml::table> config;
    };
}
#endif //GEBAAR_CONFIG_H

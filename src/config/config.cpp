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


#include <zconf.h>
#include "config.h"

bool gebaar::config::Config::find_config_file()
{
    auto true_path = std::filesystem::path(config_file_path);
    return std::filesystem::exists(true_path);
}

void gebaar::config::Config::load_config()
{
    if (find_home_folder()) {
        if (find_config_file()) {
            config = cpptoml::parse_file(std::filesystem::path(config_file_path));
            swipe_three_up_command = *config->get_qualified_as<std::string>("commands.swipe.three.up");
            swipe_three_down_command = *config->get_qualified_as<std::string>("commands.swipe.three.down");
            swipe_three_left_command = *config->get_qualified_as<std::string>("commands.swipe.three.left");
            swipe_three_right_command = *config->get_qualified_as<std::string>("commands.swipe.three.right");
            swipe_four_up_command = *config->get_qualified_as<std::string>("commands.swipe.four.up");
            swipe_four_down_command = *config->get_qualified_as<std::string>("commands.swipe.four.down");
            swipe_four_left_command = *config->get_qualified_as<std::string>("commands.swipe.four.left");
            swipe_four_right_command = *config->get_qualified_as<std::string>("commands.swipe.four.right");
            loaded = true;
        }
    }

}

bool gebaar::config::Config::find_home_folder()
{
    const char* temp_path;
    temp_path = getenv("XDG_CONFIG_HOME");
    if (temp_path==nullptr) {
        temp_path = getenv("HOME");
        if (temp_path==nullptr) {
            temp_path = getpwuid(getuid())->pw_dir;
        }
    }
    if (temp_path!=nullptr) {
        config_file_path = temp_path;
        config_file_path.append("/.config/gebaar/gebaard.toml");
        return true;
    }
    return false;
}

gebaar::config::Config::Config()
{
    if (!loaded) {
        load_config();
    }
}

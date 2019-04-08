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
#include "../util.h"

/**
 * Check if config file exists at current path
 */
bool gebaar::config::Config::config_file_exists()
{
    auto true_path = std::filesystem::path(config_file_path);
    return std::filesystem::exists(true_path);
}

/**
 * Load Configuration from TOML file
 */
void gebaar::config::Config::load_config()
{
    if (find_config_file()) {
        if (config_file_exists()) {
            try {
                config = cpptoml::parse_file(std::filesystem::path(config_file_path));
            } catch (const cpptoml::parse_exception& e) {
                std::cerr << e.what() << std::endl;
                exit(EXIT_FAILURE);
            }
            swipe_three_commands[1] = *config->get_qualified_as<std::string>("commands.swipe.three.left_up");
            swipe_three_commands[2] = *config->get_qualified_as<std::string>("commands.swipe.three.up");
            swipe_three_commands[3] = *config->get_qualified_as<std::string>("commands.swipe.three.right_up");
            swipe_three_commands[4] = *config->get_qualified_as<std::string>("commands.swipe.three.left");
            swipe_three_commands[6] = *config->get_qualified_as<std::string>("commands.swipe.three.right");
            swipe_three_commands[7] = *config->get_qualified_as<std::string>("commands.swipe.three.left_down");
            swipe_three_commands[8] = *config->get_qualified_as<std::string>("commands.swipe.three.down");
            swipe_three_commands[9] = *config->get_qualified_as<std::string>("commands.swipe.three.right_down");

            swipe_four_commands[1] = *config->get_qualified_as<std::string>("commands.swipe.four.left_up");
            swipe_four_commands[2] = *config->get_qualified_as<std::string>("commands.swipe.four.up");
            swipe_four_commands[3] = *config->get_qualified_as<std::string>("commands.swipe.four.right_up");
            swipe_four_commands[4] = *config->get_qualified_as<std::string>("commands.swipe.four.left");
            swipe_four_commands[6] = *config->get_qualified_as<std::string>("commands.swipe.four.right");
            swipe_four_commands[7] = *config->get_qualified_as<std::string>("commands.swipe.four.left_down");
            swipe_four_commands[8] = *config->get_qualified_as<std::string>("commands.swipe.four.down");
            swipe_four_commands[9] = *config->get_qualified_as<std::string>("commands.swipe.four.right_down");

            pinch_in_command = *config->get_qualified_as<std::string>("commands.pinch.in");
            pinch_out_command = *config->get_qualified_as<std::string>("commands.pinch.out");

            loaded = true;
        }
    }

}

/**
 * Find the configuration file according to XDG spec
 * @return bool
 */
bool gebaar::config::Config::find_config_file()
{
    std::string temp_path = gebaar::util::stringFromCharArray(getenv("XDG_CONFIG_HOME"));
    if (temp_path.empty()) {
        // first get the path to HOME
        temp_path = gebaar::util::stringFromCharArray(getenv("HOME"));
        if (temp_path.empty()) {
            temp_path = getpwuid(getuid())->pw_dir;
        }
        // then append .config
        if (!temp_path.empty()) {
            temp_path.append("/.config");
        }
    }
    if (!temp_path.empty()) {
        config_file_path = temp_path;
        config_file_path.append("/gebaar/gebaard.toml");
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

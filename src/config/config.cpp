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

#include "config.h"
#include "../util.h"
#include <zconf.h>

/**
 * Check if config file exists at current path
 */
bool gebaar::config::Config::config_file_exists() {
  auto true_path = std::filesystem::path(config_file_path);
  return std::filesystem::exists(true_path);
}

/**
 * Load Configuration from TOML file
 */
void gebaar::config::Config::load_config() {
  if (find_config_file()) {
    if (config_file_exists()) {
      try {
        config = cpptoml::parse_file(std::filesystem::path(config_file_path));
      } catch (const cpptoml::parse_exception &e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
      }

      /* Swipe Settings */
      swipe_three_commands[1] = *config->get_qualified_as<std::string>(
          "swipe.commands.three.left_up");
      swipe_three_commands[2] =
          *config->get_qualified_as<std::string>("swipe.commands.three.up");
      swipe_three_commands[3] = *config->get_qualified_as<std::string>(
          "swipe.commands.three.right_up");
      swipe_three_commands[4] =
          *config->get_qualified_as<std::string>("swipe.commands.three.left");
      swipe_three_commands[6] =
          *config->get_qualified_as<std::string>("swipe.commands.three.right");
      swipe_three_commands[7] = *config->get_qualified_as<std::string>(
          "swipe.commands.three.left_down");
      swipe_three_commands[8] =
          *config->get_qualified_as<std::string>("swipe.commands.three.down");
      swipe_three_commands[9] = *config->get_qualified_as<std::string>(
          "swipe.commands.three.right_down");

      swipe_four_commands[1] =
          *config->get_qualified_as<std::string>("swipe.commands.four.left_up");
      swipe_four_commands[2] =
          *config->get_qualified_as<std::string>("swipe.commands.four.up");
      swipe_four_commands[3] = *config->get_qualified_as<std::string>(
          "swipe.commands.four.right_up");
      swipe_four_commands[4] =
          *config->get_qualified_as<std::string>("swipe.commands.four.left");
      swipe_four_commands[6] =
          *config->get_qualified_as<std::string>("swipe.commands.four.right");
      swipe_four_commands[7] = *config->get_qualified_as<std::string>(
          "swipe.commands.four.left_down");
      swipe_four_commands[8] =
          *config->get_qualified_as<std::string>("swipe.commands.four.down");
      swipe_four_commands[9] = *config->get_qualified_as<std::string>(
          "swipe.commands.four.right_down");

      settings.swipe_threshold =
          config->get_qualified_as<double>("swipe.settings.threshold")
              .value_or(0.5);
      settings.swipe_one_shot =
          config->get_qualified_as<bool>("swipe.settings.one_shot")
              .value_or(true);
      settings.swipe_trigger_on_release =
          config->get_qualified_as<bool>("swipe.settings.trigger_on_release")
              .value_or(true);

      /* Pinch settings */
      pinch_commands[PINCH_IN] =
          *config->get_qualified_as<std::string>("pinch.commands.two.out");
      pinch_commands[PINCH_OUT] =
          *config->get_qualified_as<std::string>("pinch.commands.two.in");

      settings.pinch_threshold =
          config->get_qualified_as<double>("pinch.settings.threshold")
              .value_or(0.25);
      settings.pinch_one_shot =
          config->get_qualified_as<bool>("pinch.settings.one_shot")
              .value_or(false);

      loaded = true;
    }
  }
}

/**
 * Find the configuration file according to XDG spec
 * @return bool
 */
bool gebaar::config::Config::find_config_file() {
  std::string temp_path =
      gebaar::util::stringFromCharArray(getenv("XDG_CONFIG_HOME"));
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

gebaar::config::Config::Config() {
  if (!loaded) {
    load_config();
  }
}

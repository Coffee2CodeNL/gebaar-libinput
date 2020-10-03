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

#ifndef GEBAAR_INPUT_HPP
#define GEBAAR_INPUT_HPP

#include "../config/config.h"
#include <fcntl.h>
#include <libinput.h>
#include <zconf.h>

#define DEFAULT_SCALE 1.0
#define SWIPE_X_THRESHOLD 1000
#define SWIPE_Y_THRESHOLD 500

namespace gebaar::io {
struct gesture_swipe_event {
  int fingers;
  double x;
  double y;

  bool executed;
  int step;
};

struct gesture_pinch_event {
  int fingers;
  double scale;
  double angle;

  bool executed;
  int step;
};

class Input {
public:
  Input(std::shared_ptr<gebaar::config::Config> const &config_ptr);

  ~Input();

  bool initialize();

  void start_loop();

private:
  std::shared_ptr<gebaar::config::Config> config;

  struct libinput *libinput;
  struct libinput_event *libinput_event;
  struct udev *udev;

  struct gesture_swipe_event gesture_swipe_event;
  struct gesture_pinch_event gesture_pinch_event;

  bool initialize_context();

  bool gesture_device_exists();

  static int open_restricted(const char *path, int flags, void *user_data) {
    int fd = open(path, flags);
    return fd < 0 ? -errno : fd;
  }

  static void close_restricted(int fd, void *user_data) { close(fd); }

  constexpr static struct libinput_interface libinput_interface = {
      .open_restricted = open_restricted,
      .close_restricted = close_restricted,
  };

  /*
   * Decrements step of current trigger. Just to skip 0
   * @param cur current step
   */
  inline void dec_step(int &cur) { --cur == 0 ? --cur : cur; }

  /*
   * Increase step of current trigger. Just to pass -1
   * @param cur current step
   */
  inline void inc_step(int &cur) { ++cur == 0 ? ++cur : cur; }

  void handle_event();

  /* Swipe event */
  void reset_swipe_event();

  void handle_swipe_event_without_coords(libinput_event_gesture *gev,
                                         bool begin);

  void handle_swipe_event_with_coords(libinput_event_gesture *gev);

  void trigger_swipe_command();

  /* Pinch event */
  void reset_pinch_event();

  void handle_one_shot_pinch(double new_scale);

  void handle_continouos_pinch(double new_scale);

  void handle_pinch_event(libinput_event_gesture *gev, bool begin);
};
} // namespace gebaar::io

#endif // GEBAAR_INPUT_HPP

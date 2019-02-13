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

#include "input.hpp"

gebaar::io::Input::Input()
{
    gesture_swipe_event = {};
    logger = spdlog::get("logger");
}

void gebaar::io::Input::initialize_context()
{
    udev = udev_new();
    libinput = libinput_udev_create_context(&libinput_interface, nullptr, udev);
    return libinput_udev_assign_seat(libinput, "seat0");
}

void gebaar::io::Input::handle_swipe_event_without_coords(libinput_event_gesture* gev, bool begin)
{
    if (begin) {
        gesture_swipe_event.fingers = libinput_event_gesture_get_finger_count(gev);
    }
    else {
        if (abs(gesture_swipe_event.x)>abs(gesture_swipe_event.y)) {
            if (gesture_swipe_event.x<0) {
                logger->info("{} Finger Swipe Left", swipe_event.fingers);
            }
            else {
                logger->info("{} Finger Swipe Right", swipe_event.fingers);
            }
        }
        else {
            if (gesture_swipe_event.y<0) {
                logger->info("{} Finger Swipe Up", swipe_event.fingers);
            }
            else {
                logger->info("{} Finger Swipe Down", swipe_event.fingers);
            }
        }
        gesture_swipe_event = {};
    }
}

void gebaar::io::Input::handle_swipe_event_with_coords(libinput_event_gesture* gev)
{
    gesture_swipe_event.x += libinput_event_gesture_get_dx(gev);
    gesture_swipe_event.y += libinput_event_gesture_get_dy(gev);
}



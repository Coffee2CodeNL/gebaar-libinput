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

#include <poll.h>
#include "input.h"

/**
 * Input system constructor, we pass our Configuration object via a shared pointer
 *
 * @param config_ptr shared pointer to configuration object
 */
gebaar::io::Input::Input(std::shared_ptr<gebaar::config::Config> const& config_ptr)
{
    config = config_ptr;
    gesture_swipe_event = {};
}

/**
 * Initialize the libinput context
 *
 * @return bool
 */
bool gebaar::io::Input::initialize_context()
{
    udev = udev_new();
    libinput = libinput_udev_create_context(&libinput_interface, nullptr, udev);
    return libinput_udev_assign_seat(libinput, "seat0")==0;
}

int gebaar::io::Input::update_swipe_type_if_command_set(int swipe_type, int d, std::string commands[]) {
    if (commands[swipe_type] == "") {
        return swipe_type;
    } else {
        return swipe_type + d;
    }
}

std::string gebaar::io::Input::determine_command(double x, double y, std::string commands[]) {
    int swipe_type = 5; // middle = no swipe
                       // 1 = left_up, 2 = up, 3 = right_up...
                       // 1 2 3
                       // 4 5 6
                       // 7 8 9
    const double OBLIQUE_RATIO = 0.414; // =~ tan(22.5);

    if (abs(x) > abs(y)) {
        // left or right swipe
        swipe_type += x < 0 ? -1 : 1;

        // check for oblique swipe
        if (abs(y) / abs(x) > OBLIQUE_RATIO) {
            if (y < 0) {
                swipe_type = update_swipe_type_if_command_set(swipe_type, -3, commands);
            } else {
                swipe_type = update_swipe_type_if_command_set(swipe_type, 3, commands);
            }
        }
    } else {
        // up of down swipe
        swipe_type += y < 0 ? -3 : 3;

        // check for oblique swipe
        if (abs(x) / abs(y) > OBLIQUE_RATIO) {
            if (x < 0) {
                swipe_type = update_swipe_type_if_command_set(swipe_type, -1, commands);
            } else {
                swipe_type = update_swipe_type_if_command_set(swipe_type, 1, commands);
            }
        }
    }

    return commands[swipe_type];
}

/**
 * This event has no coordinates, so it's an event that gives us a begin or end signal.
 * If it begins, we get the amount of fingers used.
 * If it ends, we check what kind of gesture we received.
 *
 * @param gev Gesture Event
 * @param begin Boolean to denote begin or end of gesture
 */
void gebaar::io::Input::handle_swipe_event_without_coords(libinput_event_gesture* gev, bool begin)
{
    if (begin) {
        gesture_swipe_event.fingers = libinput_event_gesture_get_finger_count(gev);
    }
    else {
        double x = gesture_swipe_event.x;
        double y = gesture_swipe_event.y;

        if (gesture_swipe_event.fingers == 3) {
            std::system(determine_command(x, y, config->swipe_three_commands).c_str());
        } else if (gesture_swipe_event.fingers == 4) {
            std::system(determine_command(x, y, config->swipe_four_commands).c_str());
        }

        gesture_swipe_event = {};
    }
}

/**
 * Swipe events with coordinates, add it to the current tally
 * @param gev Gesture Event
 */
void gebaar::io::Input::handle_swipe_event_with_coords(libinput_event_gesture* gev)
{
    gesture_swipe_event.x += libinput_event_gesture_get_dx(gev);
    gesture_swipe_event.y += libinput_event_gesture_get_dy(gev);
}

/**
 * Initialize the input system
 * @return bool
 */
bool gebaar::io::Input::initialize()
{
    initialize_context();
    return gesture_device_exists();
}

/**
 * Run a poll loop on the file descriptor that libinput gives us
 */
void gebaar::io::Input::start_loop()
{
    struct pollfd fds{};
    fds.fd = libinput_get_fd(libinput);
    fds.events = POLLIN;
    fds.revents = 0;

    while (poll(&fds, 1, -1)>-1) {
        handle_event();
    }
}

gebaar::io::Input::~Input()
{
    libinput_unref(libinput);
}

/**
 * Check if there's a device that supports gestures on this system
 * @return
 */
bool gebaar::io::Input::gesture_device_exists()
{
    bool device_found = false;
    while ((libinput_event = libinput_get_event(libinput))!=nullptr) {
        auto device = libinput_event_get_device(libinput_event);
        if (libinput_device_has_capability(device, LIBINPUT_DEVICE_CAP_GESTURE)) {
            device_found = true;
        }

        libinput_event_destroy(libinput_event);
        libinput_dispatch(libinput);
    }
    return device_found;
}

/**
 * Handle an event from libinput and run the appropriate action per event type
 */
void gebaar::io::Input::handle_event()
{
    libinput_dispatch(libinput);
    while ((libinput_event = libinput_get_event(libinput))) {
        switch (libinput_event_get_type(libinput_event)) {
        case LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN:
            handle_swipe_event_without_coords(libinput_event_get_gesture_event(libinput_event), true);
            break;
        case LIBINPUT_EVENT_GESTURE_SWIPE_UPDATE:
            handle_swipe_event_with_coords(libinput_event_get_gesture_event(libinput_event));
            break;
        case LIBINPUT_EVENT_GESTURE_SWIPE_END:
            handle_swipe_event_without_coords(libinput_event_get_gesture_event(libinput_event), false);
            break;
        case LIBINPUT_EVENT_NONE:
            break;
        case LIBINPUT_EVENT_DEVICE_ADDED:
            break;
        case LIBINPUT_EVENT_DEVICE_REMOVED:
            break;
        case LIBINPUT_EVENT_KEYBOARD_KEY:
            break;
        case LIBINPUT_EVENT_POINTER_MOTION:
            break;
        case LIBINPUT_EVENT_POINTER_MOTION_ABSOLUTE:
            break;
        case LIBINPUT_EVENT_POINTER_BUTTON:
            break;
        case LIBINPUT_EVENT_POINTER_AXIS:
            break;
        case LIBINPUT_EVENT_TOUCH_DOWN:
            break;
        case LIBINPUT_EVENT_TOUCH_UP:
            break;
        case LIBINPUT_EVENT_TOUCH_MOTION:
            break;
        case LIBINPUT_EVENT_TOUCH_CANCEL:
            break;
        case LIBINPUT_EVENT_TOUCH_FRAME:
            break;
        case LIBINPUT_EVENT_TABLET_TOOL_AXIS:
            break;
        case LIBINPUT_EVENT_TABLET_TOOL_PROXIMITY:
            break;
        case LIBINPUT_EVENT_TABLET_TOOL_TIP:
            break;
        case LIBINPUT_EVENT_TABLET_TOOL_BUTTON:
            break;
        case LIBINPUT_EVENT_TABLET_PAD_BUTTON:
            break;
        case LIBINPUT_EVENT_TABLET_PAD_RING:
            break;
        case LIBINPUT_EVENT_TABLET_PAD_STRIP:
            break;
        case LIBINPUT_EVENT_GESTURE_PINCH_BEGIN:
            break;
        case LIBINPUT_EVENT_GESTURE_PINCH_UPDATE:
            break;
        case LIBINPUT_EVENT_GESTURE_PINCH_END:
            break;
        case LIBINPUT_EVENT_SWITCH_TOGGLE:
            break;
        }

        libinput_event_destroy(libinput_event);
        libinput_dispatch(libinput);
    }
}



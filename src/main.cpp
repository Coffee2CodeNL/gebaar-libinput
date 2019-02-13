/*
    gebaar
    Copyright (C) 2019  coffee2code

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


#include <iostream>
#include <libinput.h>
#include <libudev.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/syslog_sink.h>
#include <cxxopts.hpp>

using namespace std;

shared_ptr<spdlog::logger> logger;

static int open_restricted(const char* path, int flags, void* user_data)
{
    int fd = open(path, flags);
    return fd<0 ? -errno : fd;
}

static void close_restricted(int fd, void* user_data)
{
    close(fd);
}

const static struct libinput_interface interface = {
        .open_restricted = open_restricted,
        .close_restricted = close_restricted,
};
struct swipe_event {
    int fingers;
    double x;
    double y;
};
static swipe_event swipeEvent = {};

static void handleSwipeEventWithoutCoords(libinput_event_gesture* gev, bool begin)
{
    auto fingers = libinput_event_gesture_get_finger_count(gev);
    if (begin) {
        swipeEvent.fingers = fingers;
    }
    else {
        auto absX = abs(swipeEvent.x);
        auto absY = abs(swipeEvent.y);
        if (absX>absY) {
            if (swipeEvent.x<0) {
                logger->debug("{} Finger Swipe Left", swipeEvent.fingers);
            }
            else {
                logger->debug("{} Finger Swipe Right", swipeEvent.fingers);
            }
        }
        else {
            if (swipeEvent.y<0) {
                logger->debug("{} Finger Swipe Up", swipeEvent.fingers);
            }
            else {
                logger->debug("{} Finger Swipe Down", swipeEvent.fingers);
            }
        }
        swipeEvent = {};
    }
}

static void handleSwipeEventWithCoords(libinput_event_gesture* gev)
{
    swipeEvent.x += libinput_event_gesture_get_dx(gev);
    swipeEvent.y += libinput_event_gesture_get_dy(gev);
}

static int handleEvents(libinput* li)
{
    struct libinput_event* liEv;
    libinput_dispatch(li);
    while ((liEv = libinput_get_event(li))) {
        switch (libinput_event_get_type(liEv)) {
        case LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN:
            handleSwipeEventWithoutCoords(libinput_event_get_gesture_event(liEv), true);
            break;
        case LIBINPUT_EVENT_GESTURE_SWIPE_UPDATE:
            handleSwipeEventWithCoords(libinput_event_get_gesture_event(liEv));
            break;
        case LIBINPUT_EVENT_GESTURE_SWIPE_END:
            handleSwipeEventWithoutCoords(libinput_event_get_gesture_event(liEv), false);
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

        libinput_event_destroy(liEv);
        libinput_dispatch(li);
    }
}

static void mainLoop(libinput* li)
{
    struct pollfd fds{};
    fds.fd = libinput_get_fd(li);
    fds.events = POLLIN;
    fds.revents = 0;

    if (handleEvents(li)) {
        logger->info("Handled already existing events");
    }

    while (poll(&fds, 1, -1)>-1) {
        handleEvents(li);
    }
}

int main(int argc, char* argv[])
{
    /*uid_t userId = geteuid();
    if (userId != 0) {
        spdlog::info("You have to run this as root! Exiting now...");
        exit(EXIT_SUCCESS);
    }*/

    cxxopts::Options options("gebaard", "Gebaard Gestures Daemon");
    struct libinput* li;
    struct libinput_event* liEv;
    struct udev* ud;
    bool debug_mode = false;
    bool daemonize = false;

    options.add_options()
            ("d,debug", "Debug Mode - Output to console", cxxopts::value(debug_mode))
            ("b,background", "Daemonize", cxxopts::value(daemonize))
            ("h,help", "Prints this help text");

    auto result = options.parse(argc, argv);
    spdlog::set_level(spdlog::level::info);
    if (debug_mode) {
        spdlog::set_level(spdlog::level::debug);
        logger = spdlog::stdout_color_mt("logger");
    }
    else {
        logger = spdlog::syslog_logger_mt("logger", "gebaard");
    }

    logger->debug("Debug mode: {}", debug_mode);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(EXIT_SUCCESS);
    }

    ud = udev_new();
    li = libinput_udev_create_context(&interface, nullptr, ud);
    libinput_udev_assign_seat(li, "seat0");
    libinput_dispatch(li);
    bool deviceWithGestureSupportExists = false;
    while ((liEv = libinput_get_event(li))!=nullptr) {
        auto device = libinput_event_get_device(liEv);
        auto devName = libinput_device_get_name(device);

        if (libinput_device_has_capability(device, LIBINPUT_DEVICE_CAP_GESTURE)) {
            logger->info("Found a device with gesture support: {}", devName);
            deviceWithGestureSupportExists = true;
        }

        libinput_event_destroy(liEv);
        libinput_dispatch(li);
    }

    if (deviceWithGestureSupportExists) {
        mainLoop(li);
    }
    else {
        logger->warn("No supported devices found, this won't work");
    }
    libinput_unref(li);

    return 0;
}

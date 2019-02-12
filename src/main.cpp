//
//      Gebaar
//      Copyright (C) 2019  Coffee2Code
//
//      This program is free software: you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation, either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include <iostream>
#include <libinput.h>
#include <libudev.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <spdlog/spdlog.h>

using namespace std;

static int open_restricted(const char *path, int flags, void *user_data) {
    int fd = open(path, flags);
    return fd < 0 ? -errno : fd;
}

static void close_restricted(int fd, void *user_data) {
    close(fd);
}

const static struct libinput_interface interface = {
        .open_restricted = open_restricted,
        .close_restricted = close_restricted,
};

static int handleEvents(libinput *li) {
    struct libinput_event *liEv;
    libinput_dispatch(li);
    while ((liEv = libinput_get_event(li))) {
        string evType;
        bool workableEvent = false;
        switch (libinput_event_get_type(liEv)) {
            case LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN:
                evType = "SWIPE_STA";
                workableEvent = true;
                break;
            case LIBINPUT_EVENT_GESTURE_SWIPE_UPDATE:
                evType = "SWIPE_UPD";
                workableEvent = true;
                break;
            case LIBINPUT_EVENT_GESTURE_SWIPE_END:
                evType = "SWIPE_END";
                workableEvent = true;
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
        if (workableEvent) {
            struct libinput_event_gesture *gev = libinput_event_get_gesture_event(liEv);
            auto fingers = libinput_event_gesture_get_finger_count(gev);
            auto deltaX = libinput_event_gesture_get_dx(gev);
            auto deltaY = libinput_event_gesture_get_dy(gev);
            spdlog::info("Got event: {}, {} fingers, {},{}", evType, fingers, deltaX, deltaY);
        }
        libinput_event_destroy(liEv);
        libinput_dispatch(li);
    }
}

static void mainLoop(libinput *li) {
    struct pollfd fds{};
    fds.fd = libinput_get_fd(li);
    fds.events = POLLIN;
    fds.revents = 0;

    if (handleEvents(li)) {
        spdlog::info("Handled already existing events");
    }

    while (poll(&fds, 1, -1) > -1) {
        handleEvents(li);
    }
}

int main() {
    uid_t userId = geteuid();
    if (userId != 0) {
        spdlog::info("You have to run this as root! Exiting now...");
        exit(EXIT_SUCCESS);
    }
    struct libinput *li;
    struct libinput_event *liEv;
    struct udev *ud;
    ud = udev_new();
    li = libinput_udev_create_context(&interface, nullptr, ud);
    libinput_udev_assign_seat(li, "seat0");
    libinput_dispatch(li);
    bool deviceWithGestureSupportExists = false;
    while ((liEv = libinput_get_event(li)) != nullptr) {
        auto eventType = libinput_event_get_type(liEv);
        auto device = libinput_event_get_device(liEv);
        auto devName = libinput_device_get_name(device);

        if (libinput_device_has_capability(device, LIBINPUT_DEVICE_CAP_GESTURE)) {
            spdlog::info("Found a device with gesture support: {}", devName);
            deviceWithGestureSupportExists = true;
        }

        libinput_event_destroy(liEv);
        libinput_dispatch(li);
    }

    if (deviceWithGestureSupportExists) {
        mainLoop(li);
    }
    libinput_unref(li);

    return 0;
}
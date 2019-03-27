Gebaar
=========

WM Independent Touchpad Gesture Daemon for libinput

_Gebaar means Gesture in Dutch_

Run any command by simply gesturing on your touchpad!

### What makes this different over the other implementations?

[libinput-gestures](https://github.com/bulletmark/libinput-gestures) and [fusuma](https://github.com/iberianpig/fusuma) both parse the output of the shell command `libinput debug-events` which is an unstable API and the output just keeps coming, so it'll eat (some) RAM.

Gebaar directly interfaces with libinput to receive and react to the events.   
This is more stable, faster, and more efficient as it **does not parse the output of a program** like the aforementioned projects do.

### Getting support (or talking about the project's future)

Click to join: [![Discord](https://img.shields.io/discord/548978799136473106.svg?label=Discord)](https://discord.gg/9mbKhFR)

### How to build and install

1. Clone the repository via `git clone https://github.com/Coffee2CodeNL/gebaar-libinput`
2. Check out the latest version (`git checkout v0.0.5`)
3. Run `git submodule update --init` in the root folder
4. Run `mkdir build && cd build`
5. Run `cmake ..`
6. Run `make -j$(nproc)`
7. Run `sudo make install` to install
8. Run `mkdir -p ~/.config/gebaar`
9. Run `nano ~/.config/gebaar/gebaard.toml` (or vim, if you like it better)
10. Add the snippet below to `gebaard.toml`
11. Configure commands to run per direction
12. Add yourself to the `input` group with `usermod -a -G input $USER`
13. Run Gebaar via some startup file by adding `gebaard -b` to it
14. Reboot and see the magic

```toml
[commands.swipe.three]
left_up = ""
right_up = ""
up = ""
left_down = ""
right_down = ""
down = ""
left = ""
right = ""

[commands.swipe.four]
left_up = ""
right_up = ""
up = ""
left_down = ""
right_down = ""
down = ""
left = ""
right = ""
```

### Repository versions

![](https://img.shields.io/aur/version/gebaar.svg?style=flat)  

### Examples

**bspwm**

_~/.config/gebaar/gebaard.toml_
```toml
[commands.swipe.three]
left_up = ""
right_up = ""
up = "bspc node -f north"
left_down = ""
right_down = ""
down = "bspc node -f south"
left = "bspc node -f west"
right = "bspc node -f east"

[commands.swipe.four]
left_up = ""
right_up = ""
up = "rofi -show combi"
left_down = ""
right_down = ""
down = ""
left = "bspc desktop -f prev"
right = "bspc desktop -f next"
```

Add `gebaard -b` to `~/.config/bspwm/bspwmrc`

### State of the project

- [x] Receiving swipe events from libinput
- [ ] Receiving pinch/zoom events from libinput
- [ ] Receiving rotation events from libinput
- [x] Converting libinput events to motions
- [x] Running commands based on motions
- [x] Refactor code to be up to Release standards, instead of testing-hell


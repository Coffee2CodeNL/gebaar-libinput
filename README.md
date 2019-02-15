Gebaar
=========
[![Gitter chat](https://badges.gitter.im/gebaar-libinput/community.png)](https://gitter.im/gebaar-libinput/community)

WM Independent Touchpad Gesture Daemon for libinput

_Gebaar means Gesture in Dutch_

Run any command by simply gesturing on your touchpad!

### What makes this different over the other implementations?

[libinput-gestures](https://github.com/bulletmark/libinput-gestures) and [fusuma](https://github.com/iberianpig/fusuma) both parse the output of the shell command `libinput debug-events` which is an unstable API and the output just keeps coming, so it'll eat (some) RAM.

Gebaar directly interfaces with libinput to receive and react to the events.   
This is more stable, faster, and more efficient as it **does not parse the output of a program** like the aforementioned projects do.


### How to build and install

1. Clone the repository
2. Check out the latest version (`git checkout v0.0.1`)
3. Run `git submodule update --init` in the root folder
4. Run `mkdir build && cd build`
5. Run `cmake ..`
6. Run `make -j$(nproc)`
7. Run `sudo make install` to install
8. Run `mkdir -p ~/.config/gebaar`
9. Run `nano ~/.config/gebaar/gebaard.toml` (or vim, if you like it better)
10. Add the snippet below to `gebaard.toml`
11. Configure commands to run per direction
12. Run Gebaar in some startup file via `gebaard -b`

```toml
[commands.swipe.three]
up = ""
down = ""
left = ""
right = ""

[commands.swipe.four]
up = ""
down = ""
left = ""
right = ""
```



### Examples

**bspwm**

_~/.config/gebaar/gebaard.toml_
```toml
[commands.swipe.three]
up = "bspc node -f north"
down = "bspc node -f south"
left = "bspc node -f west"
right = "bspc node -f east"

[commands.swipe.four]
up = "rofi -show combi"
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


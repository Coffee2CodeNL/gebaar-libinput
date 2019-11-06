FROM debian:stretch

RUN set -ex; \
    echo 'deb http://http.us.debian.org/debian/ testing non-free contrib main' > /etc/apt/sources.list.d/debian-testing.list; \
    apt-get update -q; \
    apt-get install -y git gcc-8 curl make cmake build-essential libinput-dev zlib1g-dev libinput-tools libsystemd-dev


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

#ifndef GEBAAR_DAEMONIZER_H
#define GEBAAR_DAEMONIZER_H

#include <csignal>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>

namespace gebaar::daemonizer {
class Daemonizer {
public:
  Daemonizer();
  ~Daemonizer();
  bool daemonize();
};
} // namespace gebaar::daemonizer

#endif // GEBAAR_DAEMONIZER_H

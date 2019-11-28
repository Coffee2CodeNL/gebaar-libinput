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

#include "daemonizer.h"

/**
 * Forking logic for classic style daemon functionality
 *
 * @return bool that denotes fork success
 */
bool gebaar::daemonizer::Daemonizer::daemonize() {
  pid_t pid = 0;
  pid = fork();
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  if (setsid() < 0) {
    // Boo.
  }

  signal(SIGCHLD, SIG_IGN);
  signal(SIGTRAP, SIG_IGN);

  pid = fork();
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }
  umask(0);
  if ((chdir("/")) < 0) {
    return false;
  }

  close(STDOUT_FILENO);
  close(STDIN_FILENO);
  close(STDERR_FILENO);

  if (getpid() != getsid(getpid())) {
    //
  }
  return true;
}

gebaar::daemonizer::Daemonizer::Daemonizer() = default;

gebaar::daemonizer::Daemonizer::~Daemonizer() = default;

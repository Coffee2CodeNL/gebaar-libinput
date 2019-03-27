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

#include "util.h"

/**
 * @brief Safely converts a char array to a std::string
 * @param charArr The char array to convert
 * @return charArr or an empty string, if charArr is a nullptr
 */
std::string gebaar::util::stringFromCharArray(char* charArr)
{
    return charArr == nullptr ? "" : charArr;
}

/*
Copyright (C) 2012 Sebastian Herbord. All rights reserved.

This file is part of Mod Organizer.

Mod Organizer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Mod Organizer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Mod Organizer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PRIVILEGES_H
#define PRIVILEGES_H

#include <filesystem>
#include <format>
#include <iostream>
#include <string>

template <class... Args>
void error(std::format_string<Args... > format, Args&& ...args) {
  std::cerr << std::format(format, std::forward<Args>(args)...) << std::endl;
}

template <class... Args>
void debug(std::format_string<Args... > format, Args&& ...args) {
  std::cout << std::format(format, std::forward<Args>(args)...) << std::endl;
}

bool SetOwner(std::filesystem::path filename, std::string newOwner);

#endif // PRIVILEGES_H

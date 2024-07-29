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

#include "privileges.h"

#include <cstring>
#include <filesystem>
#include <utime.h>

// 1.1.2000
static constexpr time_t targetTime = 946'684'800;

using namespace std;

static bool createMODirectory(const std::filesystem::path& directory)
{
  if (!exists(directory)) {
    error_code ec;
    if (!create_directory(directory, ec)) {
      error(R"(Failed to create "{}", {})", directory.native(), ec.message());
      return false;
    }
  }

  if (!is_directory(directory)) {
    error(R"("{}" is not a directory)", directory.native());
    return false;
  }
  return true;
}

static bool init(const std::filesystem::path& mopath)
{
  if (!createMODirectory(mopath / "profiles") ||
    !createMODirectory(mopath / "mods") ||
    !createMODirectory(mopath / "downloads")) {
    return false;
  }
  return true;
}

// sets actime/modtime to 1.1.2000
static bool backdateBSAs(const std::filesystem::path& dataPath)
{
  for (auto& entry : std::filesystem::directory_iterator{ dataPath }) {
    const auto& path = entry.path();
    if (path.extension() != ".bsa") {
      continue;
    }

    const utimbuf times{targetTime, targetTime};
    int result = utime(path.string().c_str(), &times);
    if(result != 0) {
      int e = errno;
      error("Failed to change date for {}: ", strerror(e));
      return false;
    }

  }
  return true;
}


int main(int argc, char** argv)
{
  if (argc < 2) {
    error("Invalid number of parameters");
    return -1;
  }

  debug("action: {}", argv[1]);

  if (strcmp(argv[1], "init") == 0) {
    if (argc < 4) {
      error("Invalid number of parameters");
      return -1;
    }
    debug("init: {} - {}", argv[2], argv[3]);
    // set up mod organizer directory
    if (!init(argv[2])) {
      return -2;
    }
  }
  else if (strcmp(argv[1], "backdateBSA") == 0) {
    debug("backdate bsas in {}", argv[2]);
    if (!backdateBSAs(argv[2])) {
      return -2;
    }
  }
  else if (strcmp(argv[1], "adminLaunch") == 0) {
    error("adminLaunch is disabled on linux");
    return -3;
  }
  else {
    return -1;
  }

  return 0;
}

#include "privileges.h"

#include <cstring>
#include <filesystem>
#include <string>
#include <utime.h>

// 1.1.2000
static inline constexpr time_t targetTime = 946'684'800;

using namespace std;

static bool createMODirectory(const std::filesystem::path& directory)
{
  if (!exists(directory)) {
    error_code ec;
    if (!create_directory(directory, ec)) {
      error(R"(Failed to create "{}", {})", directory.string(), ec.message());
      return false;
    }
  }

  if (!is_directory(directory)) {
    error(R"("{}" is not a directory)", directory.string());
    return false;
  }
  return true;
}

static bool init(const std::filesystem::path& mopath)
{
  if (!createMODirectory(mopath / "profiles") || !createMODirectory(mopath / "mods") ||
      !createMODirectory(mopath / "downloads")) {
    return false;
  }
  return true;
}

// sets actime/modtime to 1.1.2000
static bool backdateBSAs(const std::filesystem::path& dataPath)
{
  for (auto& entry : std::filesystem::directory_iterator{dataPath}) {
    const auto& path = entry.path();
    if (path.extension() != ".bsa") {
      continue;
    }

    const utimbuf times{targetTime, targetTime};
    int result = utime(path.string().c_str(), &times);
    if (result != 0) {
      int e = errno;
      error("Failed to change date for {}: {}", path.string(), strerror(e));
      return false;
    }
  }
  return true;
}

static bool adminLaunch(pid_t processID, const std::filesystem::path& executable,
                        const std::filesystem::path& workingDir)
{
  std::cerr << __FUNCTION__ << ": STUB\n";
  return false;
}

int mainDelegate(int argc, char** argv)
{
  if (argc < 2) {
    error("Invalid number of parameters");
    return -1;
  }

  debug("action: {}", argv[1]);

  if (strcmp(argv[1], "init") == 0) {
    if (argc < 3) {
      error("Invalid number of parameters");
      return -1;
    }
    debug("init: {}", argv[2]);
    // set up mod organizer directory
    if (!init(argv[2])) {
      return -2;
    }
  } else if (strcmp(argv[1], "backdateBSA") == 0) {
    debug("backdate bsas in {}", argv[2]);
    if (!backdateBSAs(argv[2])) {
      return -2;
    }
  } else if (strcmp(argv[1], "adminLaunch") == 0) {
    if (argc < 5) {
      error("Invalid number of parameters");
      return -3;
    }
    debug("adminLaunch {} {} {}", argv[2], argv[3], argv[4]);
    if (!adminLaunch(std::stoi(argv[2]), argv[3], argv[4])) {
      return -3;
    }
  } else {
    return -1;
  }

  return 0;
}

int main(int argc, char** argv)
{
  int res = mainDelegate(argc, argv);
  if (res != 0) {
    error("{}", res);
    getchar();
  }

  return res;
}

#include "privileges.h"

#include <filesystem>
#include <string>

bool SetOwner(const std::filesystem::path& filename, const std::string& newOwner)
{
  std::cerr << __FUNCTION__ << ": STUB\n";
  return true;
}

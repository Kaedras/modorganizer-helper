#include "privileges.h"

#include <filesystem>

// this could be implemented using polkit
bool SetOwner(std::filesystem::path filename, std::string newOwner)
{
  (void)filename;
  (void)newOwner;

  return false;
}

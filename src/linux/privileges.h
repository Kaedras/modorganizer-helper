#ifndef PRIVILEGES_H
#define PRIVILEGES_H

#include <filesystem>
#include <format>
#include <iostream>
#include <string>

template <class... Args>
void error(std::format_string<Args...> format, Args&&... args)
{
  std::cerr << std::format(format, std::forward<Args>(args)...) << std::endl;
}

template <class... Args>
void debug(std::format_string<Args...> format, Args&&... args)
{
  std::cout << std::format(format, std::forward<Args>(args)...) << std::endl;
}

bool SetOwner(const std::filesystem::path& filename, const std::string& newOwner);

#endif  // PRIVILEGES_H

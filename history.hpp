#pragma once

#include <string>
#include <vector>

std::vector<std::string> load_history(const std::string& path);
void append_history(const std::string& path, const std::string& command);
void clear_history(const std::string& path, std::vector<std::string>& history);

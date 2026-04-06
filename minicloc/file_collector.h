#pragma once

#include <string>
#include <vector>

void collectFiles(const std::string& path, const std::vector<std::string>& filter_exts, std::vector<std::string>& out_files);

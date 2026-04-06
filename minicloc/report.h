#pragma once

#include <map>
#include <vector>

#include "stats.h"

void printReport(const std::map<std::string, LangStats>& lang_stats, const std::vector<FileStats>& all_files, bool show_files);

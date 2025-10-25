#pragma once

#include <cstdint>
#include <vector>
#include "file_structs.hpp"

struct event_stat {
  std::vector<char> buffer;
  std::vector<hb_arena_instrumentation_event_type_T> type;
  std::vector<int> index;
  std::vector<size_t> offset;
  std::vector<int> capacity;
  std::vector<int> page_count;
  std::vector<int> page_occupation;
  std::vector<int> free_space;
};

void read_file(const char* file_name, std::vector<char> &buffer);
void get_stats(std::vector<char> &raw_buffer, event_stat &stats);

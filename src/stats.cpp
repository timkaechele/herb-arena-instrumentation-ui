#include "stats.hpp"
#include "file_structs.hpp"
#include <cstddef>
#include <cstdio>
#include <SDL3/SDL.h>

void read_file(const char* file_name, std::vector<char> &buffer) {
  SDL_Log("%s", file_name);
 if (FILE *fp = fopen(file_name, "rb"))
  {
    char buf[1024];
    while (size_t len = fread(buf, 1, sizeof(buf), fp))
      buffer.insert(buffer.end(), buf, buf + len);
    fclose(fp);
  }
}

void get_stats(std::vector<char> &raw_buffer, event_stat &stats) {
  size_t *event_count = (size_t*)&raw_buffer[0];
  SDL_Log("%ld", *event_count);

  stats.buffer = raw_buffer;
  size_t offset = sizeof(size_t);
  hb_arena_instrumentation_event_T *event;
  for (int i = 0; i < *event_count; ++i)
  {
    event = (hb_arena_instrumentation_event_T *)&raw_buffer[offset];
    stats.offset.push_back(offset);

    switch (event->type) {
    case HB_ARENA_INSTRUMENTATION_EVENT_INIT: {
      stats.index.push_back(stats.index.size());
      stats.type.push_back(hb_arena_instrumentation_event_type_T::HB_ARENA_INSTRUMENTATION_EVENT_INIT);
      stats.capacity.push_back(event->init_data.default_size);
      stats.page_count.push_back(1);
      stats.page_occupation.push_back(0);
      stats.free_space.push_back(event->init_data.default_size);

      offset += sizeof(hb_arena_instrumentation_event_T);
    }
    break;
    case HB_ARENA_INSTRUMENTATION_EVENT_ALLOC: {
      size_t capacity = 0;
      size_t filled = 0;
      for (int i = 0; i < event->alloc_data.page_count; ++i)
      {
        capacity += event->alloc_data.page_states[i].capacity;
        filled += event->alloc_data.page_states[i].position;
      }

      stats.index.push_back(stats.index.size());
      stats.type.push_back(hb_arena_instrumentation_event_type_T::HB_ARENA_INSTRUMENTATION_EVENT_ALLOC);
      stats.capacity.push_back(capacity);
      stats.page_count.push_back(event->alloc_data.page_count);
      stats.page_occupation.push_back(filled);
      stats.free_space.push_back(capacity - filled);

      offset += sizeof(hb_arena_instrumentation_event_T) + sizeof(hb_arena_instrumentation_page_state_T) * event->alloc_data.page_count;
    }
    break;
    case HB_ARENA_INSTRUMENTATION_EVENT_RESET: {
      size_t capacity = 0;
      size_t filled = 0;
      for (int i = 0; i < event->reset_data.page_count; ++i)
      {
        capacity += event->reset_data.page_states[i].capacity;
        filled += event->reset_data.page_states[i].position;
      }

      stats.index.push_back(stats.index.size());
      stats.type.push_back(hb_arena_instrumentation_event_type_T::HB_ARENA_INSTRUMENTATION_EVENT_RESET);
      stats.capacity.push_back(capacity);
      stats.page_count.push_back(event->reset_data.page_count);
      stats.page_occupation.push_back(filled);
      stats.free_space.push_back(capacity - filled);

      offset += sizeof(hb_arena_instrumentation_event_T) + sizeof(hb_arena_instrumentation_page_state_T) * event->reset_data.page_count;
    }
    break;
    }
  }
}

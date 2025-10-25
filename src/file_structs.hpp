#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

typedef enum {
  HB_ARENA_INSTRUMENTATION_EVENT_INIT,
  HB_ARENA_INSTRUMENTATION_EVENT_ALLOC,
  HB_ARENA_INSTRUMENTATION_EVENT_RESET,
} hb_arena_instrumentation_event_type_T;

typedef struct HB_ARENA_INSTRUMENTATION_PAGE_STATE_STRUCT {
  uintptr_t address;
  size_t capacity;
  size_t position;
} hb_arena_instrumentation_page_state_T;

typedef struct HB_ARENA_INSTRUMENTATION_EVENT_STRUCT {
  hb_arena_instrumentation_event_type_T type;
  size_t time;
  union {
    struct {
      size_t default_size;
    } init_data;
    struct {
      size_t allocation_size;
      size_t page_count;
      hb_arena_instrumentation_page_state_T page_states[];
    } alloc_data;
    struct {
      size_t reset_position;
      size_t page_count;
      hb_arena_instrumentation_page_state_T page_states[];
    } reset_data;
  };
} hb_arena_instrumentation_event_T;
#ifdef __cplusplus
}
#endif

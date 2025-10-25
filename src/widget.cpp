#include "widget.hpp"

#include "file_structs.hpp"
#include "imgui.h"
#include "implot.h"
#include "stats.hpp"
#include <cstdio>
#include <vector>

static int selected_row = -1;

void render_window(event_stat &stats) {
  static size_t selected_item = -1;
  if (ImGui::Begin("Stats")) {
    if(ImPlot::BeginPlot("Allocation Stats")) {

      ImPlot::PlotLine("Capacity", &stats.index[0], &stats.capacity[0], stats.index.size());
      ImPlot::PlotLine("Filled", &stats.index[0], &stats.page_occupation[0], stats.index.size());
      ImPlot::PlotLine("Free", &stats.index[0], &stats.free_space[0], stats.index.size());

      ImPlot::EndPlot();
    }
    if(ImPlot::BeginPlot("Page Count")) {

      ImPlot::PlotLine("Page Count", &stats.index[0], &stats.page_count[0], stats.index.size());

      ImPlot::EndPlot();
    }
  }
  ImGui::End();

  if (ImGui::Begin("Events")) {
    if (ImGui::BeginTable("table2", 5,
        ImGuiTableFlags_BordersV |
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("Event");
        ImGui::TableSetupColumn("Capacity");
        ImGui::TableSetupColumn("Filled");
        ImGui::TableSetupColumn("Page Count");

        ImGui::TableSetupColumn("Free Space");
        ImGui::TableHeadersRow();

        ImGuiListClipper clipper = ImGuiListClipper();
        clipper.Begin(stats.index.size());
        while(clipper.Step()) {
          for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
          {
              ImGui::TableNextRow();

              // Column 1
              ImGui::TableNextColumn();
              if (stats.type[row] == HB_ARENA_INSTRUMENTATION_EVENT_ALLOC) {
                ImGui::Text("ALLOC");
              }
              if (stats.type[row] == HB_ARENA_INSTRUMENTATION_EVENT_RESET) {
                ImGui::Text("RESET");
              }
              if (stats.type[row] == HB_ARENA_INSTRUMENTATION_EVENT_INIT) {
                ImGui::Text("INIT");
              }
              if (ImGui::IsItemHovered()) {
                selected_item = row;
              }

              ImGui::TableNextColumn();
              ImGui::Text("%d", stats.capacity[row]);
              if (ImGui::IsItemHovered()) {
                selected_item = row;
              }

              ImGui::TableNextColumn();
              ImGui::Text("%d", stats.page_occupation[row]);
              if (ImGui::IsItemHovered()) {
                selected_item = row;
              }

              ImGui::TableNextColumn();
              ImGui::Text("%d", stats.page_count[row]);
              if (ImGui::IsItemHovered()) {
                selected_item = row;
              }

              ImGui::TableNextColumn();
              ImGui::Text("%d", stats.free_space[row]);
              if (ImGui::IsItemHovered()) {
                selected_item = row;
              }
          }
        }

        clipper.End();
        ImGui::EndTable();
    }
  }
  ImGui::End();

  if (ImGui::Begin("Page State")) {
    if (selected_item != -1) {
      ImGui::Text("Event %ld", selected_item);
      char buffer[128];
      hb_arena_instrumentation_event_T *event = (hb_arena_instrumentation_event_T *)&stats.buffer[stats.offset[selected_item]];
      if (event->type == HB_ARENA_INSTRUMENTATION_EVENT_ALLOC) {
        for (int i = 0; i < event->alloc_data.page_count; ++i)
        {
          hb_arena_instrumentation_page_state_T *state = &event->alloc_data.page_states[i];
          ImGui::Text("Page %d  0x%08lx", i, state->address);
          snprintf(buffer, 128, "%ld/%ld", state->position, state->capacity);
          ImGui::ProgressBar(state->position / (float)state->capacity, ImVec2(-FLT_MIN, 0), buffer);
        }
      }
    }
  }
  ImGui::End();

}


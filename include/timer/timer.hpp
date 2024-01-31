#pragma once

#include <lvgl.h>
#include <cronos.hpp>

namespace glue {

class timer {
public:
  timer() = default;
  static void init() { init([](void) { return msecu32().count(); }); }
  static void init(lv_tick_get_cb_t get) { lv_tick_set_cb(get); }
};

} // namespace glue

#pragma once

#include <drivers/display/st7789/lv_st7789.h>

#include "display/display.hpp"
#include "bus/bus.hpp"

namespace glue {

namespace native {

template <class Bus, size_t Height, size_t Width,
  size_t BufHeight = Height / 10, bool DoubleBuf = true>
class display: public glue::display<Bus, Height, Width, BufHeight, DoubleBuf> {
private:
  using base_display_t =
    glue::display<Bus, Height, Width, BufHeight, DoubleBuf>;

  static void send_cmd(lv_display_t *disp, const uint8_t *cmd, size_t size,
    const uint8_t *param, size_t param_size) {
  }

  static void send_color(lv_display_t *disp, const uint8_t *cmd, size_t size,
    uint8_t *param, size_t param_size) {
    //Bus::tx();
  }

public:
  display() = delete;

  virtual constexpr bool is_native() const override { return true; }

  static void init(
    lv_display_render_mode_t mode = LV_DISPLAY_RENDER_MODE_PARTIAL,
    lv_lcd_flag_t flags = LV_LCD_FLAG_NONE) {
    base_display_t::init(lv_st7789_create, send_cmd, send_color, flags, mode);
    Bus::instance().init();
  }
};

} // namespace native

} // namespace glue

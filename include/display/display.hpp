#pragma once

#include <functional>

#include <lvgl.h>
#include <drivers/display/lcd/lv_lcd_generic_mipi.h>

namespace glue {

// Displays with native lvgl drivers share a common creation function signature
// that requires two callbacks:
//   - lv_lcd_send_cmd_cb_t   : transmit a command (polling)
//   - lv_lcd_send_color_cb_t : transmit raw pixel data (polling or DMA)
//
// The user can provide their own implementations, or but the simplest approach
// is to wrap a working display driver.
//
// Native drivers already provide their own flush implementation.
using lv_display_create_cb_t = std::function<
  lv_display_t *(uint32_t, uint32_t, lv_lcd_flag_t,
    lv_lcd_send_cmd_cb_t, lv_lcd_send_color_cb_t)>;

template
  < class Bus, size_t Height, size_t Width,
    size_t BufHeight, bool DoubleBuf >
class display {
protected:
  static constexpr size_t      _size = BufHeight * Width * LV_COLOR_DEPTH / 8;
  static inline lv_display_t  *_disp;
  static inline lv_color_t     _fb[DoubleBuf ? 2 : 1][_size];

  // Common initialization for all display types. This init must be called at a
  // specific moment (after the display is created and callbacks installed), so
  // it has private visibility to prevent misuse.
  static void init(lv_display_render_mode_t mode) {
    lv_display_set_buffers(
      _disp, _fb[0], DoubleBuf ? _fb[1] : nullptr, _size, mode);
  }

public:
  display() = delete;

  virtual constexpr bool is_native() const { return false; }

  // Generic displays without native lvgl driver require a flush implementation.
  static void init(lv_display_flush_cb_t flush, lv_display_render_mode_t mode) {
    _disp = lv_display_create(Width, Height);
    lv_display_set_flush_cb(_disp, flush);
    init(mode);
  }

  // Native lvgl drivers provide their own flush implementation, but require
  // separate implementations for command and color transmission.
  static void init(lv_display_create_cb_t create,
    lv_lcd_send_cmd_cb_t send_cmd, lv_lcd_send_color_cb_t send_color,
    lv_lcd_flag_t flags, lv_display_render_mode_t mode) {
    _disp = create(Width, Height, flags, send_cmd, send_color);
    init(mode);
  }
};

} // namespace glue

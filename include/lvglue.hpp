#pragma once

#include "timer/timer.hpp"
#if (LV_USE_ILI9341)
  #include "display/ili9341/ili9341.hpp"
#elif (LV_USE_ST7735)
  #include "display/st7735/st7735.hpp"
#elif (LV_USE_ST7789)
  #include "display/st7789/st7789.hpp"
#elif (LV_USE_ST7796)
  #include "display/st7796/st7796.hpp"
#elif (LV_USE_GENERIC_MIPI)
  #include "display/display.hpp"
#else
  #error "No display driver selected!"
#endif
#include "input/input.hpp"

#include "bus/spi/samd51/samd51.hpp"

namespace glue {

using init_cb_t = std::function<void()>;

template <typename Timer, typename Display, typename Input>
class adapter {
public:
  using timer_t = Timer;
  using display_t = Display;
  using input_t = Input;

  adapter() = delete;

  static void init(
    init_cb_t init_timer, init_cb_t init_display, init_cb_t init_input) {
    init_timer();
    lv_init();
    init_display();
    init_input();
  }
};

} // namespace glue

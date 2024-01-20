#include <cronos.hpp>
#include <lvgl.h>

namespace glue {

static void log(const char *message) {
#if (LV_USE_LOG)
  printf("%s\n", message);
#endif
}

template <size_t Height, Width, typename Log = decltype(log)> class core {
private:
  static lv_disp_t          *_disp;
  static lv_disp_drv_t       _ddrv;
  static lv_indev_t         *_inpt;
  static lv_indev_drv_t      _idrv;
  static lv_disp_draw_buf_t  _draw;
  static lv_color_t          _fb[(Height * Width) / 10];

  static void init_runtime(Log log) {
    lv_tick_set_cb([](void) -> void { return msecu32().count(); });
    lv_log_register_print_cb(log);
  }

  static void init_display() {
    lv_disp_draw_buf_init(&_draw, _fb, nullptr, (Height * Width) / 10);
    lv_disp_drv_init(&_ddrv);
    _ddrv.hor_res = Width;
    _ddrv.ver_res = Height;
    _ddrv.draw_buf = &_draw;
    _ddrv.flush_cb = [](lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p) {}
    // Callback<void(lv_disp_drv_t *, const lv_area_t *, lv_color_t *)>::fn =
    //   std::bind(&TPC_LCD::flush, this,
    //   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    // _ddrv.flush_cb = static_cast<flush_t>(
    //   Callback<void(lv_disp_drv_t *, const lv_area_t *, lv_color_t *)>::callback);
    _ddrv.draw_buf = &_draw;
    _disp = lv_disp_drv_register(&_ddrv);
  }

  static void init_input() {
    lv_indev_drv_init(&_idrv);
    _idrv.type = LV_INDEV_TYPE_POINTER;
    // Callback<void(lv_indev_drv_t *, lv_indev_data_t *)>::fn =
    //   std::bind(&TPC_LCD::read, this,
    //   std::placeholders::_1, std::placeholders::_2);
    // _idrv.read_cb = static_cast<read_t>(
    //   Callback<void(lv_indev_drv_t *, lv_indev_data_t *)>::callback);
    _idrv.read_cb = [](lv_indev_drv_t *drv, lv_indev_data_t *data) {}
    _inpt = lv_indev_drv_register(&_idrv);
  }

public:
  core() = default;

  static void init(Log log = log) {
    init_runtime(log);
    lv_init();
    init_display();
    init_input();
    // lv_obj_clean(lv_scr_act()); // TBD: necessary?
  }
};

}

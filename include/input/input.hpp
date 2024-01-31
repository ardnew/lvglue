#pragma once

#include <lvgl.h>

namespace glue {

class input {
private:
  static inline lv_indev_t *_inpt;
public:
  static void init(lv_indev_read_cb_t read,
    lv_indev_type_t type = LV_INDEV_TYPE_POINTER) {
    _inpt = lv_indev_create();
    lv_indev_set_type(_inpt, type);
    lv_indev_set_read_cb(_inpt, read);
  }
};

} // namespace glue

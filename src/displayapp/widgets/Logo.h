#pragma once

#include <vector>

#include <lvgl/lvgl.h>

#include "components/fs/FS.h"

namespace Pinetime::Applications::Widgets {
  class Logo {
  public:
    Logo(Controllers::FS& filesystem, lv_color_t color);\
    bool TryCreate();

    lv_obj_t* GetObject() {
      return image;
    }

  private:
    static lv_img_dsc_t* imageDesc;

    Controllers::FS& filesystem;
    lv_obj_t* image;
    lv_color_t color;
  };
}
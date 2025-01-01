#include "displayapp/widgets/Logo.h"
#include "nrf_log.h"

using namespace Pinetime::Applications::Widgets;

constexpr const char* const LOGO_PATH = "/images/logo.bin";

lv_img_dsc_t* Logo::imageDesc = nullptr;

Logo::Logo(Controllers::FS& filesystem, lv_color_t color)
  : filesystem(filesystem),
    image(nullptr),
    color(color) {

}

bool Logo::TryCreate() {
  if (imageDesc == nullptr) {
    lfs_info info;
    lfs_file_t logoFile;

    if (filesystem.FileOpen(&logoFile, LOGO_PATH, LFS_O_RDONLY) >= 0) {
      NRF_LOG_INFO("[Widgets::Logo] Logo file successfully opened");
      imageDesc = lv_img_buf_alloc(192, 192, LV_IMG_CF_INDEXED_1BIT);
      uint32_t readSize = 0;

      // skip signature
      if ((imageDesc != nullptr) && (filesystem.FileSeek(&logoFile, 4) >= 0)) {
        NRF_LOG_DEBUG("[Widgets::Logo] Logo file skipped first 4 bytes")
        readSize = filesystem.FileRead(
          &logoFile,
          const_cast<uint8_t*>(imageDesc->data),
          imageDesc->data_size
          );
        NRF_LOG_DEBUG("[Widgets::Logo] Logo file read %d bytes (expected %d)", readSize, imageDesc->data_size)
      }

      // lfs_info inf;
      // filesystem.Stat(LOGO_PATH, &inf);
      // NRF_LOG_DEBUG("Actual file size: %d (%s)", inf.size, inf.name);

      filesystem.FileClose(&logoFile);

      if (imageDesc == nullptr) {
        NRF_LOG_DEBUG("Allocation of imageDesc failed");
      } else if (readSize != imageDesc->data_size) {
        NRF_LOG_DEBUG("Read size %d doesn't match expected size %d, failing...", readSize, imageDesc->data_size);
        lv_img_buf_free(imageDesc);
        imageDesc = nullptr;
      }
    }
  }

  if ((imageDesc != nullptr) && (image == nullptr)) {
    image = lv_img_create(lv_scr_act(), nullptr);
    lv_img_set_src(image, imageDesc);
    lv_obj_set_style_local_image_recolor_opa(image, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_obj_set_style_local_image_recolor(image, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, color);
  }

  return true;
}

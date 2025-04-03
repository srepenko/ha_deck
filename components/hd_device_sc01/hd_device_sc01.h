#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "LGFX.h"
#include "lvgl.h"

LV_IMG_DECLARE(bg_480x320);

namespace esphome {
namespace hd_device {

class HaDeckDevice : public Component
{
public:
private:
    unsigned long time_ = 0;
    uint8_t brightness_ = 0;
};

}  // namespace hd_device
}  // namespace esphome
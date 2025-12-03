#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "lvgl.h"
#include "pins_config.h"
#include "jd9365_lcd.h"
#include "gsl3680_touch.h"


LV_IMG_DECLARE(bg_1280x800);

namespace esphome {
namespace hd_device {

class HaDeckDevice : public Component
{
public:
    void setup() override;
    void loop() override;
    float get_setup_priority() const override;
    uint8_t get_brightness();
    void set_brightness(uint8_t value);
private:
    unsigned long time_ = 0;
    uint8_t brightness_ = 0;
};

}  // namespace hd_device
}  // namespace esphome
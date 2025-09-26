#pragma once
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace shtx_legacy {

class ShtxLegacy : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { 
    temperature_sensor_ = temperature_sensor; 
  }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) { 
    humidity_sensor_ = humidity_sensor; 
  }

 protected:
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};
  
  bool read_data_(float &temperature, float &humidity);
  uint8_t crc8_(const uint8_t *data, uint8_t len);
};

}  // namespace shtx_legacy
}  // namespace esphome

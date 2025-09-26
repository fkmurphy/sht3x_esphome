#pragma once
#include "esphome.h"

namespace esphome {
namespace shtx_legacy {

class Shtxlegacy : public PollingComponent, public Sensor {
 public:
  Shtxlegacy() : PollingComponent(60000) {}   // lee cada 60 s
  void setup() override;
  void update() override;

  void set_temperature_sensor(Sensor *s) { temperature_sensor_ = s; }
  void set_humidity_sensor  (Sensor *s) { humidity_sensor_   = s; }

 private:
  Sensor *temperature_sensor_{nullptr};
  Sensor *humidity_sensor_{nullptr};
};

}  // namespace shtx_legacy
}  // namespace esphome

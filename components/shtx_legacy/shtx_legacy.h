#ifndef SHTX_LEGACY_H
#define SHTX_LEGACY_H

#include "esphome.h"

namespace esphome {
namespace shtx_legacy {

class SHTxLegacy : public PollingComponent, public Sensor {
 public:
  SHTxLegacy() : PollingComponent(60000) {}  // Update cada 60s
  void setup() override;
  void update() override;
  float get_temperature() const;
  float get_humidity() const;

 private:
  float temperature = NAN;
  float humidity = NAN;
};

}  // namespace shtx_legacy
}  // namespace esphome

#endif

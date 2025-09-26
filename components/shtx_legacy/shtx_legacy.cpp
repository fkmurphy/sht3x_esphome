#include "shtx_legacy.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace shtx_legacy {

static const char *const TAG = "shtx_legacy";

// Comandos SHT30
static const uint16_t SHT3X_CMD_MEASURE_HIGHREP = 0x2400;
static const uint16_t SHT3X_CMD_STATUS = 0xF32D;
static const uint16_t SHT3X_CMD_SOFTRESET = 0x30A2;

void ShtxLegacy::setup() {
  ESP_LOGCONFIG(TAG, "Setting up SHT3x Legacy...");
  
  // Realizar soft reset
  if (this->write_register16(SHT3X_CMD_SOFTRESET, nullptr, 0) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Soft reset failed!");
    this->mark_failed();
    return;
  }
  
  delay(10);
  ESP_LOGCONFIG(TAG, "SHT3x Legacy setup completed");
}

void ShtxLegacy::update() {
  float temperature, humidity;
  if (this->read_data_(temperature, humidity)) {
    if (this->temperature_sensor_ != nullptr) {
      this->temperature_sensor_->publish_state(temperature);
    }
    if (this->humidity_sensor_ != nullptr) {
      this->humidity_sensor_->publish_state(humidity);
    }
  } else {
    ESP_LOGW(TAG, "Failed to read sensor data");
  }
}

void ShtxLegacy::dump_config() {
  ESP_LOGCONFIG(TAG, "SHT3x Legacy:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with SHT3x failed!");
  }
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
  LOG_SENSOR("  ", "Humidity", this->humidity_sensor_);
}

bool ShtxLegacy::read_data_(float &temperature, float &humidity) {
  // Enviar comando de medición
  if (this->write_register16(SHT3X_CMD_MEASURE_HIGHREP, nullptr, 0) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to send measurement command");
    return false;
  }
  
  // Esperar a que la medición esté lista
  delay(15);  // 15ms para alta repetibilidad
  
  // Leer 6 bytes de datos
  uint8_t data[6];
  if (this->read(data, 6) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to read measurement data");
    return false;
  }
  
  // Verificar CRC para temperatura
  if (this->crc8_(data, 2) != data[2]) {
    ESP_LOGW(TAG, "Temperature CRC check failed");
    return false;
  }
  
  // Verificar CRC para humedad
  if (this->crc8_(data + 3, 2) != data[5]) {
    ESP_LOGW(TAG, "Humidity CRC check failed");
    return false;
  }
  
  // Convertir datos
  uint16_t raw_temp = (data[0] << 8) | data[1];
  uint16_t raw_hum = (data[3] << 8) | data[4];
  
  temperature = 175.0f * (float) raw_temp / 65535.0f - 45.0f;
  humidity = 100.0f * (float) raw_hum / 65535.0f;
  
  ESP_LOGV(TAG, "Raw temp: 0x%04X, temp: %.2f°C", raw_temp, temperature);
  ESP_LOGV(TAG, "Raw hum: 0x%04X, hum: %.2f%%", raw_hum, humidity);
  
  return true;
}

uint8_t ShtxLegacy::crc8_(const uint8_t *data, uint8_t len) {
  // CRC-8 checksum con polinomio 0x131 (x^8 + x^5 + x^4 + 1)
  const uint8_t POLYNOMIAL = 0x31;
  uint8_t crc = 0xFF;
  
  for (uint8_t j = len; j; --j) {
    crc ^= *data++;
    for (uint8_t i = 8; i; --i) {
      crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
    }
  }
  return crc;
}

}  // namespace shtx_legacy
}  // namespace esphome

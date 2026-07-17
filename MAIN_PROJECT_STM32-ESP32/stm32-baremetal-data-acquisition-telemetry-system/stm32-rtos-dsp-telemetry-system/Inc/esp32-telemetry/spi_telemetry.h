#ifndef SPI_TELEMETRY_H
#define SPI_TELEMETRY_H

#include <stdint.h>

// Telemetry structure to send via SPI (20 bytes)
typedef struct {
    float temperature;
    float pressure;
    float altitude;
    float vertical_speed;
    uint32_t timestamp;
} TelemetryPacket_t;

// Initialize SPI1 for telemetry transmission using D10-D13 pins
void Telemetry_SPI_Init(void);

// Pack and transmit data over SPI1, manually managing PB6 (CS)
void Telemetry_Send(float temp, float press, float alt, float v_speed);

#endif // SPI_TELEMETRY_H

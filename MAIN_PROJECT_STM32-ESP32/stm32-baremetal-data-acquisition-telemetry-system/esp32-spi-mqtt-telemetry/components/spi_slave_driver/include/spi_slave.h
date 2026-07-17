// ============================================================
// ESP32 Bare-Metal SPI Slave Driver
// ============================================================
#ifndef SPI_SLAVE_H
#define SPI_SLAVE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Telemetry Packet Structure (Make sure this exactly matches the master)
typedef struct {
    float temperature;
    float pressure;
    float altitude;
    float vertical_speed;
    uint32_t timestamp;
} __attribute__((packed)) TelemetryPacket_t;

/**
 * @brief Initialize SPI3 (VSPI) in slave mode
 * 
 * @param mosi Pin for MOSI (Master Out Slave In) - input to slave
 * @param miso Pin for MISO (Master In Slave Out) - output from slave
 * @param sclk Pin for SCLK (Clock) - input to slave
 * @param cs   Pin for CS (Chip Select) - input to slave
 * @param mode SPI Mode (0, 1, 2, or 3)
 * @return true if successful
 */
bool SPI_Slave_Init(int mosi, int miso, int sclk, int cs, int mode);

/**
 * @brief Wait for and receive a telemetry packet
 * 
 * @param packet Pointer to packet struct to store data
 * @return true if successful
 */
bool SPI_Slave_Receive(TelemetryPacket_t *packet);

/**
 * @brief Prepare the next response to send to the master
 * 
 * @param packet Pointer to packet struct containing response
 */
void SPI_Slave_LoadResponse(const TelemetryPacket_t *packet);

#endif // SPI_SLAVE_H

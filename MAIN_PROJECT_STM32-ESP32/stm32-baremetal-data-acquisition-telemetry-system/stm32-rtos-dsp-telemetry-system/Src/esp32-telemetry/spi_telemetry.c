#include "esp32-telemetry/spi_telemetry.h"
#include "drivers/stm32f446xx_spi_driver.h"
#include "drivers/stm32f446xx_gpio_driver.h"
#include "drivers/stm32f446xx.h"
#include "BoardConfig.h"

SPI_Handle_t SPI1Handle;

static void SPI1_GPIOInits(void) {
    // Enable GPIO clocks
    GPIO_PeriClockControl(GPIOA, ENABLE);
    GPIO_PeriClockControl(GPIOB, ENABLE);

    GPIO_Handle_t SPIPins;
    
    // Configure PA5 (SCK/D13), PA6 (MISO/D12), PA7 (MOSI/D11) for SPI1
    SPIPins.pGPIOx = BOARD_SPI1_PORT;
    SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = BOARD_SPI1_AF; // AF5 for SPI1
    SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    // SCK
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = BOARD_SPI1_SCK_PIN;
    GPIO_Init(&SPIPins);

    // MISO
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = BOARD_SPI1_MISO_PIN;
    GPIO_Init(&SPIPins);

    // MOSI
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = BOARD_SPI1_MOSI_PIN;
    GPIO_Init(&SPIPins);

    // Configure PB6 (CS/D10) as standard GPIO output (Software Slave Management)
    GPIO_Handle_t CSPin;
    CSPin.pGPIOx = BOARD_SPI1_CS_PORT;
    CSPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    CSPin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    CSPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    CSPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    CSPin.GPIO_PinConfig.GPIO_PinNumber = BOARD_SPI1_CS_PIN;
    
    GPIO_Init(&CSPin);
    
    // Set CS high initially (idle state)
    GPIO_WriteToOutputPin(BOARD_SPI1_CS_PORT, BOARD_SPI1_CS_PIN, 1);
}

static void SPI1_Inits(void) {
    // Enable SPI1 Clock
    SPI_PeriClockControl(SPI1, ENABLE);

    SPI1Handle.pSPIx = SPI1;
    SPI1Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    SPI1Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    SPI1Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV64; // Slow down clock for breadboard stability!
    SPI1Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
    SPI1Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    SPI1Handle.SPIConfig.SPI_CPHA = SPI_CPHA_HIGH; // Switch to Mode 1 to fix 1-bit shift!
    SPI1Handle.SPIConfig.SPI_SSM = SPI_SSM_EN; // Software Slave Management ENABLED

    SPI_Init(&SPI1Handle);
    
    // Set SSI bit to 1 to avoid MODF error when SSM=1
    SPI_SSIConfig(SPI1, ENABLE);
}

void Telemetry_SPI_Init(void) {
    SPI1_GPIOInits();
    SPI1_Inits();
    
    // Enable SPI peripheral
    SPI_PeripheralControl(SPI1, ENABLE);
}

void Telemetry_Send(float temp, float press, float alt, float v_speed) {
    static uint32_t current_timestamp = 0; // Simple timestamp simulation
    
    TelemetryPacket_t packet;
    packet.temperature = temp;
    packet.pressure = press;
    packet.altitude = alt;
    packet.vertical_speed = v_speed;
    packet.timestamp = current_timestamp++; // Add timestamp to match ESP32 exactly!

    // 1. Pull CS low to initiate transmission
    GPIO_WriteToOutputPin(BOARD_SPI1_CS_PORT, BOARD_SPI1_CS_PIN, 0);
    
    // VERY IMPORTANT: Give ESP32 time to wake up and sync to the CS falling edge!
    // Without this delay, the STM32 sends the clock too fast and the ESP32 misses the first bit!
    for(volatile int i = 0; i < 500; i++); 

    // SEND AND RECEIVE AT THE SAME TIME
    TelemetryPacket_t rx_packet;
    
    // We send 20 bytes (sizeof TelemetryPacket_t). Wait for TXE and RXNE flags.
    uint8_t *tx_ptr = (uint8_t*)&packet;
    uint8_t *rx_ptr = (uint8_t*)&rx_packet;
    
    for (int i = 0; i < sizeof(TelemetryPacket_t); i++) {
        while(!SPI_GetFlagStatus(SPI1, SPI_TXE_FLAG)); // Wait until TX buffer empty
        SPI1->DR = tx_ptr[i]; // Send 1 byte
        
        while(!SPI_GetFlagStatus(SPI1, SPI_RXNE_FLAG)); // Wait until RX buffer full
        rx_ptr[i] = SPI1->DR; // Read 1 byte from ESP32!
    }
    
    // 3. Wait for SPI to not be busy (Optional but recommended for software CS)
    while(SPI_GetFlagStatus(SPI1, SPI_BUSY_FLAG));
    
    // Give the ESP32 time to process the final clock edge before yanking CS high
    for(volatile int i = 0; i < 500; i++);

    // 4. Pull CS high to terminate transmission
    GPIO_WriteToOutputPin(BOARD_SPI1_CS_PORT, BOARD_SPI1_CS_PIN, 1);

    // Check if the ESP32 sent the Magic SMS Trigger Packet
    if (rx_packet.timestamp == 0xAA55AA55) {
        extern volatile uint8_t trigger_sms_from_dashboard;
        trigger_sms_from_dashboard = 1;
    }
}

// ============================================================
// ESP32 Bare-Metal SPI Slave Driver
// Direct register manipulation via SPI peripheral struct.
// ============================================================
#include "spi_slave.h"
#include "soc/spi_struct.h"
#include "soc/gpio_struct.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_sig_map.h"
#include "soc/dport_reg.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

extern const uint32_t GPIO_PIN_MUX_REG[40];

// VSPI (SPI3) slave signal mappings
#define VSPI_MOSI_IN_IDX   VSPID_IN_IDX
#define VSPI_MISO_OUT_IDX  VSPIQ_OUT_IDX
#define VSPI_SCLK_IN_IDX   VSPICLK_IN_IDX
#define VSPI_CS0_IN_IDX    VSPICS0_IN_IDX

static void spi_route_pin_in(int pin, uint8_t signal) {
    if (pin < 0 || pin > 39) return;
    uint32_t mux = GPIO_PIN_MUX_REG[pin];
    uint32_t val = *(volatile uint32_t *)mux;
    val = (val & ~(MCU_SEL_M)) | (PIN_FUNC_GPIO << MCU_SEL_S);
    *(volatile uint32_t *)mux = val;
    *(volatile uint32_t *)mux |= FUN_IE;
    GPIO.func_in_sel_cfg[signal].func_sel = pin;
    GPIO.func_in_sel_cfg[signal].sig_in_sel = 1;
    GPIO.func_in_sel_cfg[signal].sig_in_inv = 0;
}

static void spi_route_pin_out(int pin, uint8_t signal) {
    if (pin < 0 || pin > 33) return;
    uint32_t mux = GPIO_PIN_MUX_REG[pin];
    uint32_t val = *(volatile uint32_t *)mux;
    val = (val & ~(MCU_SEL_M)) | (PIN_FUNC_GPIO << MCU_SEL_S);
    *(volatile uint32_t *)mux = val;
    GPIO.func_out_sel_cfg[pin].func_sel = signal;
    GPIO.func_out_sel_cfg[pin].inv_sel = 0;
    GPIO.func_out_sel_cfg[pin].oen_sel = 0;
    GPIO.enable_w1ts = (1U << pin);
}

bool SPI_Slave_Init(int mosi, int miso, int sclk, int cs, int mode) {
    // 1. Enable peripheral clock and clear reset for SPI3
    DPORT_SET_PERI_REG_MASK(DPORT_PERIP_CLK_EN_REG, DPORT_SPI3_CLK_EN);
    DPORT_CLEAR_PERI_REG_MASK(DPORT_PERIP_RST_EN_REG, DPORT_SPI3_RST);

    // 2. Route pins via GPIO matrix
    spi_route_pin_in(mosi, VSPI_MOSI_IN_IDX);
    spi_route_pin_out(miso, VSPI_MISO_OUT_IDX);
    spi_route_pin_in(sclk, VSPI_SCLK_IN_IDX);
    spi_route_pin_in(cs, VSPI_CS0_IN_IDX);

    // 3. Reset SPI peripheral
    SPI3.slave.val = 0;
    SPI3.slave.sync_reset = 1;
    SPI3.slave.sync_reset = 0;

    // 4. Clear config registers
    SPI3.user.val = 0;
    SPI3.user1.val = 0;
    SPI3.user2.val = 0;
    SPI3.pin.val = 0;
    SPI3.ctrl.val = 0;
    SPI3.dma_conf.val = 0;

    // 5. Configure Slave Mode
    SPI3.slave.slave_mode = 1;
    SPI3.slave.wr_rd_buf_en = 1; // Enable buffer mode for slave
    
    // 6. SPI Mode (CPOL, CPHA)
    // ESP32 ck_out_edge=1 means "sample on rising edge". ck_out_edge=0 means "sample on falling edge".
    switch (mode) {
        case 0: SPI3.pin.ck_idle_edge = 0; SPI3.user.ck_out_edge = 1; break; // Mode 0: Sample on Rising
        case 1: SPI3.pin.ck_idle_edge = 0; SPI3.user.ck_out_edge = 0; break; // Mode 1: Sample on Falling
        case 2: SPI3.pin.ck_idle_edge = 1; SPI3.user.ck_out_edge = 0; break; // Mode 2: Sample on Falling
        case 3: SPI3.pin.ck_idle_edge = 1; SPI3.user.ck_out_edge = 1; break; // Mode 3: Sample on Rising
    }

    // 7. Full-duplex setup and enable MOSI/MISO phases!
    SPI3.user.doutdin = 1;
    SPI3.user.usr_mosi = 1; // CRITICAL: Enable MOSI phase to receive data!
    SPI3.user.usr_miso = 1; // Enable MISO phase to send data!

    // 8. Prepare for first transaction (Set buffer lengths to exact size: 20 bytes * 8 = 160 bits)
    // Register value is bits - 1, so 159
    SPI3.slv_rdbuf_dlen.val = 159; 
    SPI3.slv_wrbuf_dlen.val = 159; 

    // Enable slave to receive
    SPI3.cmd.usr = 1;

    return true;
}

bool SPI_Slave_Receive(TelemetryPacket_t *packet) {
    if (!packet) return false;

    // Wait until transaction is done with a timeout for debugging
    int timeout_ms = 2000; // 2 seconds timeout
    while (SPI3.slave.trans_done == 0) {
        vTaskDelay(1); // Yield to IDLE task
        timeout_ms--;
        if (timeout_ms <= 0) {
            printf("\n[DEBUG] SPI Timeout! ESP32 never received the full transaction.\n");
            printf("Register Dump -> SPI3.cmd.usr: %d, SPI3.slave.trans_done: %d\n", SPI3.cmd.usr, SPI3.slave.trans_done);
            printf("Please double check your 5 jumper wires (MOSI, MISO, SCLK, CS, GND)!\n\n");
            return false;
        }
    }

    // Read the received data directly from the FIFO
    // Since we fixed the CPHA 1-bit shift, the data arrives perfectly aligned!
    uint32_t word_buf[16];
    for (int i = 0; i < (sizeof(TelemetryPacket_t) + 3) / 4; i++) {
        word_buf[i] = SPI3.data_buf[i]; // NO BYTE SWAP NEEDED!
    }
    memcpy(packet, word_buf, sizeof(TelemetryPacket_t));

    // Clear trans_done flag
    SPI3.slave.trans_done = 0;

    // Re-arm the slave for the next transaction
    SPI3.cmd.usr = 1;

    return true;
}

void SPI_Slave_LoadResponse(const TelemetryPacket_t *packet) {
    if (!packet) return;

    // Load data into TX FIFO for the next transaction
    uint32_t word_buf[16] = {0};
    memcpy(word_buf, packet, sizeof(TelemetryPacket_t));
    for (int i = 0; i < (sizeof(TelemetryPacket_t) + 3) / 4; i++) {
        SPI3.data_buf[i] = word_buf[i];
    }
}

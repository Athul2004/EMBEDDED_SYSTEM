# ============================================================
# SPI Slave Bare-Metal Driver
# ============================================================
idf_component_register(
    SRCS "spi_slave.c"
    INCLUDE_DIRS "include"
    REQUIRES "soc"
)

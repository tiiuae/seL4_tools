#include <devices_gen.h>
#include <drivers/common.h>
#include <drivers/uart.h>

#include <elfloader_common.h>

#define MBOX_REG(mmio) ((volatile uint32_t *)(mmio))
#define MBOX_FULL (1 << 31)

static int tcu_putchar(struct elfloader_device *dev, unsigned int c)
{
    volatile void *mmio = dev->region_bases[0];

    while (*MBOX_REG(mmio) & MBOX_FULL);

    *MBOX_REG(mmio) = (c & 0xff) | (1 << 24) | MBOX_FULL;

    return 0;
}

static int tcu_init(struct elfloader_device *dev, UNUSED void *match_data)
{
    uart_set_out(dev);
    return 0;
}

static const struct dtb_match_table tcu_matches[] = {
    { .compatible = "nvidia,tegra194-tcu" },
    { .compatible = NULL /* sentinel */ },
};

static const struct elfloader_uart_ops tcu_ops = {
    .putc = &tcu_putchar,
};

static const struct elfloader_driver tcu_uart = {
    .match_table = tcu_matches,
    .type = DRIVER_UART,
    .init = &tcu_init,
    .ops = &tcu_ops,
};

ELFLOADER_DRIVER(tcu_uart);

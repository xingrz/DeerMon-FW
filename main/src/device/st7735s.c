#define TAG "ST7735S"

#include "st7735s.h"

#include <stdlib.h>
#include <string.h>

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pinout.h"

#define ST7735S_DMA_CHANNEL 2

#define REG_SLPIN 0x10
#define REG_SLPOUT 0x11
#define REG_PTLON 0x12
#define REG_NORON 0x13
#define REG_INVOFF 0x20
#define REG_INVON 0x21
#define REG_GAMSET 0x26
#define REG_DISPOFF 0x28
#define REG_DISPON 0x29
#define REG_CASET 0x2A
#define REG_RASET 0x2B
#define REG_RAMWR 0x2C
#define REG_RGBSET 0x2D
#define REG_RAMRD 0x2E
#define REG_PTLAR 0x30
#define REG_SCRLAR 0x33
#define REG_TEOFF 0x34
#define REG_TEON 0x35
#define REG_MADCTL 0x36
#define REG_VSCSAD 0x37
#define REG_IDMOFF 0x38
#define REG_IDMON 0x39
#define REG_COLMOD 0x3A
#define REG_FRMCTR1 0xB1
#define REG_FRMCTR2 0xB2
#define REG_FRMCTR3 0xB3
#define REG_INVCTR 0xB4
#define REG_PWCTR1 0xC0
#define REG_PWCTR2 0xC1
#define REG_PWCTR3 0xC2
#define REG_PWCTR4 0xC3
#define REG_PWCTR5 0xC4
#define REG_VMCTR1 0xC5
#define REG_VMOFCTR 0xC7
#define REG_GMCTRP1 0xE0
#define REG_GMCTRN1 0xE1

#define MADCTL_MY 1  // Row Address Order
#define MADCTL_MX 1  // Column Address Order
#define MADCTL_MV 0  // Row/Column Exchange
#define MADCTL_ML 0  // Vertical Refresh Order (0 = Top to Buttom; 1 = Buttom to Top)
#define MADCTL_RGB 1  // RGB-BGR Order (0 = RGB; 1 = BGR)
#define MADCTL_MH 0  // Horizontal Refresh Order (0 = Left to Right; 1 = Right to Left)

#define TRANS_CMD (void *)0
#define TRANS_DATA (void *)1

#define COUNT(x) (sizeof(x) / sizeof(x[0]))

static struct {
	uint8_t cmd;
	uint8_t len;
	uint8_t data[16];
} st7735s_reg_conf[] = {
	{REG_FRMCTR1, 3, {0x02, 0x35, 0x36}},
	{REG_FRMCTR2, 3, {0x02, 0x35, 0x36}},
	{REG_FRMCTR3, 6, {0x02, 0x35, 0x36, 0x02, 0x35, 0x36}},
	{REG_INVCTR, 1, {0x03}},
	{REG_PWCTR1, 3, {0xA2, 0x02, 0x84}},
	{REG_PWCTR2, 1, {0xC5}},
	{REG_PWCTR3, 2, {0x0D, 0x00}},
	{REG_PWCTR4, 2, {0x8D, 0x2A}},
	{REG_PWCTR5, 2, {0x8D, 0xEE}},
	{REG_VMCTR1, 1, {0x0A}},
	{REG_INVOFF, 0, {}},
	{REG_MADCTL, 1,
		{(MADCTL_MY << 7) | (MADCTL_MX << 6) | (MADCTL_MV << 5) | (MADCTL_ML << 4) |
			(MADCTL_RGB << 3) | (MADCTL_MH << 2)}},
	{REG_COLMOD, 1, {0x05}},
	{REG_GMCTRP1, 16,
		{0x12, 0x1C, 0x10, 0x18, 0x33, 0x2C, 0x25, 0x28, 0x28, 0x27, 0x2F, 0x3C, 0x00, 0x03, 0x03,
			0x10}},
	{REG_GMCTRN1, 16,
		{0x12, 0x1C, 0x10, 0x18, 0x2D, 0x28, 0x23, 0x28, 0x28, 0x26, 0x2F, 0x3B, 0x00, 0x03, 0x03,
			0x10}},
	{REG_NORON, 0, {}},
	{REG_DISPON, 0, {}},
};

static spi_device_handle_t spi;

static void IRAM_ATTR write_reg(uint8_t val);
static void IRAM_ATTR write_data(void *buf, uint32_t len);
static void IRAM_ATTR spi_pre_transfer_callback(spi_transaction_t *t);

void
st7735s_init(void)
{
	ESP_LOGI(TAG, "Init interfaces...");

	gpio_config_t output = {
		.pin_bit_mask = (1UL << PIN_LCD_DC) | (1UL << PIN_LCD_RST),
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = GPIO_PULLUP_ENABLE,
	};

	ESP_ERROR_CHECK(gpio_config(&output));

	spi_bus_config_t bus = {
		.mosi_io_num = PIN_LCD_SDA,
		.miso_io_num = -1,
		.sclk_io_num = PIN_LCD_SCL,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = ST7735S_WIDTH * 8 * sizeof(uint16_t),
		.flags = SPICOMMON_BUSFLAG_MASTER | SPICOMMON_BUSFLAG_IOMUX_PINS,
	};

	ESP_ERROR_CHECK(spi_bus_initialize(HSPI_HOST, &bus, ST7735S_DMA_CHANNEL));

	spi_device_interface_config_t dev = {
		.clock_speed_hz = SPI_MASTER_FREQ_40M,
		.mode = 0,
		.spics_io_num = -1,
		.queue_size = 7,
		.pre_cb = spi_pre_transfer_callback,
	};

	ESP_ERROR_CHECK(spi_bus_add_device(HSPI_HOST, &dev, &spi));

	ESP_LOGI(TAG, "Reset panel...");

	gpio_set_level(PIN_LCD_RST, 0);
	vTaskDelay(100 / portTICK_PERIOD_MS);
	gpio_set_level(PIN_LCD_RST, 1);
	vTaskDelay(100 / portTICK_PERIOD_MS);

	ESP_LOGI(TAG, "Init ST7735S...");

	write_reg(REG_SLPOUT);
	vTaskDelay(120 / portTICK_PERIOD_MS);
	for (int i = 0; i < COUNT(st7735s_reg_conf); i++) {
		write_reg(st7735s_reg_conf[i].cmd);
		if (st7735s_reg_conf[i].len > 0) {
			write_data(st7735s_reg_conf[i].data, st7735s_reg_conf[i].len);
		}
	}

	ESP_LOGI(TAG, "Init done");
}

#define SWAP(u16) (((u16 & 0xFF) << 8) | (u16 >> 8))

typedef struct {
	uint16_t start;
	uint16_t end;
} st7735s_write_address_t;

static void
st7735s_address_set(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2)
{
	st7735s_write_address_t addr;

	write_reg(REG_CASET);
	addr.start = SWAP(x1);
	addr.end = SWAP(x2);
	write_data(&addr, sizeof(addr));

	write_reg(REG_RASET);
	addr.start = SWAP(y1);
	addr.end = SWAP(y2);
	write_data(&addr, sizeof(addr));
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void IRAM_ATTR
st7735s_draw(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint16_t *pixels)
{
	st7735s_address_set(x1, x2, y1, y2);

	write_reg(REG_RAMWR);
	write_data(pixels, (x2 - x1 + 1) * (y2 - y1 + 1) * sizeof(uint16_t));
}

void
st7735s_fill(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint16_t color)
{
	uint16_t pixels[ST7735S_BUF_CNT];
	for (int i = 0; i < COUNT(pixels); i++) {
		pixels[i] = SWAP(color);
	}

	st7735s_address_set(x1, x2, y1, y2);

	write_reg(REG_RAMWR);
	int pixel_bytes = (x2 - x1 + 1) * (y2 - y1 + 1) * sizeof(uint16_t);
	for (int i = 0; i < pixel_bytes; i += sizeof(pixels)) {
		write_data(pixels, MIN(sizeof(pixels), pixel_bytes - i));
	}
}

static void IRAM_ATTR
hspi_write(void *buf, uint32_t len, void *dc)
{
	spi_transaction_t trans = {
		.tx_buffer = buf,
		.length = len * 8,
		.user = dc,
	};

	ESP_ERROR_CHECK(spi_device_polling_transmit(spi, &trans));
}

static void IRAM_ATTR
write_reg(uint8_t val)
{
	hspi_write(&val, sizeof(uint8_t), TRANS_CMD);
}

static void IRAM_ATTR
write_data(void *buf, uint32_t len)
{
	hspi_write(buf, len, TRANS_DATA);
}

static void IRAM_ATTR
spi_pre_transfer_callback(spi_transaction_t *t)
{
	gpio_set_level(PIN_LCD_DC, (uint32_t)t->user);
}

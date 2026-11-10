#include "lcd.h"

/* ---- Wiring ---------------------------------------------------------------
   The pins are the GPIO outputs labeled LCD_RS, LCD_E, LCD_D4 .. LCD_D7 in
   CubeMX. MX_GPIO_Init() configures them, so to move the LCD to other pins,
   change the pins and labels in the .ioc file and nothing here. */

/* ---- RS levels: tell the LCD whether a byte is a command or a character - */
#define LCD_RS_COMMAND  0
#define LCD_RS_DATA     1

/* ---- HD44780 commands --------------------------------------------------- */
#define LCD_CMD_CLEAR         0x01  /* clear screen, cursor to start        */
#define LCD_CMD_ENTRY_MODE    0x06  /* cursor moves right after each char   */
#define LCD_CMD_DISPLAY_ON    0x0C  /* display on, cursor hidden            */
#define LCD_CMD_FUNCTION_SET  0x28  /* 4-bit bus, 2 lines, 5x8 font         */
#define LCD_CMD_SET_DDRAM     0x80  /* set cursor address, OR with address  */
#define LCD_ROW1_OFFSET       0x40  /* address of first character of row 1  */

/* ---- Timing (from the HD44780 datasheet, rounded up) -------------------- */
#define LCD_ENABLE_PULSE_US   2     /* E must stay high at least ~0.5 us    */
#define LCD_COMMAND_DELAY_US  50    /* most commands finish within 37 us    */
#define LCD_CLEAR_DELAY_MS    2     /* clear command needs about 1.6 ms     */

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} lcd_pin_t;

static const lcd_pin_t data_pins[4] = {
    { LCD_D4_GPIO_Port, LCD_D4_Pin },
    { LCD_D5_GPIO_Port, LCD_D5_Pin },
    { LCD_D6_GPIO_Port, LCD_D6_Pin },
    { LCD_D7_GPIO_Port, LCD_D7_Pin },
};

/* Microsecond delay based on the DWT cycle counter of the Cortex-M4 core. */
static void delay_us_init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static void delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = us * (SystemCoreClock / 1000000U);

    while ((DWT->CYCCNT - start) < cycles) {
    }
}

static void write_pin(GPIO_TypeDef *port, uint16_t pin, uint8_t level)
{
    HAL_GPIO_WritePin(port, pin, level ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* E goes high then low: the LCD reads D4..D7 on the falling edge. */
static void pulse_enable(void)
{
    write_pin(LCD_E_GPIO_Port, LCD_E_Pin, 1);
    delay_us(LCD_ENABLE_PULSE_US);
    write_pin(LCD_E_GPIO_Port, LCD_E_Pin, 0);
}

static void write_nibble(uint8_t nibble)
{
    for (uint8_t bit = 0; bit < 4; bit++) {
        write_pin(data_pins[bit].port, data_pins[bit].pin, (nibble >> bit) & 1);
    }
    pulse_enable();
}

/* A byte is sent as two nibbles, high 4 bits first. */
static void send_byte(uint8_t value, uint8_t rs)
{
    write_pin(LCD_RS_GPIO_Port, LCD_RS_Pin, rs);
    write_nibble(value >> 4);
    write_nibble(value & 0x0F);
    delay_us(LCD_COMMAND_DELAY_US);
}

void lcd_clear(void)
{
    send_byte(LCD_CMD_CLEAR, LCD_RS_COMMAND);
    HAL_Delay(LCD_CLEAR_DELAY_MS);
}

void lcd_init(void)
{
    delay_us_init();

    HAL_Delay(50);                      /* wait for the LCD to power up */
    write_pin(LCD_RS_GPIO_Port, LCD_RS_Pin, LCD_RS_COMMAND);

    /* Reset sequence from the HD44780 datasheet: send 0x3 three times,
       then 0x2 to switch the LCD into 4-bit mode. */
    write_nibble(0x03);
    HAL_Delay(5);
    write_nibble(0x03);
    HAL_Delay(1);
    write_nibble(0x03);
    HAL_Delay(1);
    write_nibble(0x02);
    HAL_Delay(1);

    send_byte(LCD_CMD_FUNCTION_SET, LCD_RS_COMMAND);
    send_byte(LCD_CMD_DISPLAY_ON, LCD_RS_COMMAND);
    send_byte(LCD_CMD_ENTRY_MODE, LCD_RS_COMMAND);
    lcd_clear();
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
    uint8_t address = (uint8_t)((row == 0 ? 0x00 : LCD_ROW1_OFFSET) + col);

    send_byte(LCD_CMD_SET_DDRAM | address, LCD_RS_COMMAND);
}

void lcd_print(const char *text)
{
    while (*text) {
        send_byte((uint8_t)*text++, LCD_RS_DATA);
    }
}

void lcd_print_line(uint8_t row, const char *text)
{
    lcd_set_cursor(row, 0);
    for (uint8_t col = 0; col < LCD_COLS; col++) {
        char c = (*text != '\0') ? *text++ : ' ';
        send_byte((uint8_t)c, LCD_RS_DATA);
    }
}

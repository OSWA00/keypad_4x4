/*
|| @file pico_keypad4x4.c
|| @version 0.2
|| @author Oswaldo Hernandez, Owen Jauregui
|| @contact oswahdez00@gmail.com
||
|| @description
|| | This library provides a simple interface for using a 4x4 matrix
|| | keypad with the Raspberry Pico.
*/

#include "pico_keypad4x4.h"

#define ROWS 4
#define COLUMNS 4

#define GPIO_INPUT false
#define GPIO_OUTPUT true

uint _columns[4];
uint _rows[4];
char _matrix_values[16];

/**
 * @brief Set up the keypad
 *
 * @param columns Pins connected to keypad columns
 * @param rows Pins connected to keypad rows
 * @param matrix_values values assigned to each key
 */
void pico_keypad_init(uint columns[4], uint rows[4], char matrix_values[16]) {
    _columns = columns;
    _rows = rows;
    _matrix_values = matrix_values;

    for (int i = 0; i < 4; i++) {
        gpio_init(_columns[i]);
        gpio_init(_rows[i]);

        gpio_set_dir(_columns[i], GPIO_INPUT);
        gpio_set_dir(_rows[i], GPIO_OUTPUT);

        gpio_put(_rows[i], 1);
    }
}

/**
 * @brief Scan and get the pressed key.
 *
 * This routine returns the first key found to be pressed
 * during the scan.
 */
char pico_keypad_get_key(void) {
    int row;
    uint32_t cols;
    bool pressed = false;

    cols = gpio_get_all();
    cols = cols & 0x3C0000;
    if (cols == 0x0) {
        return 0;
    }

    for (int j = 0; j < 4; j++) {
        gpio_put(_rows[j], 0);
    }

    for (row = 0; row < 4; row++) {

        gpio_put(_rows[row], 1);

        busy_wait_us(10000);

        cols = gpio_get_all();
        gpio_put(_rows[row], 0);
        cols = cols & 0x3C0000;
        if (cols != 0x0) {
            break;
        }
    }

    for (int i = 0; i < 4; i++) {
        gpio_put(_rows[i], 1);
    }

    if (cols == 0x040000) return (char)matriz[row * 4 + 0];
    if (cols == 0x080000) return (char)matriz[row * 4 + 1];
    if (cols == 0x100000) return (char)matriz[row * 4 + 2];
    if (cols == 0x200000) return (char)matriz[row * 4 + 3];
    return 0;
}

/**
 * @brief Setup keypad to work with IRQ.
 *
 * Make keypad pins work as processor interruption
 *
 * @param enable Enable or disable interruption pins
 * @param callback Function of what will happen if the key is pressed
 */
void pico_keypad_irq_enable(bool enable, gpio_irq_callback_t callback) {
    for (int i = 0; i < 4; i++) {
        gpio_set_irq_enabled_with_callback(columns[i], 0x8, enable, callback);
    }
}

#include <stdint.h>
#include <stdbool.h>
#include "matrix.h"
#include "quantum.h"
#include "keymap.h"

static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

static void unselect_cols(void) {
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        if (col_pins[col] != NO_PIN) {
            setPinOutput(col_pins[col]);
            //writePinLow(col_pins[col]);
            writePinHigh(col_pins[col]);
        }
    }
}

static bool select_col(uint8_t col) {
    //writePinHigh(col_pins[col]);
    pin_t pin = col_pins[col];
    if (pin != NO_PIN) {
        writePinLow(pin);
        return true;
    }
    return false;
}

static bool read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col) {
    bool matrix_changed = false;

    // Select col and wait for col selecton to stabilize
    //select_col(current_col);
    if (!select_col(current_col)) { // select col
        return false;                     // skip NO_PIN col
    }
    //wait_us(30);
    matrix_output_select_delay();

    // row:0 , col:0 FN key is DIRECT_PIN
    /*
    if (current_col == 0) {

        matrix_row_t last_row_value = current_matrix[0];
        if (readPin(row_pins[0]) == 0) {
            // Pin LO, set col bit
            current_matrix[0] |= (1 << current_col);
        } else {
            // Pin HI, clear col bit
            current_matrix[0] &= ~(1 << current_col);
        }

        // Determine if the matrix changed state
        if ((last_row_value != current_matrix[0]) && !(matrix_changed)) {
            matrix_changed = true;
        }
    } */
    if (joy_type == DIGITAL) {
        matrix_row_t last_row_value = current_matrix[8];
        /*
        current_matrix[8][0] = arrow_state.UP;
        current_matrix[8][1] = arrow_state.DOWN;
        current_matrix[8][3] = arrow_state.LEFT;
        current_matrix[8][4] = arrow_state.RIGHT; 
        */
        //current_matrix[8] = *( (matrix_row_t *)(&arrow_state) );
        memcpy(current_matrix+8,&joy_digi_state,sizeof(joy_digi_state));

        // Determine if the matrix changed state
        if ((last_row_value != current_matrix[8]) && !(matrix_changed)) {
            matrix_changed = true;
        }
    }

    // other row use MATRIX
    for (uint8_t row_index = 0; row_index < MATRIX_ROWS-1; row_index++) {

        matrix_row_t last_row_value = current_matrix[row_index];
        if (readPin(row_pins[row_index]) == 0) {
            // Pin LO, set col bit
            current_matrix[row_index] |= (1 << current_col);
        } else {
            // Pin HI, clear col bit
            current_matrix[row_index] &= ~(1 << current_col);
        }

        // Determine if the matrix changed state
        if ((last_row_value != current_matrix[row_index]) && !(matrix_changed)) {
            matrix_changed = true;
        }
    }


    // Unselect cols
    unselect_cols();
    matrix_output_select_delay(); //this should be here??

    return matrix_changed;
}

void matrix_init_custom(void) {
    // initialize hardware and global matrix state here
    unselect_cols();

    // initialize key pins
    for (uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++) {
        if (row_pins[row_index] != NO_PIN) 
        setPinInputHigh(row_pins[row_index]);
    }

    matrix_init_kb();
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool changed = false;

    for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++) {
        changed |= read_rows_on_col(current_matrix, current_col);
    }

    matrix_scan_kb();

    return changed;
}

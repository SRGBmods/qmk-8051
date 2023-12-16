/* Copyright 2019 Nick Brassel (tzarc)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <string.h>

#include "eeprom_driver.h"
#include "ch555.h"

void eeprom_driver_init(void) {
    /* Any initialisation code */
 }

void eeprom_driver_erase(void) {
    /* Wipe out the EEPROM, setting values to zero */
}

void eeprom_read_block(void *buf, const void *addr, size_t len) {
    /*
        Read a block of data:
            buf: target buffer
            addr: 0-based offset within the EEPROM
            len: length to read
     */
}

void eeprom_write_block(const void *buf, void *addr, size_t len) {
    /*
        Write a block of data:
            buf: target buffer
            addr: 0-based offset within the EEPROM
            len: length to write
     */
  //  uint16_t Waddr = (intptr_t)addr; 
  //if (Waddr >= 128) {
  //  return;
  //}

  //SAFE_MOD = 0x55;
  //SAFE_MOD = 0xAA;        // Enter Safe mode
  //GLOBAL_CFG |= bDATA_WE; // Enable DataFlash write
  //SAFE_MOD = 0;           // Exit Safe mode
  //ROM_ADDR_H = DATA_FLASH_ADDR >> 8;
  //ROM_ADDR_L = addr << 1;
  //ROM_DATA_L = val;
  //if (ROM_STATUS & bROM_ADDR_OK) { // Valid access Address
  //  ROM_CTRL = ROM_CMD_WRITE;      // Write
  //}
  //SAFE_MOD = 0x55;
  //SAFE_MOD = 0xAA;         // Enter Safe mode
  //GLOBAL_CFG &= ~bDATA_WE; // Disable DataFlash write
  //SAFE_MOD = 0;            // Exit Safe mode
}

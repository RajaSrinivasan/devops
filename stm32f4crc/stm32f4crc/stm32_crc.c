//
//  stm32_crc.c
//  stm32f4crc
//
//  Created by Raja Srinivasan on 3/13/26.
//

#include "stm32_crc.h"
#include <stdint.h>
#include <stddef.h>

/* Replicates STM32F4 CRC unit exactly:
 * - Polynomial:        0x04C11DB7
 * - Initial value:     0xFFFFFFFF
 * - Input:             32-bit words only, no reflection
 * - Output:            no reflection, no final XOR
 * - Equivalent to:     CRC->CR = RESET; then CRC->DR = word; for each word
 */
#include <string.h>

/* Pack bytes big-endian — matches firmware doing:
 * CRC->DR = (buf[0]<<24)|(buf[1]<<16)|(buf[2]<<8)|buf[3]
 */
uint32_t stm32_crc32_bytes(const uint8_t *data, size_t len) {
    uint32_t crc = 0xFFFFFFFF;
    size_t i = 0;

    for (; i + 4 <= len; i += 4) {
        uint32_t word = (uint32_t)data[i    ] << 24 |
                        (uint32_t)data[i + 1] << 16 |
                        (uint32_t)data[i + 2] <<  8 |
                        (uint32_t)data[i + 3];
        crc = stm32_crc32_word(crc, word);
    }

    /* remaining bytes — pad with 0x00 to fill 32-bit word */
    if (i < len) {
        uint32_t word = 0;
        for (size_t j = 0; j < len - i; j++)
            word |= (uint32_t)data[i + j] << (24 - j * 8);
        crc = stm32_crc32_word(crc, word);
    }

    return crc;
}
uint32_t stm32_crc32_word(uint32_t crc, uint32_t word) {
    crc ^= word;
    for (int i = 0; i < 32; i++) {
        if (crc & 0x80000000)
            crc = (crc << 1) ^ 0x04C11DB7;
        else
            crc = (crc << 1);
    }
    return crc;
}

uint32_t stm32_crc32(const uint32_t *words, size_t word_count) {
    uint32_t crc = 0xFFFFFFFF;  /* hardware reset value */
    for (size_t i = 0; i < word_count; i++) {
        crc = stm32_crc32_word(crc, words[i]);
    }
    return crc;
}

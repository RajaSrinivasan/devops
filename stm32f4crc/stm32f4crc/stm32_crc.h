//
//  stm32_crc.h
//  stm32f4crc
//
//  Created by Raja Srinivasan on 3/13/26.
//

#ifndef STM32_CRC_H
#define STM32_CRC_H

#include <stdint.h>
#include <stddef.h>

uint32_t stm32_crc32_word(uint32_t crc, uint32_t word) ;
uint32_t stm32_crc32(const uint32_t *words, size_t word_count);

#endif

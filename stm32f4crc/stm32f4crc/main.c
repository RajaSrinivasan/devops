//
//  main.c
//  stm32f4crc
//
//  Created by Raja Srinivasan on 3/13/26.
//


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "stm32_crc.h"

int main(int argc, char *argv[]) {

        uint32_t words[] = {0x12345678, 0xDEADBEEF};
        uint8_t *bytes = (uint8_t *)(&words[0]) ;
        uint32_t sw_result = stm32_crc32(words, 2);
        for (int i=0; i<8; i++)
        {
            printf("%02x" , bytes[i]);
        }
        printf("\n");
        printf("0x%08X\n", sw_result);  // should print 0xD92B271B
 
    //uint32_t words[] = {0x12345678, 0xDEADBEEF};
    bytes = (uint8_t *)(&words[0]) ;
    sw_result = stm32_crc32(words, 2);
    for (int i=0; i<8; i++)
    {
        printf("%02x" , bytes[i]);
    }
    printf("\n");
    printf("0x%08X\n", sw_result);  // should print 0xD92B271B
    
    uint32_t *wordsp = calloc(sizeof(uint32_t) , 2 );
    wordsp[0] = words[0] ;
    wordsp[1] = words[1] ;
    sw_result = stm32_crc32(wordsp , 2);
    for (int i=0; i<2; i++ )
    {
        printf( "%08x " , wordsp[i]) ;
    }
    printf("\n");
    printf("0x%08X\n", sw_result);
    
    if (argc > 1)
    {
        uint32_t *block ;
        int blocklen = ( strlen(argv[1]) / 2 ) / sizeof(uint32_t) ;
        if (blocklen * sizeof(uint32_t) == strlen(argv[1])/2 )
        {
            
        }
        else
            blocklen++ ;
        block = (uint32_t *)calloc( blocklen , sizeof(uint32_t)) ;
        printf("Block length %d\n" , blocklen );
        for (int i=0; i<blocklen; i++ )
        {
            sscanf( &argv[1][i*8] ,"%8x" ,  &block[i]) ;
            printf( "%08x " , block[i]) ;
        }
        printf("\n");
        uint32_t sw_result=stm32_crc32(block,blocklen);
        printf("0x%08X\n", sw_result);
    }
}

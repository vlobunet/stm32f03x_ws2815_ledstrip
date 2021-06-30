#include "PCB705.h"

uint16_t Brightness[4] = { 0x019E, 0x001E, 0x0198, 0x0018 };

void DMA1_Channel2_3_IRQHandler(void) {
    DMA_ClearITPendingBit(DMA1_IT_TC2);
    DMA_Cmd(DMA1_Channel2, DISABLE);
}

void DMA1_Channel4_5_IRQHandler(void) {
    DMA_ClearITPendingBit(DMA1_IT_TC4);
    DMA_Cmd(DMA1_Channel4, DISABLE);
}			
//-----------------------------------------------------------------------------

void Fill_In_The_DMA_Bufer(uint16_t* USART_TX_Bufer, uint32_t* Input_Colour_Bufer, uint16_t n) {
    uint16_t Pixel_Nomber;

    for ( Pixel_Nomber = 0 ; Pixel_Nomber < n ; Pixel_Nomber++) {
        USART_TX_Bufer[ Pixel_Nomber * 12 + 0 ] = Brightness[  ( Input_Colour_Bufer[ Pixel_Nomber ] & 0x0000C0 ) >> 6  ] ;
        USART_TX_Bufer[ Pixel_Nomber * 12 + 1 ] = Brightness[  ( Input_Colour_Bufer[ Pixel_Nomber ] & 0x000030 ) >> 4  ] ;
        USART_TX_Bufer[ Pixel_Nomber * 12 + 2 ] = Brightness[  ( Input_Colour_Bufer[ Pixel_Nomber ] & 0x00000C ) >> 2  ] ;
        USART_TX_Bufer[ Pixel_Nomber * 12 + 3 ] = Brightness[  ( Input_Colour_Bufer[ Pixel_Nomber ] & 0x000003 ) >> 0  ] ;

        USART_TX_Bufer[ Pixel_Nomber * 12 + 4 ] = Brightness[  ( Input_Colour_Bufer[ Pixel_Nomber ] & 0x00C000 ) >> 14 ] ;
        USART_TX_Bufer[ Pixel_Nomber * 12 + 5 ] = Brightness[  ( Input_Colour_Bufer[ Pixel_Nomber ] & 0x003000 ) >> 12 ] ;
        USART_TX_Bufer[ Pixel_Nomber * 12 + 6 ] = Brightness[  ( Input_Colour_Bufer[ Pixel_Nomber ] & 0x000C00 ) >> 10 ] ;
        USART_TX_Bufer[ Pixel_Nomber * 12 + 7 ] = Brightness[  ( Input_Colour_Bufer[ Pixel_Nomber ] & 0x000300 ) >> 8  ] ;

        USART_TX_Bufer[ Pixel_Nomber * 12 + 8 ] = Brightness[  ( Input_Colour_Bufer[ Pixel_Nomber ] & 0xC00000 ) >> 22 ] ;
        USART_TX_Bufer[ Pixel_Nomber * 12 + 9 ] = Brightness[  ( Input_Colour_Bufer[ Pixel_Nomber ] & 0x300000 ) >> 20 ] ;
        USART_TX_Bufer[ Pixel_Nomber * 12 + 10] = Brightness[  ( Input_Colour_Bufer[ Pixel_Nomber ] & 0x0C0000 ) >> 18 ] ;
        USART_TX_Bufer[ Pixel_Nomber * 12 + 11] = Brightness[  ( Input_Colour_Bufer[ Pixel_Nomber ] & 0x030000 ) >> 16 ] ;
    }
}

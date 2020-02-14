#include "mbed.h"
#include "stm32f413h_discovery.h"
#include "stm32f413h_discovery_ts.h"
#include "stm32f413h_discovery_lcd.h"

TS_StateTypeDef  TS_State = {0};
Serial UART(SERIAL_TX, SERIAL_RX);

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

int main()
{
    UART.baud(115200);
    uint16_t x1, y1;

    BSP_LCD_Init();

    /* Touchscreen initialization */
    if (BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()) == TS_ERROR) {
        printf("BSP_TS_Init error\n");
    }

    /* Clear the LCD */
    BSP_LCD_Clear(LCD_COLOR_WHITE);

    /* Set Touchscreen Demo1 description */
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 40);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0, 15, (uint8_t *)"1        2        3", CENTER_MODE);


    while (1) {
        BSP_TS_GetState(&TS_State);

        if(TS_State.touchDetected){
            /* Get X and Y position of the first touch post calibrated */
            x1 = TS_State.touchX[0];
            y1 = TS_State.touchY[0];
            printf("Touch Detected x=%d y=%d\n", x1, y1);

            if(x1 >= 0 && x1 <= 100){
                led1.write(1);
                thread_sleep_for(1);
                led1.write(0);
                thread_sleep_for(1);
            }

            else if(x1 >= 101 && x1 <= 200){
                led2.write(1);
                thread_sleep_for(1);
                led2.write(0);
                thread_sleep_for(1);
            }

            else {
                led3.write(1);
                thread_sleep_for(1);
                led3.write(0);
                thread_sleep_for(1);
            }

        }

    }

        
    
}
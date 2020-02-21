#include "mbed.h"
#include "stm32f413h_discovery.h"
#include "stm32f413h_discovery_ts.h"
#include "stm32f413h_discovery_lcd.h"
#include "string"

TS_StateTypeDef  TS_State = {0};
DigitalOut led1(LED1);

Timer timer;

int main()
{
    uint16_t x1, y1;
    int Count = 0;
    bool state = 0;

    BSP_LCD_Init();

    /* Touchscreen initialization */
    if (BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()) == TS_ERROR) {
        printf("BSP_TS_Init error\n");
    }

    BSP_LCD_Clear(LCD_COLOR_YELLOW);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0, 20, (uint8_t *)"Press to start", CENTER_MODE);

    int zonesize = BSP_LCD_GetXSize() / 3;

    while (1) {
        if(timer.read() > 5) {
            timer.stop();
            BSP_LCD_Clear(LCD_COLOR_GREEN);
            BSP_LCD_SetTextColor(LCD_COLOR_RED);
            BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
            BSP_LCD_DisplayStringAt(0, 20, (uint8_t *)"YAY YOU GOT", CENTER_MODE);
            string countText = "Count = ";
            countText += to_string(Count);
            BSP_LCD_DisplayStringAt(0, 60,(uint8_t *)&countText, CENTER_MODE);
            break;
        }
        
        BSP_TS_GetState(&TS_State);
        if(state) {
            int timeSec = timer.read_ms() / 1000;
            string timeText = "Time -> ";
            timeText += to_string(timeSec);

            string countText = "Count -> ";
            countText += to_string(Count);

            BSP_LCD_DisplayStringAt(0, 20,(uint8_t *)&timeText, CENTER_MODE);
            BSP_LCD_DisplayStringAt(0, 60,(uint8_t *)&countText, CENTER_MODE);
        }

        if(TS_State.touchDetected) {
            if(!state){
                BSP_LCD_Clear(LCD_COLOR_BLACK);
                state = !state;
                timer.start();
            }
            else {
                Count += 1;
                thread_sleep_for(50);
            }
        } 
    }
}
// // Signal
// #include "mbed.h"

// Thread thread;
// DigitalOut led(LED1);

// void led_thread()
//  {
//     while (true)
//      {
//         // Signal flags that are reported as event are automatically cleared.
//         Thread::signal_wait(0x1);
//         led = !led;
//     }
// }

// int main (void) 
// {
//     thread.start(callback(led_thread));

//     while (true) 
//     {
//         wait(0.25);
//         thread.signal_set(0x1);
//     }
// }

// // thread
// #include "mbed.h"
 
// DigitalOut led1(LED1);
// DigitalOut led2(LED2);
// DigitalOut led3(LED3);
// Thread thread,thread2;
 

// void led3_thread() 
// {
//     while (true) 
//     {
//         led3 = !led3;
//         wait(1.5);
//     }
// }
// void led2_thread() 
// {
//     thread.start(led3_thread);
//     while (true) 
//     {
//         led2 = !led2;
//         wait(1);
//     }
// }
 
// int main()
// {
//     thread.start(led2_thread);
//     thread2.start(led3_thread);
    
//     while (true) 
//     {
//         led1 = !led1;
//         wait(0.5);
//     }
// }

// // Thread_Example_With_CallBack
// #include "mbed.h"

// Thread thread;
// DigitalOut led1(LED1);
// volatile bool running = true;

// // Blink function toggles the led in a long running loop
// void blink(DigitalOut *led) 
// {
//     while (running) 
//     {
//         *led = !*led;
//         wait(1);
//     }
// }

// // Spawns a thread to run blink for 5 seconds
// int main() 
// {
//     thread.start(callback(blink, &led1));
//     wait(5);
//     running = false;
//     thread.join();
// }

// number1

// #include "mbed.h"

// Mutex stdio_mutex;
// Thread t2;
// Thread t3;

// void notify(const char* name, int state) 
// {
//     stdio_mutex.lock();
//     printf("M N 12\n\r", name, state); 
//     stdio_mutex.unlock();
// }

// void test_thread(void const *args) 
// {
//     while (true) 
//     {
//         notify((const char*)args, 0); 
//         wait(1);
//         notify((const char*)args, 1); 
//         wait(1);
//     } 
// }
// int main() 
// {
//     t2.start(callback(test_thread, (void *)"Th 2")); 
//     t3.start(callback(test_thread, (void *)"Th 3"));

// //     test_thread((void *)"Th 1"); 
// // }
 


// #include "mbed.h"

// Semaphore one_slot(4);
// Thread t2;
// Thread t3;

// void test_thread(void const *name) 
// {
//     while (true) 
//     {
//         one_slot.wait();
//         printf("%s\n\r", (const char*)name);
//         wait(1);
//         one_slot.release();
//     }
// }

// int main (void) 
// {
//     t2.start(callback(test_thread, (void *)"Th 2"));
//     t3.start(callback(test_thread, (void *)"Th 3"));

//     test_thread((void *)"Th 1");
// }

// #include "mbed.h"
// #include "stm32f413h_discovery.h"
// #include "stm32f413h_discovery_ts.h"
// #include "stm32f413h_discovery_lcd.h"
 
// DigitalOut led1(LED1);
// DigitalOut led2(LED2);
// DigitalOut led3(LED3);
// Thread thread1, thread2, thread3;

// TS_StateTypeDef  TS_State = {0};
// void led2_thread() 
// {
//     while (true) 
//     {
//         led2 = !led2;
//         wait(0.3);
//     }
// }

// void led3_thread()
// {
//     while(true)
//     {
//         led3 = !led3;
//         wait(0.4);
//     }
// }

// void show_display()
// {
//     BSP_LCD_Init();
//      /* Clear the LCD */
//     BSP_LCD_Clear(LCD_COLOR_WHITE);

//     while(true)
//     {
//         thread_sleep_for(2000);
//         BSP_LCD_Clear(LCD_COLOR_RED);
//         thread_sleep_for(2000);
//         BSP_LCD_Clear(LCD_COLOR_GREEN);
//     }
// }
 
// int main()
// {
//     thread1.start(led2_thread);
//     thread2.start(led3_thread);
//     thread3.start(show_display);
    
//     while (true) 
//     {
//         led1 = !led1;
//         wait(0.2);
//     }
// }


// // number2

#include "mbed.h"
#include <stdlib.h>
#include "stm32f413h_discovery.h"
#include "stm32f413h_discovery_ts.h"
#include "stm32f413h_discovery_lcd.h"
#define PressButton 1

Mutex stdio_mutex;
Thread thread_A, thread_B, thread_C;
DigitalIn  User_Button(PA_0,PullNone);

//DISPLAY
TS_StateTypeDef  TS_State = {0};  

int temp , humidity, light;
char string_temp[5], string_humidity[5], string_light[5];
int counter = 0;

void screen_setup(uint16_t text, uint16_t background){
    BSP_LCD_Clear(background);
    BSP_LCD_SetTextColor(text);
    BSP_LCD_SetBackColor(background);
    BSP_LCD_SetFont(&Font20);
}

void generate_temp(){
    temp = rand() % (100 + 1 - 0) + 0;
    sprintf(string_temp, "%d", temp);
    screen_setup(LCD_COLOR_RED, LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAt(0, 80, (uint8_t *)"Temp : ", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 200, (uint8_t *) string_temp, CENTER_MODE);
}

void generate_humidity(){
    humidity = rand() % (1000 + 1 - 100) + 100;
    sprintf(string_humidity, "%d", humidity);
    screen_setup(LCD_COLOR_RED, LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAt(0, 80, (uint8_t *)"Humidity : ", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 200, (uint8_t *) string_humidity, CENTER_MODE);
}

void generate_light(){
    light = rand() % (10000 + 1 - 1000) + 1000;
    sprintf(string_light, "%d", light);
    screen_setup(LCD_COLOR_RED, LCD_COLOR_GREENN);
    BSP_LCD_DisplayStringAt(0, 80, (uint8_t *)"Light : ", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 200, (uint8_t *) string_light, CENTER_MODE);

}

void notify(const char* args) 
{
    stdio_mutex.lock();
    wait(1);

    if(User_Button.read() == PressButton && counter == 0){
        generate_temp();
        printf("test_1\n");
        counter += 1;

    }
    else if(User_Button.read() == PressButton && counter == 1){
        generate_humidity();
        printf("test_2\n");
        counter += 1;
    }

    else if(User_Button.read() == PressButton && counter == 2){
        generate_light();
        printf("test_3\n");
        counter -= 2;
    }

    stdio_mutex.unlock();
    thread_sleep_for(750);
}

void test_thread(void const *args)
{
    while (true) 
    {
        printf("test\n");
        notify((const char*)args); 
        thread_sleep_for(750);
    }
}

int main() 
{
    BSP_LCD_Init();
    BSP_LCD_Clear(LCD_COLOR_YELLOW);

    thread_A.start(callback(test_thread, (void *)"temperature"));
    thread_B.start(callback(test_thread, (void *)"humidity"));
    thread_C.start(callback(test_thread, (void *)"lightIntensity"));  
}
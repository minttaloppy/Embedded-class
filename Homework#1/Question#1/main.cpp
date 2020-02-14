#include "mbed.h"

#define PressButton 1

DigitalOut led1(LED1);

DigitalIn  User_Button(PA_0,PullNone);
Serial UART(SERIAL_TX, SERIAL_RX);


int main()
{
    bool status = false;
    int count = 0;
    UART.baud(115200);
    while (true)
    {
        if(User_Button.read() == PressButton && status == false) // pressed the button
        {
            count += 1;
            thread_sleep_for(3000); //delay 3 sec
            if(count >= 3){
                led1.write(1);
                status = true;
            }

            thread_sleep_for(1000);
        }

        else if(User_Button.read() == PressButton && status == true)
        {
            led1.write(0);
            status = false;
            thread_sleep_for(1000);
        }
    
            
    }
}
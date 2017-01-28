#include <mbed.h>
#include <rtos.h>
#include <USBSerial.h>

#include "Pin_Defines.h"
#include "analog_read_thread.h"
#include "error_checking_thread.h"
#include "data_logging_thread.h"

USBSerial serial;

DigitalOut status_led(STATUS_LED);

int main() {
    Thread::wait(1000);
    
    Thread analog_read_t(analog_read_thread,NULL,osPriorityNormal,256*4);
    Thread error_checking_t(error_checking_thread,NULL,osPriorityNormal,256*4);
    Thread data_logging_t(data_logging_thread,NULL,osPriorityNormal,256*4);

    while (true) {
        status_led = !status_led;
        serial.printf("Hello World!\n");
        serial.printf("CAP Voltage = %f\n",get_capvolt());
        Thread::wait(200);
    }
}

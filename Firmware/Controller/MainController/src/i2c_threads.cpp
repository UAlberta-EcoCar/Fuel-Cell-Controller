#include "mbed.h"
#include "rtos.h"

#include "i2c_threads.h"
#include "Adafruit_SHT31.h"
#include "ThermistorLookup.h"

I2C i2c(I2C_SDA,I2C_SCL);

char RunCommand[] = {0x04,0x00};
char motor_data[6];
int motor_current;
int battery_volts;
void motor_command_thread(void const *args)
{
  while(true)
  {
    RunCommand[0] = 0x04;
    i2c.write(MOTOR_I2C_ADDRESS<<1,RunCommand,2);
    //i2c.read(MOTOR_I2C_ADDRESS<<1,motor_data,6);
    // motor_current = motor_data[0] | (motor_data[1]<<8);
    // battery_volts = motor_data[4] | (motor_data[5]<<8);

    Thread::wait(1000);

    RunCommand[0] = 0x05;
    i2c.write(MOTOR_I2C_ADDRESS<<1,RunCommand,2);
    Thread::wait(1000);
  }
}
int get_motor_current(void)
{
  return(motor_current);
}
int get_batery_volts(void)
{
  return(battery_volts);
}

char indicator_leds_reg[2];
void set_indicator_leds_thread(void const *args)
{
  while(true)
  {
    i2c.write(LED_INDICATOR_ADDRESS<<1,indicator_leds_reg,2);
    Thread::wait(300);
  }
}
int get_indicator_leds(void)
{
  return((int)indicator_leds_reg[0]|((int)indicator_leds_reg[1]<<8));
}
void set_indicator_leds(int val)
{
  indicator_leds_reg[0] = (char)val;
  indicator_leds_reg[1] = (char)(val >> 8);
}

char fan_pwr_status[2]; //needs to be array. second char will be ignored
char fctemp[3];
void fan_control_board_thread(void const *args)
{
  while(true)
  {
    i2c.write(FAN_CONTROL_BOARD_ADDRESS<<1,fan_pwr_status,1);
    Thread::wait(50);
    i2c.read(FAN_CONTROL_BOARD_ADDRESS<<1,fctemp,3);
    Thread::wait(150);
  }
}
void set_fan_pwr_status(char val)
{
  fan_pwr_status[0] = val;
}
float get_fctemp(void)
{
  return(therm_lookup(((float)(fctemp[0]*4)+(float)fctemp[1])/1023.0f));
}
char get_fan_speed(void)
{
  return(fctemp[2]);
}
char get_temp_raw(void)
{
  return(fctemp[0]*4+fctemp[1]);
}
bool get_fan_status(void)
{
  if(fctemp[3])
  {
    return(1);
  }
  else
  {
    return(0);
  }
}

float temp;
float humidity;
void sht31_writeCommand(uint16_t cmd);
void sht31_readtemphum_thread(void const *args)
{

  sht31_writeCommand(SHT31_SOFTRESET);

  Thread::wait(10);

  while(true)
  {

    char readbuffer[6];

    sht31_writeCommand(SHT31_MEAS_HIGHREP);

    Thread::wait(300);

    for(uint8_t i = 0;i<6;i++)
    {
      readbuffer[i] = 0;
    }


    i2c.read(SHT31_DEFAULT_ADDR<<1,readbuffer,6); //i2c returns a signed array

    uint16_t ST, SRH;
    ST = readbuffer[0];
    ST <<= 8;
    ST |= readbuffer[1];

    //  if (readbuffer[2] != crc8(readbuffer, 2)) return false;

    SRH = readbuffer[3];
    SRH <<= 8;
    SRH |= readbuffer[4];

    //  if (readbuffer[5] != crc8(readbuffer+3, 2)) return false;

    // Serial.print("ST = "); Serial.println(ST);
    double stemp = ST;
    stemp *= 175;
    stemp /= 0xffff;
    stemp = -45 + stemp;
    temp = stemp;

    //  Serial.print("SRH = "); Serial.println(SRH);
    double shum = SRH;
    shum *= 100;
    shum /= 0xFFFF;

    humidity = shum;

  }
}

void sht31_writeCommand(uint16_t cmd)
{
  char send_data[2];
  send_data[0] = cmd >> 8;
  send_data[1] = cmd & 0xFF;
  i2c.write(SHT31_DEFAULT_ADDR<<1,send_data,2);
}

float sht31_readTemperature(void)
{
  return temp;
}

float sht31_readHumidity(void)
{
  return humidity;
}
/*********************************************************************/


// ds3231 rtc_data
char now[13]; //string containing ssmmhhddmmyy
char ds3231_buffer[19];
void ds3231_thread(void const *args)
{
  //reads data from rtc
  while(1)
  {
    i2c.write(DS3231_ADDRESS<<1,0,1);
    Thread::wait(5);

    i2c.read(DS3231_ADDRESS<<1,ds3231_buffer,19);

    //write to string
    //seconds
    now[1] = (ds3231_buffer[0]&0x0f) + 48;
    now[0] = ((ds3231_buffer[0]&0xf0)>>4) + 48;
    //minutes
    now[3] = (ds3231_buffer[1]&0x0f) + 48;
    now[2] = ((ds3231_buffer[1]&0xf0)>>4) + 48;
    //hours
    now[5] = (ds3231_buffer[2]&0x0f) + 48;
    now[4] = ((ds3231_buffer[2]&0xf0)>>4) + 48;
    //day
    now[7] = (ds3231_buffer[4]&0x0f) + 48;
    now[6] = ((ds3231_buffer[4]&0xf0)>>4) + 48;
    //month
    now[9] = (ds3231_buffer[5]&0x0f) + 48;
    now[8] = ((ds3231_buffer[5]&0xf0)>>4) + 48;
    //year
    now[11] = (ds3231_buffer[6]&0x0f) + 48;
    now[10] = ((ds3231_buffer[6]&0xf0)>>4) + 48;

    now[12] = 0; //terminated with null char
    Thread::wait(300);
  }
}
char * get_time(void)
{
  return(now);
}

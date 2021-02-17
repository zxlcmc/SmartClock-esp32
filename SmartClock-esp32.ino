// ESP32 by zhuxiaoli.

#include "DFRobot_DS3231M.h"
#include <WiFi.h>
#include "time.h"
#include <U8g2lib.h>

DFRobot_DS3231M rtc;
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

const char* ssid       = "wifi_zxl";//wlan information
const char* password   = "wifipassword";

const char* ntpServer = "ntp.ntsc.ac.cn";//local ntp server
const long  gmtOffset_sec = 8*3600;
const int   daylightOffset_sec = 0;


void getTimeFromNTP()
{
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return;
    }
    
    rtc.setYear(timeinfo.tm_year - 100);
    rtc.setMonth(timeinfo.tm_mon + 1);
    rtc.setDate(timeinfo.tm_mday);
    rtc.setHour(timeinfo.tm_hour, e24hours);
    rtc.setMinute(timeinfo.tm_min);
    rtc.setSecond(timeinfo.tm_sec + 1);
    rtc.adjust();
    
}

void page1() {
  u8g2.setFont(u8g2_font_timB18_tf);
  u8g2.setFontPosTop();
  u8g2.setCursor(10,10);
  u8g2.print(String(rtc.year()) + String("-") + String(rtc.month()/10%10) + String(rtc.month()%10) + String("-") + String(rtc.day()/10%10)+ String(rtc.day()%10));
  u8g2.setCursor(22,40);
  u8g2.print(String(rtc.hour()/10%10) + String(rtc.hour()%10) + String(":") + String(rtc.minute()/10%10) + String(rtc.minute()%10) + String(":") + String(rtc.second()/10%10) + String(rtc.second()%10));
}

void setup()
{
    Serial.begin(9600);
    while(rtc.begin() != true){
        Serial.println("Failed to init chip, please check if the chip connection is fine. ");
        delay(1000);
    }
   
    u8g2.begin();
    u8g2.enableUTF8Print();
       
    //connect to WiFi
    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" CONNECTED");
    
    //init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    getTimeFromNTP();
    //disconnect WiFi as it's no longer needed
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

void loop()
{
    rtc.getNowTime();
    /*Serial.print(rtc.year(), DEC);//year
    Serial.print('/');
    Serial.print(rtc.month(), DEC);//month
    Serial.print('/');
    Serial.print(rtc.day(), DEC);//date
    Serial.print(" (");
    Serial.print(rtc.getDayOfTheWeek());//day of week
    Serial.print(") ");
    Serial.print(rtc.hour(), DEC);//hour
    Serial.print(':');
    Serial.print(rtc.minute(), DEC);//minute
    Serial.print(':');
    Serial.print(rtc.second(), DEC);//second
    Serial.println();
    delay(1000); */

    u8g2.firstPage();
    do
    {
      page1();
    }while(u8g2.nextPage());
    
}

#include <U8g2lib.h>
#include <EasyButton.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PID_v1_bc.h>


#define ENCODER_PIN1 A0
#define ENCODER_PIN2 A1
#define BUTTON_PIN A2
#define RELAY_PIN A3
#define ONE_WIRE_BUS 10
#define RELAY_ACTIVE HIGH
#define INITIAL_SETPOINT 40
#define TEMPERATURE_PRECISION 9

double Setpoint, Input, Output, duty;

double Kp=100, Ki=0, Kd=0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

int period = 5000;
unsigned long periodStartTime;

//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE); 
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

EasyButton button(BUTTON_PIN);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempProbe(&oneWire);

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

bool button_single = true;
bool last_button_single = true;
bool button_double = false;

void updateSetpoint(){
  digitalWrite(RELAY_PIN, !RELAY_ACTIVE);
  encoderValue = Setpoint * 4;
  while (button_double){
    u8g2.setFont(u8g2_font_helvB12_tf); 
    u8g2.setFontDirection(0);
    u8g2.clearBuffer();
    u8g2.setCursor(0, 15);
    u8g2.setFont(u8g2_font_helvB08_tf); 
    u8g2.print("current setpoint:");
    u8g2.setFont(u8g2_font_helvB12_tf); 
    u8g2.setCursor(0, 35);
    u8g2.print(Setpoint);
    u8g2.print("°C");
    u8g2.setCursor(0, 55);
    u8g2.print("new: ");
    u8g2.print(encoderValue/4);
    u8g2.print("°C");
    u8g2.sendBuffer();
  }
  Setpoint = encoderValue/4;
  button_single = false;
}

void updateConstDisp(){
  u8g2.setFont(u8g2_font_helvB12_tf); 
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print("Temp: ");
  u8g2.print(Input);
  u8g2.print("°C");
  u8g2.setCursor(0, 30);
  u8g2.setFont(u8g2_font_helvB10_tf); 
  u8g2.print("Duty cyle: ");
  u8g2.setFont(u8g2_font_helvB24_tf); 
  u8g2.setCursor(45, 64);
  u8g2.print((duty * 100.0) / period, 0);
  u8g2.println("%");
  u8g2.sendBuffer();
}

void updatePIDDisp(){
  u8g2.setFont(u8g2_font_helvB12_tf); 
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print("Temp: ");
  u8g2.print(Input);
  u8g2.print("°C");
  u8g2.setCursor(0, 35);
  u8g2.setFont(u8g2_font_helvB08_tf); 
  u8g2.print("Setpoint: ");
  u8g2.setFont(u8g2_font_helvB12_tf); 
  u8g2.print(Setpoint);
  u8g2.print("°C");
  u8g2.setCursor(0, 55);
  u8g2.print("Output: ");
  u8g2.print((Output * 100.0) / period, 0);
  u8g2.println("%");
  u8g2.sendBuffer();
}

void updateEncoder(){
  int MSB = digitalRead(ENCODER_PIN1);
  int LSB = digitalRead(ENCODER_PIN2);

  int encoded = (MSB << 1) |LSB;
  int sum  = (lastEncoded << 2) | encoded;

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  lastEncoded = encoded;
}

void singleClick()
{
  button_single = !button_single;
  if (!last_button_single) last_button_single = true;
}

void doubleClick()
{
  button_double = !button_double;
}

void buttonInterrupt()
{
  button.read();
}

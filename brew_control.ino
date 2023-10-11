#include "brew_control.h"

void setup(void) {
  u8g2.begin();
  u8g2.enableUTF8Print();

  pinMode(ENCODER_PIN1, INPUT_PULLUP); 
  pinMode(ENCODER_PIN2, INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN1), updateEncoder, CHANGE); 
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN2), updateEncoder, CHANGE);
  
  button.begin();
  button.onPressed(singleClick);
  button.onSequence(2, 500, doubleClick);
  button.enableInterrupt(buttonInterrupt);

  button2.begin();
  button2.onPressed(pumpClick);
  // button2.onSequence(2, 500, doubleClick);
  button2.enableInterrupt(button2Interrupt);

  tempProbe.begin();
  tempProbe.setResolution(0, TEMPERATURE_PRECISION);

  periodStartTime = millis();

  Setpoint = INITIAL_SETPOINT;
  myPID.SetOutputLimits(0, period);
  myPID.SetMode(AUTOMATIC);
}

void loop(void) {
  tempProbe.requestTemperatures();
  Input = tempProbe.getTempCByIndex(0);
  if (button_double) updateSetpoint();
  if (button_single){
    if (last_button_single){
      duty = 0;
      encoderValue = 0;
      last_button_single = false;
    }
    
    duty = encoderValue / 80.0 * period;
    if (duty < 0) {
      duty = 0.0;
      encoderValue = 0;
    }
    if (duty > period) {
      duty = period;
      encoderValue = 80;
    }
    updateConstDisp();
  }
  else{
  myPID.Compute();
  updatePIDDisp();
  duty = Output;
  }
 
  if (millis() - periodStartTime > period) periodStartTime += period;
  if (duty < millis() - periodStartTime) digitalWrite(RELAY_PIN, !RELAY_ACTIVE);
  else digitalWrite(RELAY_PIN, RELAY_ACTIVE);
  
}

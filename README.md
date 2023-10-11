# brew_control
This is intended as a brewpot controller with either constant duty cycle or PID temperature controll.

<img src=https://raw.githubusercontent.com/ssk8/project_pics/main/brew_control.jpeg  width="300"/> 

Main parts are as follows:
- oled screen
- rotary encoder with button
- momentary button and relay for pump
- DS18B20 temperature probe
- solid state relay between 220V mains and heating element
- microcontroler (QT-PY m0)

Operation overview:
Device should start in 0% duty configuration, rotate encoder to change duty. Click the encoder button to enter the PID temperature controller and toggle back to 0% duty cycle. Double click encoder button to change temperature setpoint.
Click on, click of pump with indicator (P) on OLED.

NOTE: As of 10/2023, onewire library was not compatible with Rp2040 in arduino!

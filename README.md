# brew_control
This is intended as a brewpot controller with either constant duty cycle or PID temperature controll.

Main parts are as follows:
- oled screen
- rotary encoder
- DS18B20 temperature probe
- solid state relay between 220V mains and heating element
- microcontroler (tested with QT PY)

Operation overview:
Device should start in 0% duty configuration, rotate encoder to change duty. Click the encoder button to enter the PID temperature controller and toggle back to 0% duty cycle. Double click encoder button to change temperature setpoint.

NOTE: onewire is not working on Rpi Pico/arduino!

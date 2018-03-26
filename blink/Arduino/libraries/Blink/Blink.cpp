#include "Arduino.h"
#include "Blink.h"

Blink::Blink(int pin){
  pinMode(pin,OUTPUT);
  _pin = pin;
  // blink class 내부에 있는 variable _pin 값을 parameter 값으로 바꿔준다.
}
void Blink::on(int _delay){
  digitalWrite(_pin,HIGH);
  delay(_delay);
  // _pin에 해당하는 부분을 켜준다. 그리고 _delay 만큼 정지한다.
}
void Blink::off(int _delay){
  digitalWrite(_pin, LOW);
  delay(_delay);
  // _pin에 해당하는 부분을 끈다. 그리고 _delay 만큼 정지한다.
}




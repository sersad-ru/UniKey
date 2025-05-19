/*
* Command interface.
* Работает с UART-ом
*/
#pragma once
#include <Arduino.h>

class CMD {
  public:
    CMD(Serial_ &ser);
  private:
    Serial_ _ser;
};//class
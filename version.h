/*
 * Константы версии 
*/

#pragma once
#include <Arduino.h>

#define FF(val) ((const __FlashStringHelper*)val)

const char APP_VER_VAL[] PROGMEM = "v1.0"; // Базовая версия
#define APP_VER FF(APP_VER_VAL)

const char APP_CPY_VAL[] PROGMEM = " (c)2025"; // Годы копирайта
#define APP_COPYRIGHT FF(APP_CPY_VAL)

const char APP_NAME_VAL[] PROGMEM = "UniKey "; // Название приложения
#define APP_NAME FF(APP_NAME_VAL)

const char APP_SERSAD_VAL[] PROGMEM = " by sersad.ru"; // Копирайт
#define APP_SERSAD FF(APP_SERSAD_VAL)

const char APP_BUILD_ID_VAL[] PROGMEM = "BuildID: "; // Идентификатор сборки
#define APP_BUILD_ID FF(APP_BUILD_ID_VAL)

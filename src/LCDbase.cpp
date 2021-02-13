//
// Created by filip on 12.02.2021.
//

#include "LCDbase.h"

LCDbase::LCDbase(const uint &pinReadWrite, const uint &pinCommandData, const uint &pinEnable, const uint *pinData) :
        PIN_READ_WRITE(pinReadWrite), PIN_COMMAND_DATA(pinCommandData), PIN_ENABLE(pinEnable) {
    rwPinConnected = true;
    PIN_DATA[0] = pinData[0];
    PIN_DATA[1] = pinData[1];
    PIN_DATA[2] = pinData[2];
    PIN_DATA[3] = pinData[3];

#if USE_4_BIT_DATA_BUS != 1
    PIN_DATA[4] = pinData[4];
    PIN_DATA[5] = pinData[5];
    PIN_DATA[6] = pinData[6];
    PIN_DATA[7] = pinData[7];
#endif
}

LCDbase::LCDbase(const uint &pinCommandData, const uint &pinEnable, const uint *pinData) :
        PIN_COMMAND_DATA(pinCommandData), PIN_ENABLE(pinEnable) {
    rwPinConnected = false;
    PIN_DATA[0] = pinData[0];
    PIN_DATA[1] = pinData[1];
    PIN_DATA[2] = pinData[2];
    PIN_DATA[3] = pinData[3];

#if USE_4_BIT_DATA_BUS != 1
    PIN_DATA[4] = pinData[4];
    PIN_DATA[5] = pinData[5];
    PIN_DATA[6] = pinData[6];
    PIN_DATA[7] = pinData[7];
#endif
}



#if USE_4_BIT_DATA_BUS == 1

LCDbase::LCDbase(const uint &pinReadWrite, const uint &pinComandData, const uint &pinEnable, const uint &pinData0,
                 const uint &pinData1, const uint &pinData2, const uint &pinData3) :
        PIN_READ_WRITE(pinReadWrite), PIN_COMMAND_DATA(pinComandData), PIN_ENABLE(pinEnable) {
    rwPinConnected = true;
    PIN_DATA[0] = pinData0;
    PIN_DATA[1] = pinData1;
    PIN_DATA[2] = pinData2;
    PIN_DATA[3] = pinData3;
}

LCDbase::LCDbase(const uint &pinCommandData, const uint &pinEnable, const uint &pinData0, const uint &pinData1,
                 const uint &pinData2, const uint &pinData3) :
        PIN_COMMAND_DATA(pinCommandData), PIN_ENABLE(pinEnable) {
    rwPinConnected = false;
    PIN_DATA[0] = pinData0;
    PIN_DATA[1] = pinData1;
    PIN_DATA[2] = pinData2;
    PIN_DATA[3] = pinData3;
}

inline void LCDbase::putToGpio(uint8_t value, bool commandData, bool readWrite) {
    gpio_put(PIN_COMMAND_DATA, commandData);
    if (rwPinConnected) gpio_put(PIN_READ_WRITE, readWrite);

    putToGpio4Pin(value >> 4);
    putToGpio4Pin(value);
}

#else

LCDbase::LCDbase(const uint &pinReadWrite, const uint &pinComandData, const uint &pinEnable, const uint &pinData0,
                 const uint &pinData1, const uint &pinData2, const uint &pinData3, const uint &pinData4, const uint &pinData5,
                 const uint &pinData6, const uint &pinData7) :
        PIN_READ_WRITE(pinReadWrite), PIN_COMMAND_DATA(pinComandData), PIN_ENABLE(pinEnable) {
    rwPinConnected = true;
    PIN_DATA[0] = pinData0;
    PIN_DATA[1] = pinData1;
    PIN_DATA[2] = pinData2;
    PIN_DATA[3] = pinData3;
    PIN_DATA[4] = pinData4;
    PIN_DATA[5] = pinData5;
    PIN_DATA[6] = pinData6;
    PIN_DATA[7] = pinData7;
}

LCDbase::LCDbase(const uint &pinCommandData, const uint &pinEnable, const uint &pinData0, const uint &pinData1,
                 const uint &pinData2, const uint &pinData3, const uint &pinData4, const uint &pinData5,
                 const uint &pinData6, const uint &pinData7) :
        PIN_COMMAND_DATA(pinCommandData), PIN_ENABLE(pinEnable) {
    rwPinConnected = false;
    PIN_DATA[0] = pinData0;
    PIN_DATA[1] = pinData1;
    PIN_DATA[2] = pinData2;
    PIN_DATA[3] = pinData3;
    PIN_DATA[4] = pinData4;
    PIN_DATA[5] = pinData5;
    PIN_DATA[6] = pinData6;
    PIN_DATA[7] = pinData7;
}


inline void LCDbase::putToGpio(uint8_t value, bool commandData, bool readWrite) {
    gpio_put(PIN_COMMAND_DATA, commandData);
    if(rwPinConnected) gpio_put(PIN_READ_WRITE, readWrite);

    gpio_put(PIN_DATA[0], value & 1);
    gpio_put(PIN_DATA[1], (value >> 1) & 1);
    gpio_put(PIN_DATA[2], (value >> 2) & 1);
    gpio_put(PIN_DATA[3], (value >> 3) & 1);
    gpio_put(PIN_DATA[4], (value >> 4) & 1);
    gpio_put(PIN_DATA[5], (value >> 5) & 1);
    gpio_put(PIN_DATA[6], (value >> 6) & 1);
    gpio_put(PIN_DATA[7], (value >> 7) & 1);
    enablePulse();
}
#endif

inline void LCDbase::putToGpio4Pin(uint8_t value) {
    gpio_put(PIN_DATA[0], (value) & 1);
    gpio_put(PIN_DATA[1], (value >> 1) & 1);
    gpio_put(PIN_DATA[2], (value >> 2) & 1);
    gpio_put(PIN_DATA[3], (value >> 3) & 1);
    enablePulse();
}


inline void LCDbase::enablePulse() const {
    gpio_put(PIN_ENABLE, false);
    sleep_us(1);
    gpio_put(PIN_ENABLE, true);
    sleep_us(1);
    gpio_put(PIN_ENABLE, false);
    sleep_us(60);
}

void LCDbase::clearScreen() {
    putToGpio(LCD_CMD_CLEAR_SCREEN, false);
    sleep_ms(2);
}

void LCDbase::cursorMoveToBasePoint() {
    putToGpio(LCD_CMD_CURSOR_RETURN, false);
    sleep_ms(2);
}

void LCDbase::autoShiftOnNewChar(LCDbase::State state, LCDbase::Direction direction) {
    putToGpio(LCD_CMD_CURSOR_PROPERTIES |
              (state == ON ? LCD_FLG_CURSOR_PROPERTIES_SHIFT : LCD_FLG_CURSOR_PROPERTIES_NOT_SHIFT)
              | (direction == RIGHT ? LCD_FLG_CURSOR_PROPERTIES_INCREMENT : LCD_FLG_CURSOR_PROPERTIES_DECREMENT),
              false);
}

void LCDbase::display(LCDbase::State state) {
    displayControlState = (state == ON ? displayControlState | LCD_FLG_DISPLAY_CONTROL_DISPLAY_ON :
                           displayControlState & LCD_FLG_DISPLAY_CONTROL_DISPLAY_OFF);
    putToGpio(LCD_CMD_DISPLAY_CONTROL | displayControlState, false);
}

void LCDbase::cursor(LCDbase::State state) {
    displayControlState = (state == ON ? displayControlState | LCD_FLG_DISPLAY_CONTROL_CURSOR_ON :
                           displayControlState & LCD_FLG_DISPLAY_CONTROL_CURSOR_OFF);
    putToGpio(LCD_CMD_DISPLAY_CONTROL | displayControlState, false);
}

void LCDbase::blink(LCDbase::State state) {
    displayControlState = (state == ON ? displayControlState | LCD_FLG_DISPLAY_CONTROL_BLINK_ON :
                           displayControlState & LCD_FLG_DISPLAY_CONTROL_BLINK_OFF);
    putToGpio(LCD_CMD_DISPLAY_CONTROL | displayControlState, false);
}

void LCDbase::cursorMove(LCDbase::Direction direction) {
    putToGpio(LCD_CMD_CURSOR_SHIFT | LCD_FLG_CURSOR_SHIFT_CURSOR_SHIFT |
              (direction == RIGHT ? LCD_FLG_CURSOR_SHIFT_RIGHT : LCD_FLG_CURSOR_SHIFT_LEFT), false);
}

void LCDbase::displayContentMove(LCDbase::Direction direction) {
    putToGpio(LCD_CMD_CURSOR_SHIFT | LCD_FLG_CURSOR_SHIFT_DISPLAY_SHIFT |
              (direction == RIGHT ? LCD_FLG_CURSOR_SHIFT_RIGHT : LCD_FLG_CURSOR_SHIFT_LEFT), false);
}

void LCDbase::initialize(unsigned short lineSize, unsigned short amountLines, LCDbase::Font font) {
    this->lineSize = lineSize;
    this->amountLines = amountLines;

    sleep_ms(90);
#if USE_4_BIT_DATA_BUS == 1
    gpio_put(PIN_COMMAND_DATA, false);
    if (rwPinConnected) gpio_put(PIN_READ_WRITE, false);
    putToGpio4Pin(0x3);
    putToGpio(0x30, false);

    sleep_ms(5);
    putToGpio4Pin(0x3);

    sleep_ms(5);
    putToGpio4Pin(0x3);

    sleep_us(200);
    putToGpio4Pin(0x2);

#else
    putToGpio(0x30, false);

    sleep_ms(5);
    putToGpio(0x30, false);

    sleep_us(200);
    putToGpio(0x30, false);

#endif


    putToGpio(
            LCD_CMD_FUNCTION_SET | (lineSize == 1 ? LCD_FLG_CMD_FUNCTION_SET_1_LINE : LCD_FLG_CMD_FUNCTION_SET_2_LINE) |
            (font == STYLE_5x7 ? LCD_FLG_CMD_FUNCTION_SET_FONT_5x7 : LCD_FLG_CMD_FUNCTION_SET_FONT_5x10), false);

    display();
    clearScreen();
    autoShiftOnNewChar(OFF);

    sleep_ms(10);

}

void LCDbase::cursorMoveTo(uint8_t address) {
    putToGpio(LCD_CMD_SET_DDRAM | address, false);
}

void LCDbase::defineChar(unsigned short location, uint8_t *dots) {
    if (location > 8) location = location % 8;
    putToGpio(LCD_CMD_SET_CGRAM | (location << 3), false);

    for (int i = 0; i < 8; ++i) {
        writeChar(dots[i]);
    }

    cursorMoveToBasePoint();
}

void LCDbase::writeChar(uint16_t character) {
    putToGpio(character, true);

}

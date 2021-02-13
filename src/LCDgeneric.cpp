//
// Created by filip on 12.02.2021.
//

#include "LCDgeneric.h"

LCDgeneric::LCDgeneric(const uint &pinReadWrite, const uint &pinCommandData, const uint &pinEnable, const uint *pinData)
        : LCDbase(pinReadWrite, pinCommandData, pinEnable, pinData) {}

LCDgeneric::LCDgeneric(const uint &pinCommandData, const uint &pinEnable, const uint *pinData) : LCDbase(pinCommandData,
                                                                                                         pinEnable,
                                                                                                         pinData) {}

#if USE_4_BIT_DATA_BUS == 1

LCDgeneric::LCDgeneric(const uint &pinReadWrite, const uint &pinComandData, const uint &pinEnable, const uint &pinData0,
                       const uint &pinData1, const uint &pinData2, const uint &pinData3) : LCDbase(pinReadWrite,
                                                                                                   pinComandData,
                                                                                                   pinEnable, pinData0,
                                                                                                   pinData1, pinData2,
                                                                                                   pinData3) {}

LCDgeneric::LCDgeneric(const uint &pinCommandData, const uint &pinEnable, const uint &pinData0, const uint &pinData1,
                       const uint &pinData2, const uint &pinData3) : LCDbase(pinCommandData, pinEnable, pinData0,
                                                                             pinData1, pinData2, pinData3) {}

#else

LCDgeneric::LCDgeneric(const uint &pinReadWrite, const uint &pinComandData, const uint &pinEnable, const uint &pinData0,
                 const uint &pinData1, const uint &pinData2, const uint &pinData3, const uint &pinData4, const uint &pinData5,
                 const uint &pinData6, const uint &pinData7) :
                 LCDbase(pinReadWrite, pinComandData, pinEnable, pinData0,pinData1, pinData2, pinData3, pinData4,
                         pinData5, pinData6, pinData7) {}

LCDgeneric::LCDgeneric(const uint &pinCommandData, const uint &pinEnable, const uint &pinData0, const uint &pinData1,
                 const uint &pinData2, const uint &pinData3, const uint &pinData4, const uint &pinData5,
                 const uint &pinData6, const uint &pinData7) :
        LCDbase(pinComandData, pinEnable, pinData0,pinData1, pinData2, pinData3, pinData4, pinData5, pinData6,
                pinData7) {}
#endif

#if LCD_GENERIC_USE_STRING == 1

void LCDgeneric::writeLine(unsigned short lineNum, std::string text) {
    cursorMoveTo(getFirstAddressOfLine(lineNum));

    for (int i = 0; i < lineSize; ++i) {
        if (text.size() > i)
            writeChar(text[i]);
        else writeChar(' ');
    }
}
#else

void LCDgeneric::writeLine(unsigned short lineNum, char *text) {
    cursorMoveTo(getFirstAddressOfLine(lineNum));

    int i = 0;
    for (; i < lineSize; ++i) {
        if (text[i] != '\0') {
            writeChar(text[i]);
            break;
        }
    }

    for (; i < lineSize; ++i) {
        writeChar(' ');
    }
}

#endif

uint LCDgeneric::getFirstAddressOfLine(unsigned short lineNumber) {
    if (amountLines > 1) {
        if (lineNumber % 2 == 1) {
            return 0x40 | ((lineNumber / (amountLines / 2)) * lineSize);
        } else {
            return ((lineNumber / (amountLines / 2)) * lineSize);
        }
    } else
        return 0;
}

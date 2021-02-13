//
// Created by filip on 12.02.2021.
//

#ifndef BUTTON_FIRST_LCDGENERIC_H
#define BUTTON_FIRST_LCDGENERIC_H

#ifndef LCD_GENERIC_USE_STRING
#define LCD_GENERIC_USE_STRING 0
#endif

#if LCD_GENERIC_USE_STRING == 1
#include <string>
#endif

#include "LCDbase.h"


class LCDgeneric: public LCDbase {
public:
    LCDgeneric(const uint &pinCommandData, const uint &pinEnable, const uint *pinData);

    LCDgeneric(const uint &pinReadWrite, const uint &pinCommandData, const uint &pinEnable, const uint *pinData);

#if USE_4_BIT_DATA_BUS == 1

    LCDgeneric(const uint &pinReadWrite, const uint &pinComandData, const uint &pinEnable, const uint &pinData0,
               const uint &pinData1, const uint &pinData2, const uint &pinData3);

    LCDgeneric(const uint &pinCommandData, const uint &pinEnable, const uint &pinData0, const uint &pinData1,
               const uint &pinData2, const uint &pinData3);

#else
    LCDgeneric(const uint &pinReadWrite, const uint &pinComandData, const uint &pinEnable, const uint &pinData0,
            const uint &pinData1, const uint &pinData2, const uint &pinData3, const uint &pinData4, const uint &pinData5,
            const uint &pinData6, const uint &pinData7);

    LCDgeneric(const uint &pinCommandData, const uint &pinEnable, const uint &pinData0, const uint &pinData1,
            const uint &pinData2, const uint &pinData3, const uint &pinData4, const uint &pinData5, const uint &pinData6,
            const uint &pinData7);
#endif

#if LCD_GENERIC_USE_STRING == 1
    void writeLine(unsigned short lineNum, std::string text);
#else
    void writeLine(unsigned short lineNum, char text[]);
#endif

private:
    uint getFirstAddressOfLine(unsigned short lineNumber);
};


#endif //BUTTON_FIRST_LCDGENERIC_H

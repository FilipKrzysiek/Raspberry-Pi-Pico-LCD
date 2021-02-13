//
// Created by filip on 12.02.2021.
//

#ifndef BUTTON_FIRST_LCDGENERIC_H
#define BUTTON_FIRST_LCDGENERIC_H

#include <string>
#include "LCDbase.h"


class LCDgeneric: public LCDbase {
public:
    LCDgeneric(const uint &pinCommandData, const uint &pinEnable, const uint *pinData);

    LCDgeneric(const uint &pinReadWrite, const uint &pinCommandData, const uint &pinEnable, const uint *pinData);

    LCDgeneric(const uint &pinReadWrite, const uint &pinComandData, const uint &pinEnable, const uint &pinData0,
               const uint &pinData1, const uint &pinData2, const uint &pinData3);

    LCDgeneric(const uint &pinCommandData, const uint &pinEnable, const uint &pinData0, const uint &pinData1,
               const uint &pinData2, const uint &pinData3);

    void writeLine(unsigned short lineNum, std::string text);

private:
    uint getFirstAddressOfLine(unsigned short lineNumber);
};


#endif //BUTTON_FIRST_LCDGENERIC_H

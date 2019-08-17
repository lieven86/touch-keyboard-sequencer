/*
Copyright (c) 2015 4ms Company

Author: Dan Green - danngreen1@gmail.com

LICENSE:

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

See http://creativecommons.org/licenses/MIT/ for more information.
*/
#include <lib.h>

namespace lst {

#ifndef DAC_MCP4921_H_
#define DAC_MCP4921_H_

#define DAC_ON_TIMER (0)

#define DAC_CS_PORT PORTB
#define DAC_CS_DDR DDRB
#define DAC_CS PB2
#define DAC_CS2 PB1
#define DAC_CS3 PB0

#define MCP4921_ABSEL 7
#define MCP4921_BUF 6
#define MCP4921_GAIN 5
#define MCP4921_SHDN 4

#define SPI_PIN PINB
#define SPI_PORT PORTB
#define SPI_DDR DDRB
#define SPI_MOSI PB3
#define SPI_MISO PB4
#define SPI_SCLK PB5

void output_dac(uint8_t channel, uint8_t id, uint16_t data);
void init_spi();

#endif /* DAC_MCP4921_H_ */

}

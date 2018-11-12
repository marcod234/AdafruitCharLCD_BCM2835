/* Ported to use the bcm2835 library for Raspberry Pi by Marco Duarte*/

/*************************************************** 
  This is a library for the MCP23017 i2c port expander

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

 #include "Adafruit_MCP23017.h"
////////////////////////////////////////////////////////////////////////////////

//function wrapping bcm2835_i2c_begin
void Adafruit_MCP23017::i2cBegin()
{
  if(!bcm2835_i2c_begin())
  {
    printf("i2c failed, run as root\n");
    exit(0);
  }
}

void Adafruit_MCP23017::begin(uint8_t addr) {
  if (addr > 7) {
    addr = 7;
  }
  i2caddr = addr;
 
  // set defaults!
  //i2cBegin();
  //bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_148);
  
  bcm2835_i2c_setSlaveAddress(MCP23017_ADDRESS | i2caddr);
  writeByte(MCP23017_IODIRA);
  writeByte(0xFF);  // all inputs on port A

  writeByte(MCP23017_IODIRB);
  writeByte(0xFF);  // all inputs on port B
}

void Adafruit_MCP23017::writeByte(uint8_t byte)
{
  char buffer[] = {byte};
  
  bcm2835_i2c_write(buffer, sizeof(buffer));
}

void Adafruit_MCP23017::begin(void) {
  begin(0);
}

void Adafruit_MCP23017::pinMode(uint8_t p, uint8_t d) {
  char buff;
  uint8_t iodir;
  uint8_t iodiraddr;

  // only 16 bits!
  if (p > 15)
    return;

  if (p < 8)
    iodiraddr = MCP23017_IODIRA;
  else {
    iodiraddr = MCP23017_IODIRB;
    p -= 8;
  }

  bcm2835_i2c_setSlaveAddress(MCP23017_ADDRESS | i2caddr);
  // read the current IODIR
  writeByte(iodiraddr);	
  
  bcm2835_i2c_read(&buff, 1);
  iodir = buff;

  // set the pin and direction
  if (d == INPUT) {
    iodir |= 1 << p; 
  } else {
    iodir &= ~(1 << p);
  }

  // write the new IODIR
  writeByte(iodiraddr);
  writeByte(iodir);	
}

uint16_t Adafruit_MCP23017::readGPIOAB() {
  char buff[2];
  uint16_t ba = 0;
  uint8_t a;

  bcm2835_i2c_setSlaveAddress(MCP23017_ADDRESS | i2caddr);
  // read the current GPIO output latches
  writeByte(MCP23017_GPIOA);	
  
  bcm2835_i2c_read(buff, 2);
  a = buff[0];
  ba = buff[1];
  ba <<= 8;
  ba |= a;

  return ba;
}

void Adafruit_MCP23017::writeGPIOAB(uint16_t ba) {
  bcm2835_i2c_setSlaveAddress(MCP23017_ADDRESS | i2caddr);
  writeByte(MCP23017_GPIOA);	
  writeByte(ba & 0xFF);
  writeByte(ba >> 8);
}

void Adafruit_MCP23017::digitalWrite(uint8_t p, uint8_t d) {
  char buff;
  uint8_t gpio;
  uint8_t gpioaddr, olataddr;

  // only 16 bits!
  if (p > 15)
    return;

  if (p < 8) {
    olataddr = MCP23017_OLATA;
    gpioaddr = MCP23017_GPIOA;
  } else {
    olataddr = MCP23017_OLATB;
    gpioaddr = MCP23017_GPIOB;
    p -= 8;
  }

  bcm2835_i2c_setSlaveAddress(MCP23017_ADDRESS | i2caddr);
  // read the current GPIO output latches
  writeByte(olataddr);	
  
  bcm2835_i2c_read(&buff, 1);
  gpio = buff;

  // set the pin and direction
  if (d == HIGH) {
    gpio |= 1 << p; 
  } else {
    gpio &= ~(1 << p);
  }

  // write the new GPIO
  writeByte(gpioaddr);
  writeByte(gpio);	
}

void Adafruit_MCP23017::pullUp(uint8_t p, uint8_t d) {
  char buff;
  uint8_t gppu;
  uint8_t gppuaddr;

  // only 16 bits!
  if (p > 15)
    return;

  if (p < 8)
    gppuaddr = MCP23017_GPPUA;
  else {
    gppuaddr = MCP23017_GPPUB;
    p -= 8;
  }

  bcm2835_i2c_setSlaveAddress(MCP23017_ADDRESS | i2caddr);
  // read the current pullup resistor set
  writeByte(gppuaddr);	
  
  bcm2835_i2c_read(&buff, 1);
  gppu = buff;

  // set the pin and direction
  if (d == HIGH) {
    gppu |= 1 << p; 
  } else {
    gppu &= ~(1 << p);
  }

  // write the new GPIO
  writeByte(gppuaddr);
  writeByte(gppu);	
}

uint8_t Adafruit_MCP23017::digitalRead(uint8_t p) {
  char buff;
  uint8_t gpioaddr;

  // only 16 bits!
  if (p > 15)
    return 0;

  if (p < 8)
    gpioaddr = MCP23017_GPIOA;
  else {
    gpioaddr = MCP23017_GPIOB;
    p -= 8;
  }

  // read the current GPIO
  bcm2835_i2c_setSlaveAddress(MCP23017_ADDRESS | i2caddr);
  writeByte(gpioaddr);	
  
  bcm2835_i2c_read(&buff, 1);
  return (buff >> p) & 0x1;
}

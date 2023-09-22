/*
 * gpio_sc16IS750.c
 *
 *  Created on: 19 sept. 2022
 *      Author: dvdsosa
 */

#include "gpio_sc16IS750.h"

extern status_t i2c_read(uint8_t, uint8_t*, uint8_t*, uint8_t);
extern status_t i2c_write(uint8_t, uint8_t*, uint8_t);

//Returns the decimal value of sensor channel single-ended input
uint8_t gpio_pollCfg(void)
{
  /* Config GPIOs as output */
  uint8_t remoteAddress = SC16IS750_ADDRESS;
  uint8_t buffer[] = { (IODIR_REG << 3 ), 0xFF};
  status_t status = i2c_write(remoteAddress, buffer, 2);

  /* Disable GPIO interrupt */
  buffer[0] = ( IOINTENA_REG << 3);
  buffer[1] = 0x00;
  status = i2c_write(remoteAddress, buffer, 2);

  return status;
}

uint8_t gpio_toggle(uint8_t *data)
{
  uint8_t remoteAddress = SC16IS750_ADDRESS;
  uint8_t pin = 0;

  switch(*data)
  {
    case 1:
	pin = 0x01;
      break;
    case 2:
	pin = 0x02;
      break;
    case 3:
	pin = 0x04;
      break;
    case 4:
	pin = 0x08;
      break;
    case 5:
	pin = 0x10;
      break;
    case 6:
	pin = 0x20;
      break;
    case 7:
	pin = 0x40;
      break;
    default:
      return kStatus_Fail;
      break;
  }

  uint8_t buffer[] = { (IOSTATE_REG << 3 ), pin};

  /* Change GPIO */
  status_t status = i2c_write(remoteAddress, buffer, 2);

  return status;
}

uint8_t gpio_powerDown(void)
{
  uint8_t remoteAddress = SC16IS750_ADDRESS;
  uint8_t buffer[] = { (LCR_REG << 3), 0x80};
  status_t status = i2c_write(remoteAddress, buffer, 2);

  buffer[0] = (DLL_REG << 3);
  buffer[1] = 0x08;
  status = i2c_write(remoteAddress, buffer, 2);

  buffer[0] = (DLH_REG << 3);
  buffer[1] = 0x00;
  status = i2c_write(remoteAddress, buffer, 2);

  buffer[0] = (LCR_REG << 3);
  buffer[1] = 0xBF;
  status = i2c_write(remoteAddress, buffer, 2);

  buffer[0] = (EFR_REG << 3);
  buffer[1] = 0x10;
  status = i2c_write(remoteAddress, buffer, 2);

  buffer[0] = (LCR_REG << 3);
  buffer[1] = 0x03;
  status = i2c_write(remoteAddress, buffer, 2);

  buffer[0] = (FCR_REG << 3);
  buffer[1] = 0x07;
  status = i2c_write(remoteAddress, buffer, 2);

  buffer[0] = (IER_REG << 3);
  buffer[1] = 0x11;
  status = i2c_write(remoteAddress, buffer, 2);

  return status;
}

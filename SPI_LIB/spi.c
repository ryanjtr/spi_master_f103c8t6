/*
 * spi.c
 *
 *  Created on: Nov 13, 2024
 *      Author: dell
 */
#include "spi.h"

bool spi_transmit(uint8_t *pData, uint8_t len, uint32_t timeout)
{
  uint32_t count = 0;
  uint8_t index = 0;
  // Chờ cờ BUSY tắt
  while (LL_SPI_IsActiveFlag_BSY(SPI1))
  {
    if (count > timeout)
    {
      return false;
    }
    else
      count++;
  }
  count = 0;

  // Bật ngoại vi SPI
  LL_SPI_Enable(SPI1);
  // Truyền dữ liệu
  while (index < len)
  {
    // Kiểm tra bộ đệm truyền có trống hay không
    if (LL_SPI_IsActiveFlag_TXE(SPI1))
    {
      LL_SPI_TransmitData8(SPI1, pData[index]);
      index++;
      count = 0;
    }
    else
    {
      if (count > timeout)
      {
        return false;
      }
      else
        count++;
    }
  }
  // Chờ cờ BUSY tắt
  while (LL_SPI_IsActiveFlag_BSY(SPI1))
  {
    if (count > timeout)
    {
      return false;
    }
    else
      count++;
  }
  count = 0;
  // Xóa cờ OVERRUN
  LL_SPI_ClearFlag_OVR(SPI1);
  return true;
}

bool spi_receive(uint8_t *pData, uint8_t len, uint32_t timeout)
{
  uint32_t count = 0;
  uint8_t index = 0;

  // Chờ cờ BUSY tắt
  while (LL_SPI_IsActiveFlag_BSY(SPI1))
  {
    if (count > timeout)
    {
      return false;
    }
    else
      count++;
  }
  count = 0;

  // Bật ngoại vi SPI
  LL_SPI_Enable(SPI1);
  bool isTransmit = 1;
  // Truyền trước sau đó nhận dữ liệu về
  while (index < len)
  {
    // Truyền dữ liệu rác trước
    // Kiểm tra bộ đệm truyền có trống hay không và isTransmit = 1 hay không
    if ((LL_SPI_IsActiveFlag_TXE(SPI1)) && (isTransmit))
    {
      LL_SPI_TransmitData8(SPI1, 0xFF);
      isTransmit = 0;
    }

    // Nhận dữ liệu
    if (LL_SPI_IsActiveFlag_RXNE(SPI1))
    {
      pData[index] = LL_SPI_ReceiveData8(SPI1);
      index++;
      isTransmit = 1;
      count = 0;
    }
    else
    {
      if (count > timeout)
      {
        return false;
      }
      else
        count++;
    }
  }
  // Chờ cờ BUSY tắt
  while (LL_SPI_IsActiveFlag_BSY(SPI1))
  {
    if (count > timeout)
    {
      return false;
    }
    else
      count++;
  }
  count = 0;
  // Xóa cờ OVERRUN
  LL_SPI_ClearFlag_OVR(SPI1);
  return true;
}

bool spi_transmit_receive(uint8_t *txdata, uint8_t *rxdata, uint8_t len, uint32_t timeout)
{
  uint32_t count = 0;
  uint8_t index = 0;
  // Chờ cờ BUSY tắt
  while (LL_SPI_IsActiveFlag_BSY(SPI1))
  {
    if (count > timeout)
    {
      return false;
    }
    else
      count++;
  }
  count = 0;

  // Bật ngoại vi SPI
  LL_SPI_Enable(SPI1);
  bool isTransmit = 1;
  while (index < len)
  {

    // Kiểm tra bộ đệm truyền có trống hay không và isTransmit = 1 hay không
    if ((LL_SPI_IsActiveFlag_TXE(SPI1)) && (isTransmit))
    {
      LL_SPI_TransmitData8(SPI1, txdata[index]);
      isTransmit = 0;
    }

    // Nhận dữ liệu
    if (LL_SPI_IsActiveFlag_RXNE(SPI1))
    {
      rxdata[index] = LL_SPI_ReceiveData8(SPI1);
      index++;
      isTransmit = 1;
      count = 0;
    }
    else
    {
      if (count > timeout)
      {
        return false;
      }
      else
        count++;
    }
  }

  // Chờ cờ BUSY tắt
  while (LL_SPI_IsActiveFlag_BSY(SPI1))
  {
    if (count > timeout)
    {
      return false;
    }
    else
      count++;
  }
  count = 0;
  // Xóa cờ OVERRUN
  LL_SPI_ClearFlag_OVR(SPI1);
  return true;
}
/*----------------------------------Function for EEPROM W25Q32-----------------------------------*/
void read_id_manufacter(uint8_t rxdata[4])
{
  // Dùng salae để coi ID
  uint8_t txdata[4] = {0x9F, 0x00, 0x00, 0x00};
  LL_GPIO_ResetOutputPin(CS3_GPIO_Port, CS3_Pin);
  spi_transmit_receive(txdata, rxdata, 4, 1000);
  LL_GPIO_SetOutputPin(CS3_GPIO_Port, CS3_Pin);
  LL_mDelay(1000);
//  for(int i=0;i<1000;i++);
}

void reset_device(void)
{
  uint8_t enable_reset_instruction = 0x66;
  uint8_t reset_instruction = 0x99;

  LL_GPIO_ResetOutputPin(CS3_GPIO_Port, CS3_Pin);
  spi_transmit(&enable_reset_instruction, 1, 1000);
  LL_GPIO_SetOutputPin(CS3_GPIO_Port, CS3_Pin);

  LL_GPIO_ResetOutputPin(CS3_GPIO_Port, CS3_Pin);
  spi_transmit(&reset_instruction, 1, 1000);
  LL_GPIO_SetOutputPin(CS3_GPIO_Port, CS3_Pin);
  for(int i=0;i<1000;i++);

}
void write_data(uint32_t Address, uint8_t *pdata, uint16_t size)
{
    uint8_t enable_write_instruction = 0x06;
    uint8_t write_page_instruction = 0x02;

    // Kéo CS xuống mức 0 để bắt đầu giao tiếp
    LL_GPIO_ResetOutputPin(CS3_GPIO_Port, CS3_Pin);
    // Bật tính năng ghi bằng lệnh Write Enable (0x06)
    spi_transmit(&enable_write_instruction, 1, 1000);
    // Kéo CS lên mức 1 để hoàn thành lệnh
    LL_GPIO_SetOutputPin(CS3_GPIO_Port, CS3_Pin);
    // Kéo CS xuống mức 0 để bắt đầu lệnh ghi trang
    LL_GPIO_ResetOutputPin(CS3_GPIO_Port, CS3_Pin);
    // Gửi lệnh ghi trang (0x02)
    spi_transmit(&write_page_instruction, 1, 1000);
    // Gửi địa chỉ 24-bit (A23-A0)
    uint8_t addr_bytes[3];
    addr_bytes[0] = (Address >> 16) & 0xFF; // A23-A16
    addr_bytes[1] = (Address >> 8) & 0xFF;  // A15-A8
    addr_bytes[2] = Address & 0xFF;         // A7-A0
    spi_transmit(addr_bytes, 3, 1000);

    // Gửi dữ liệu cần ghi
    spi_transmit(pdata, size, 1000);
    // Kéo CS lên mức 1 để hoàn thành lệnh
    LL_GPIO_SetOutputPin(CS3_GPIO_Port, CS3_Pin);

//    for(int i=0;i<1000;++i);//Delay
}

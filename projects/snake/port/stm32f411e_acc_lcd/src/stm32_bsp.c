#include "stm32f4xx.h"
#include "stm32_init.h"
#include "stm32_bsp.h"
#include <string.h> 

//#include "lsm303dlhc.h"
#include "lis3dh_reg.h"

#include "ssd1306.h"


static stmdev_ctx_t dev_ctx;

I2C_HandleTypeDef hi2c1;
extern FontDef Font_8x8;
FontDef* Font = &Font_8x8;

int stm32_i2c_init (void)
{
  static uint8_t result = 0;

  if(!result) {
    if (stm32_init()) {
      hi2c1.Instance = I2C1;
      hi2c1.Init.ClockSpeed = 100000;
      hi2c1.Init.DutyCycle = 0;
      hi2c1.Init.OwnAddress1 = 0;
      hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
      hi2c1.Init.DualAddressMode = 0;
      hi2c1.Init.OwnAddress2 = 0;
      hi2c1.Init.GeneralCallMode = 0;
      hi2c1.Init.NoStretchMode = 0;
   
      result = (HAL_I2C_Init(&hi2c1) == HAL_OK);
    }
  }
  return result;
}

void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_I2C1_CLK_ENABLE();  

    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9; // SCL, PB6->PB8, SDA, PB9->PB9
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
}

// Inicializa I2C (llama a stm32_i2c_init):
void COMPASSACCELERO_IO_Init (void)
{
  if (!stm32_i2c_init()) {
    while (1) {
      ;
    }
  }
}

// Inicializa interrupción de Acelerómetro (click, PE4 y PE5)
void COMPASSACCELERO_IO_ITConfig (void)
{
}

// Escribe en el acelerómetro byte a byte (configuración)
void COMPASSACCELERO_IO_Write (uint16_t DeviceAddr, uint8_t RegisterAddr, uint8_t Value)
{
  if (HAL_I2C_Mem_Write(&hi2c1, LIS3DH_I2C_ADD_H, RegisterAddr, 1, &Value, 1, 100) != HAL_OK) {
    while (1) {
      ;
    }
  }
}

// Lee del acelerómetro byte a byte (configuración)
uint8_t COMPASSACCELERO_IO_Read (uint16_t DeviceAddr, uint8_t RegisterAddr)
{
  uint8_t value = 0;
  if (HAL_I2C_Mem_Read(&hi2c1, LIS3DH_I2C_ADD_H, RegisterAddr, 1, &value, 1, 100) != HAL_OK) {
    while (1) {
      ;
    }
  }
  return value;
}

ACCELERO_DrvTypeDef* bsp_get_accelero(void) {
  return 0; //Ya inicializo en acc init
}

uint16_t bsp_get_accelero_config(void) {

  return 0;
}

int lcd_init(void)
{
  static int result = 0;

  if (!result) {
    result = stm32_i2c_init() && !ssd1306_Init(&hi2c1);
  }
  return result;
}

void 
lcd_reset(void) 
{
  ssd1306_Fill(0);        
  ssd1306_UpdateScreen(&hi2c1); 
  ssd1306_SetCursor(0, 0); 
}

void 
lcd_update_screen(void) 
{
  ssd1306_UpdateScreen(&hi2c1);
}

void lcd_set_cursor(uint8_t x, uint8_t y)
{
  ssd1306_SetCursor(y*Font->FontWidth, x*Font->FontHeight);
}

void lcd_write_char(char ch)
{
  ssd1306_WriteChar(ch, Font_8x8, White);
}

void ssd1306_Delay(uint32_t ms) {
  HAL_Delay(ms);
}

uint8_t ssd1306_WriteCommand(void *hi2c, uint8_t command)
{
    return HAL_I2C_Mem_Write((I2C_HandleTypeDef *)hi2c,
                           SSD1306_I2C_ADDR ,  
                           0x00, 1,  
                           &command, 1, 100);
}

uint8_t ssd1306_WriteData(void *hi2c, uint8_t* data, uint16_t len)
{
  return HAL_I2C_Mem_Write((I2C_HandleTypeDef *)hi2c,
                           SSD1306_I2C_ADDR ,  
                           0x40, 1,  
                           data, len, 100);
}



// Funciones para lis3dh

int32_t LIS3DH_IO_Write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len)
{
  // Usamos la dirección de escritura de 8 bits del LIS3DH (0x32)
  if (HAL_I2C_Mem_Write((I2C_HandleTypeDef*)handle, 0x32, reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)bufp, len, 1000) != HAL_OK) {
    return -1;
  }
  return 0;
}

int32_t LIS3DH_IO_Read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
  // HAL_I2C_Mem_Read también recibe la dirección 0x32, la HAL le aplica el bit R/W internamente
  if (HAL_I2C_Mem_Read((I2C_HandleTypeDef*)handle, 0x32, reg, I2C_MEMADD_SIZE_8BIT, bufp, len, 1000) != HAL_OK) {
    return -1;
  }
  return 0;
}

void LIS3DH_AccReadXYZ(int16_t *pDataXYZ)
{
  /* Llama pasándole el contexto registrado del sensor y el buffer */
  lis3dh_acceleration_raw_get(&dev_ctx, pDataXYZ);
}


void LIS3DH_AccInit(uint16_t InitStruct)
{
  // 1. Inicializamos el hardware físico del I2C (pines, reloj, etc.)
  COMPASSACCELERO_IO_Init();

  // 2. Conectamos las funciones puente al contexto del submódulo
  dev_ctx.write_reg = (stmdev_write_ptr)LIS3DH_IO_Write;
  dev_ctx.read_reg  = (stmdev_read_ptr)LIS3DH_IO_Read;
  dev_ctx.handle    = &hi2c1; // Le pasamos nuestro bus de datos

  // 3. CONFIGURACIÓN DEL ODR A 50HZ USANDO EL SUBMÓDULO
  // Invocamos la función propia de lis3dh.c para cambiar la frecuencia de muestreo.
  // Esta función llamará por debajo a tu LIS3DH_IO_Write de forma totalmente automática.
  lis3dh_data_rate_set(&dev_ctx, LIS3DH_ODR_50Hz);
  
  // 4. Habilitamos también los ejes X, Y, Z para que el juego pueda leerlos
  lis3dh_operating_mode_set(&dev_ctx, LIS3DH_HR_12bit); // Modo alta resolución (opcional)
}
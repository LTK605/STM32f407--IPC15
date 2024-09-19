#include "SPI2.h"
/*
#define FLASH_CS_PIN        GPIO_Pin_1       		 //CS引脚
#define FLASH_MISO_PIN      GPIO_Pin_2       		 //MISO引脚
#define FLASH_MOSI_PIN      GPIO_Pin_3       		 //MOSI引脚
#define FLASH_SCK_PIN       GPIO_Pin_13       		 //SCK引脚
#define FLASH_MMC_PORT      GPIOC                     // SPI2 MISO MOSI CS 端口
#define FLASH_MMC_CLK       RCC_AHB1Periph_GPIOC           // SPI2 MISO MOSI CS 端口
#define FLASH_SCK_PORT      GPIOB    // SPI2 SCK端口时钟
#define FLASH_SCK_CLK       RCC_AHB1Periph_GPIOB    // SPI2 SCK端口时钟
*/
void SPI2_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef SPI_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); // 使能GPIOB时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // 使能GPIOB时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  // 使能SPI2时钟

  // GPIOFB13初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;         // PB13复用功能输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       // 复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       // 上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);             // 初始化

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;         // PB3~5复用功能输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       // 复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       // 上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);             // 初始化

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2); // PB13复用为 SPI2
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_SPI2);  // PC2复用为 SPI2
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);  // PC3复用为 SPI2

  // 这里只针对SPI口初始化
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);  // 复位SPI2
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE); // 停止复位SPI2

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   // 设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                        // 设置SPI工作模式:设置为主SPI
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                    // 设置SPI的数据大小:SPI发送接收8位帧结构
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High  ;                          // 串行同步时钟的空闲状态为高电平
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                         // 串行同步时钟的第二个跳变沿（上升或下降）数据被采样
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                            // NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // 定义波特率预分频的值:波特率预分频值为256
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                   // 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
  SPI_InitStructure.SPI_CRCPolynomial = 7;                             // CRC值计算的多项式
  SPI_Init(SPI2, &SPI_InitStructure);                                  // 根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

  SPI_Cmd(SPI2, ENABLE); // 使能SPI外设

  SPI2_ReadWriteByte(0xff); // 启动传输rxData
}
// SPI2速度设置函数
// SPI速度=fAPB1/分频系数
//@refSPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256
// fAPB2时钟一般为42Mhz：
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); // 判断有效性
  SPI2->CR1 &= 0XFFC7;                                            // 位3-5清零，用来设置波特率
  SPI2->CR1 |= SPI_BaudRatePrescaler;                             // 设置SPI2速度
  SPI_Cmd(SPI2, ENABLE);                                          // 使能SPI2
}
// SPI2 读写一个字节
// TxData:要写入的字节
// 返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{
	//检查TXE为空的时候再发送数据，缓冲区非空的标志为0
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET);
	//通过这个while后说明数据BUFFER为空了，就可以发送数据了
	SPI_I2S_SendData(SPI2,TxData);
	//检测RXNE标志为置1表示数据发送完毕
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE) == RESET);
	//返回接收到的数据
	return SPI_I2S_ReceiveData(SPI2);
}

#include "icm20602.h"
// PE3:CS
// PB4:MISO
// PA7:MOSI
// PA5:SCK
// PE8:INT

static float _accel_scale;
static float _gyro_scale;
u8 idtype=0;
void ICM_CSINT_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); // 使能GPIOB时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_8; // PE3功能输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		   // 复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	   // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   // 上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;		   // PE8输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	   // 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	   // 下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);			   // 初始化
	ICM20602_CS = 1; 
	
}

uint8_t icm20602_read_buffer(uint8_t const regAddr, uint8_t *pData, uint8_t len)
{
    uint8_t rxData = 0;
	uint8_t txData = 0;
	uint8_t i = 0;
	ICM20602_CS = 0; // CS置低，开始传输
	// 发送寄存器地址并接收响应
	txData = regAddr | 0x80;			 // 设置读取位
	rxData = SPI1_ReadWriteByte(txData); // 使用您的函数发送地址并接收数据
//	// 循环读取数据
	for (i = 0; i < len; i++)
	{
		pData[i] = SPI1_ReadWriteByte(0xff); // 发送虚拟字节并接收数据
	}

	ICM20602_CS = 1; // CS置高，结束传输
	return 0;		 // 返回0表示成功
					 /*ICM20602_CS = 0;// HAL_GPIO_WritePin(CS_ICM_GPIO_Port, CS_ICM_Pin,0);
				 tx = regAddr | 0x80;
				 tx_buff[0] = tx;
				 SPI_I2S_SendReceiveData(tx);
				 // HAL_SPI_TransmitReceive(&hspi2, &tx, &rx, 1, 55);
				 // HAL_SPI_TransmitReceive(&hspi2, tx_buff, pData, len, 55);
				 ICM20602_CS = 1;// HAL_GPIO_WritePin(CS_ICM_GPIO_Port, CS_ICM_Pin,1);
				 return 0;*/
}

uint8_t icm20602_write_reg(uint8_t reg, uint8_t val)
{
	uint8_t txData = 0;
	uint8_t rxData = 0;
	ICM20602_CS = 0; // CS置低，开始传输
	// 发送寄存器地址
	txData = reg & 0x7F;				 // 清除读取位
	rxData = SPI1_ReadWriteByte(txData); // 使用您的函数发送地址并接收数据
	// 发送寄存器值
	txData = val;						 // 设置要写入的值
	rxData = SPI1_ReadWriteByte(txData); // 使用您的函数发送值并接收数据
	ICM20602_CS = 1;					 // CS置高，结束传输
	return 0;							 // 返回0表示成功
										 /*ICM20602_CS = 0;
										 tx = reg & 0x7F;
										 HAL_SPI_TransmitReceive(&hspi2, &tx, &rx, 1, 55);
										 tx = val;
										 HAL_SPI_TransmitReceive(&hspi2, &tx, &rx, 1, 55);
										 ICM20602_CS = 1;
										 return 0;*/
}

uint8_t icm20602_read_reg(uint8_t reg)
{
	uint8_t txData = 0;
	uint8_t rxData = 0;
	ICM20602_CS = 0; // CS置低，开始传输

	// 发送寄存器地址并接收响应
	txData = reg | 0x80;				 // 设置读取位
	rxData = SPI1_ReadWriteByte(txData); // 使用您的函数发送地址并接收数据

	// 发送虚拟字节并接收寄存器值
	//txData = 0xFF;						 // 发送任意值
	rxData = SPI1_ReadWriteByte(0xFF); // 使用您的函数发送虚拟字节并接收数据

	ICM20602_CS = 1; // CS置高，结束传输
	return rxData;	 // 返回读取到的寄存器值
}

uint8_t icm20602_init()
{   ICM_CSINT_Init(); 
    SPI1_Init();
   
//	icm20602_write_reg(ICM20_PWR_MGMT_1, 0x80);
	if (icm20602_write_reg(ICM20_PWR_MGMT_1, 0x80)) // 复位，复位后位0x41,睡眠模式，
	{

		printf("icm_20602 reset fail\r\n");
		return 1;
	}
	delay_ms(25);
	   idtype=icm20602_read_reg(ICM20_WHO_AM_I);
	printf("icm_20602 id=%x\r\n", idtype); // 读取ID
	OLED_ShowHexNum(2, 4,idtype, 2);
	delay_ms(25);
	icm20602_write_reg(ICM20_PWR_MGMT_1, 0x01); // 关闭睡眠，自动选择时钟
	delay_ms(25);
	icm20602_write_reg(ICM20_SMPLRT_DIV, 0);								  // 分频数=为0+1，数据输出速率为内部采样速率
	icm20602_write_reg(ICM20_CONFIG, DLPF_BW_20);							  // GYRO低通滤波设置
	icm20602_write_reg(ICM20_ACCEL_CONFIG2, ACCEL_AVER_4 | ACCEL_DLPF_BW_21); // ACCEL低通滤波设置

	icm20602_set_accel_fullscale(ICM20_ACCEL_FS_8G);
	icm20602_set_gyro_fullscale(ICM20_GYRO_FS_2000);

	delay_ms(100);
	printf("icm20602 init pass\r\n\r\n");
 
	
	
	return 0;
}

uint8_t icm20602_set_gyro_fullscale(uint8_t fs)
{
	switch (fs)
	{
	case ICM20_GYRO_FS_250:
		_gyro_scale = 1.0f / 131.068f; // 32767/250
		break;
	case ICM20_GYRO_FS_500:
		_gyro_scale = 1.0f / 65.534f;
		break;
	case ICM20_GYRO_FS_1000:
		_gyro_scale = 1.0f / 32.767f;
		break;
	case ICM20_GYRO_FS_2000:
		_gyro_scale = 1.0f / 16.4f;
		break;
	default:
		fs = ICM20_GYRO_FS_2000;
		_gyro_scale = 1.0f / 16.3835f;
		break;
	}

	return icm20602_write_reg(ICM20_GYRO_CONFIG, fs);
}

uint8_t icm20602_set_accel_fullscale(uint8_t fs)
{
	switch (fs)
	{
	case ICM20_ACCEL_FS_2G:
		_accel_scale = 1.0f / 16348.0f;
		break;
	case ICM20_ACCEL_FS_4G:
		_accel_scale = 1.0f / 8192.0f;
		break;
	case ICM20_ACCEL_FS_8G:
		_accel_scale = 1.0f / 4096.0f;
		break;
	case ICM20_ACCEL_FS_16G:
		_accel_scale = 1.0f / 2048.0f;
		break;
	default:
		fs = ICM20_ACCEL_FS_8G;
		_accel_scale = 1.0f / 4096.0f;
		break;
	}
	return icm20602_write_reg(ICM20_ACCEL_CONFIG, fs);
}

uint8_t icm20602_get_accel_adc(int16_t *accel)
{
	uint8_t buf[6];
	if (icm20602_read_buffer(ICM20_ACCEL_XOUT_H, buf, 6))
		return 1;

	accel[0] = ((int16_t)buf[0] << 8) + buf[1];
	accel[1] = ((int16_t)buf[2] << 8) + buf[3];
	accel[2] = ((int16_t)buf[4] << 8) + buf[5];
	return 0;
}

uint8_t icm20602_get_gyro_adc(int16_t *gyro)
{
	uint8_t buf[6];
	if (icm20602_read_buffer(ICM20_GYRO_XOUT_H, buf, 6))
		return 1;
	gyro[0] = (buf[0] << 8) + buf[1];
	gyro[1] = (buf[2] << 8) + buf[3];
	gyro[2] = (buf[4] << 8) + buf[5];
	return 0;
}
uint8_t icm20602_get_gyro(float *gyro)//°/s
{
	int16_t gyro_adc[3];
	if (icm20602_get_gyro_adc(gyro_adc))
		return 1;

	gyro[0] = _gyro_scale * gyro_adc[0];
	gyro[1] = _gyro_scale * gyro_adc[1];
	gyro[2] = _gyro_scale * gyro_adc[2];
	return 0;
}
uint8_t icm20602_get_accel(float *accel)
{
	int16_t accel_adc[3];
	if (icm20602_get_accel_adc(accel_adc))
		return 1;
	accel[0] = _accel_scale * accel_adc[0];
	accel[1] = _accel_scale * accel_adc[1];
	accel[2] = _accel_scale * accel_adc[2];
	return 0;
}

float icm20602_get_temp()
{
	int16_t temp_adc;
	uint8_t buf[2];
	if (icm20602_read_buffer(ICM20_TEMP_OUT_H, buf, 2))
		return 0.0f;

	temp_adc = (buf[0] << 8) + buf[1];

	return (25.0f + (float)temp_adc / 326.8f);
}
float icm20602_gyro[3];

void icm20602_test(void)
{  float icm20602_temp;
	icm20602_get_gyro(icm20602_gyro);
//	printf("icm20602_gyrox:%f\r\n", icm20602_gyro[0]);
//	printf("icm20602_gyroy:%f\r\n", icm20602_gyro[1]);
//	printf("icm20602_gyroz:%f\r\n", icm20602_gyro[2]);
	//icm20602_temp=icm20602_get_temp();
OLED_ShowSignedNum(3, 8, icm20602_gyro[1], 2);
	//OLED_ShowSignedNum(3, 7, icm20602_temp, 2);
}

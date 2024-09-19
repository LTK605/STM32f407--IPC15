#include "bmi088.h"
//PB0:GYRO CS
//PC4:ACC CS
//PB4:MISO
//PA7:MOSI
//PA5:SCK
//PB1:GYRO_INT
//PE7:ACC_INT
void BMI_CSINT_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); // 使能GPIOB时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // 使能GPIOC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); // 使能GPIOE时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; // PE3功能输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		   // 复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	   // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   // 上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // PE3功能输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		   // 复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	   // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   // 上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		   // PE7输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	   // 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   // 上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);			   // 初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		   // PB1输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	   // 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   // 上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);			   // 初始化
	
	 BMI088_ACC_CS=1;
	 BMI088_GYRO_CS=1;
}
void WriteDataToAcc(uint8_t addr, uint8_t data) 
{
	BMI088_ACC_CS=0;
    uint8_t rxData = 0;
    uint8_t pTxData = (addr & BMI088_SPI_WRITE_CODE);
	rxData = SPI1_ReadWriteByte(pTxData); 
    pTxData = data;
    rxData = SPI1_ReadWriteByte(pTxData); 
    delay_ms(1);
   BMI088_ACC_CS=1;
}
void WriteDataToGyro(uint8_t addr, uint8_t data)
 {
  BMI088_GYRO_CS=0;
    uint8_t pTxData = (addr & BMI088_SPI_WRITE_CODE);
    uint8_t rxData = 0;
   rxData = SPI1_ReadWriteByte(pTxData); 
    pTxData = data;
   rxData = SPI1_ReadWriteByte(pTxData); 
    delay_ms(1);
	BMI088_GYRO_CS=1;
    //HAL_GPIO_WritePin(BMI088_GYRO_GPIOx, BMI088_GYRO_GPIOp, GPIO_PIN_SET);
}
void ReadSingleDataFromAcc(uint8_t addr, uint8_t *data) {
   BMI088_ACC_CS=0;
  uint8_t rxData = 0;
    uint8_t pTxData = (addr | BMI088_SPI_READ_CODE);
     rxData =SPI1_ReadWriteByte(pTxData); 
	
    *data = SPI1_ReadWriteByte(0xFF); // 发送虚拟字节并接收数据
	BMI088_ACC_CS=1;
}
void ReadSingleDataFromGyro(uint8_t addr, uint8_t *data) {
   BMI088_GYRO_CS=0;
    uint8_t pTxData = (addr | BMI088_SPI_READ_CODE);
     uint8_t rxData = 0;
    *data = SPI1_ReadWriteByte(pTxData); 
   // *data = SPI1_ReadWriteByte(0xFF); // 发送虚拟字节并接收数据
	BMI088_GYRO_CS=1;
}
void ReadMultiDataFromAcc(uint8_t addr, uint8_t len, uint8_t *data) {
	int i =0;
   BMI088_ACC_CS=0;
    uint8_t pTxData = (addr | BMI088_SPI_READ_CODE);
     uint8_t rxData = 0;
    //uint8_t pRxData;
    rxData = SPI1_ReadWriteByte(pTxData); 
  // 发送虚拟字节并接收数据
//	 rxData = SPI1_ReadWriteByte(0xFF); 
    for (i = 0; i < len; i++) 
	{
    data[i] = SPI1_ReadWriteByte(0xFF); // 发送虚拟字节并接收数据
    }
	BMI088_ACC_CS=1;
}
void ReadMultiDataFromGyro(uint8_t addr, uint8_t len, uint8_t *data) {
   BMI088_GYRO_CS=0;
    uint8_t pTxData = (addr | BMI088_SPI_READ_CODE);
    uint8_t rxData = 0;
data[0] =SPI1_ReadWriteByte(pTxData); 
	//SPI_I2S_SendData(SPI1, pTxData); // 通过外设SPIx发送一个byte  数据

    for (int i =1; i < len; i++)
			 {
       data[i] = SPI1_ReadWriteByte(0xFF); // 发送虚拟字节并接收数据
    }
	BMI088_GYRO_CS=1;

}
void BMI088_ALL_INIT(void) 
{
	 SPI1_Init();
	 BMI_CSINT_Init();
    // 加速度计初始化
    // 先软重启，清空所有寄存器
    WriteDataToAcc(ACC_SOFTRESET_ADDR, ACC_SOFTRESET_VAL);
    delay_ms(50);
    // 打开加速度计电源
    WriteDataToAcc(ACC_PWR_CTRL_ADDR, ACC_PWR_CTRL_ON);
    // 加速度计变成正常模式
    WriteDataToAcc(ACC_PWR_CONF_ADDR, ACC_PWR_CONF_ACT);

    // 陀螺仪初始化
    // 先软重启，清空所有寄存器
    WriteDataToGyro(GYRO_SOFTRESET_ADDR, GYRO_SOFTRESET_VAL);
    delay_ms(50);
    // 陀螺仪变成正常模式
    WriteDataToGyro(GYRO_LPM1_ADDR, GYRO_LPM1_NOR);

    // 加速度计配置写入
    // 写入范围，+-3g的测量范围
    WriteDataToAcc(ACC_RANGE_ADDR, ACC_RANGE_3G);
    // 写入配置，正常带宽，1600hz输出频率
    WriteDataToAcc(ACC_CONF_ADDR,
                   (ACC_CONF_RESERVED << 7) | (ACC_CONF_BWP_NORM << 6) | (ACC_CONF_ODR_1600_Hz));

    // 陀螺仪配置写入
    // 写入范围，+-500°/s的测量范围
    WriteDataToGyro(GYRO_RANGE_ADDR, GYRO_RANGE_500_DEG_S);
    // 写入带宽，2000Hz输出频率，532Hz滤波器带宽
    WriteDataToGyro(GYRO_BANDWIDTH_ADDR, GYRO_ODR_2000Hz_BANDWIDTH_532Hz);
}
bmi088_error_e BMI088_INIT(void) {
    bmi088_error_e error = NO_ERROR;

    BMI088_ALL_INIT();
    error |= VerifyAccChipID();
	
    error |= VerifyGyroChipID();
    if (1) {  // 将来改成变量控制自检
        error |= VerifyAccSelfTest();
    }
    if (1) {  // 将来改成变量控制自检
        error |= VerifyGyroSelfTest();
    }
    printf("bmi088:%x\r\n",error);
    return error;
}

bmi088_error_e VerifyAccChipID(void) {
    uint8_t chip_id;
    ReadSingleDataFromAcc(ACC_CHIP_ID_ADDR, &chip_id);
	  delay_ms(200);
	 ReadSingleDataFromAcc(ACC_CHIP_ID_ADDR, &chip_id);//加速度计都需要读两次才能成功
	
    if (chip_id != ACC_CHIP_ID_VAL) {
		printf("ACC_CHIP_ID_ERR:%x\r\n",chip_id);
        return ACC_CHIP_ID_ERR;	
    }
printf("ACC_CHIP_ID_succ:%x\r\n",chip_id);
		OLED_ShowHexNum(2, 7,chip_id, 2);
    return NO_ERROR;
}

bmi088_error_e VerifyGyroChipID(void) {
    uint8_t chip_id;
    //ReadSingleDataFromGyro(GYRO_CHIP_ID_ADDR, &chip_id);
	BMI088_GYRO_CS=0;
    uint8_t pTxData = (GYRO_CHIP_ID_ADDR | BMI088_SPI_READ_CODE);
    chip_id = SPI1_ReadWriteByte(pTxData); 
   chip_id= SPI1_ReadWriteByte(0xFF); // 发送虚拟字节并接收数据
	BMI088_GYRO_CS=1;
	delay_ms(60);
    if (chip_id != GYRO_CHIP_ID_VAL) {
		printf("GYRO_CHIP_ID_ERR:%x\r\n",chip_id);
			return GYRO_CHIP_ID_ERR;
    }
		OLED_ShowHexNum(2, 10,chip_id, 2);
		printf("GYRO_CHIP_ID_succ:%x\r\n",chip_id);
    return NO_ERROR;
}

bmi088_error_e VerifyAccSelfTest(void) {
    acc_raw_data_t pos_data, neg_data;
    WriteDataToAcc(ACC_RANGE_ADDR, ACC_RANGE_24G);
    WriteDataToAcc(ACC_CONF_ADDR, 0xA7);
    delay_ms(10);
    WriteDataToAcc(ACC_SELF_TEST_ADDR, ACC_SELF_TEST_POS);
    delay_ms(100);
    ReadAccData(&pos_data);
    WriteDataToAcc(ACC_SELF_TEST_ADDR, ACC_SELF_TEST_NEG);
    delay_ms(100);
    ReadAccData(&neg_data);
    WriteDataToAcc(ACC_SELF_TEST_ADDR, ACC_SELF_TEST_OFF);
    delay_ms(100);
    if ((fabs(pos_data.x - neg_data.x) > 0.1f) || (fabs(pos_data.y - neg_data.y) > 0.1f) || (fabs(pos_data.z - neg_data.z) > 0.1f)) 
			{
			//printf("ACC__ERR\r\n");
        return ACC_DATA_ERR;
    }
    WriteDataToAcc(ACC_SOFTRESET_ADDR, ACC_SOFTRESET_VAL);
    WriteDataToAcc(ACC_PWR_CTRL_ADDR, ACC_PWR_CTRL_ON);
    WriteDataToAcc(ACC_PWR_CONF_ADDR, ACC_PWR_CONF_ACT);
    WriteDataToAcc(ACC_CONF_ADDR,
                   (ACC_CONF_RESERVED << 7) | (ACC_CONF_BWP_NORM << 6) | (ACC_CONF_ODR_1600_Hz));
    WriteDataToAcc(ACC_RANGE_ADDR, ACC_RANGE_3G);
    return NO_ERROR;
}

bmi088_error_e VerifyGyroSelfTest(void) {
    WriteDataToGyro(GYRO_SELF_TEST_ADDR, GYRO_SELF_TEST_ON);
    uint8_t bist_rdy = 0x00, bist_fail;
    while (bist_rdy == 0) {
        ReadSingleDataFromGyro(GYRO_SELF_TEST_ADDR, &bist_rdy);
        bist_rdy = (bist_rdy & 0x02) >> 1;
    }
    ReadSingleDataFromGyro(GYRO_SELF_TEST_ADDR, &bist_fail);
    bist_fail = (bist_fail & 0x04) >> 2;
    if (bist_fail == 0) {
        return NO_ERROR;
    } else {
			//printf("GYRO__ERR\r\n");
        return GYRO_DATA_ERR;
    }
}

void ReadAccData(acc_raw_data_t *data) {
    uint8_t buf[ACC_XYZ_LEN], range;
    int16_t acc[3];
    ReadSingleDataFromAcc(ACC_RANGE_ADDR, &range);
    ReadMultiDataFromAcc(ACC_X_LSB_ADDR, ACC_XYZ_LEN, buf);
    acc[0] = ((int16_t)buf[1] << 8) + (int16_t)buf[0];
    acc[1] = ((int16_t)buf[3] << 8) + (int16_t)buf[2];
    acc[2] = ((int16_t)buf[5] << 8) + (int16_t)buf[4];
    data->x = (float)acc[0] * BMI088_ACCEL_3G_SEN;
    data->y = (float)acc[1] * BMI088_ACCEL_3G_SEN;
    data->z = (float)acc[2] * BMI088_ACCEL_3G_SEN;
}

void ReadGyroData(gyro_raw_data_t *data) {
    uint8_t buf[GYRO_XYZ_LEN], range;
    int16_t gyro[3];
    float unit;
    ReadSingleDataFromGyro(GYRO_RANGE_ADDR, &range);
    switch (range) {
        case 0x00:
            unit = 16.384;
            break;
        case 0x01:
            unit = 32.768;
            break;
        case 0x02:
            unit = 65.536;
            break;
        case 0x03:
            unit = 131.072;
            break;
        case 0x04:
            unit = 262.144;
            break;
        default:
            unit = 16.384;
            break;
    }
    ReadMultiDataFromGyro(GYRO_RATE_X_LSB_ADDR, GYRO_XYZ_LEN, buf);
    gyro[0] = ((int16_t)buf[1] << 8) + (int16_t)buf[0];
    gyro[1] = ((int16_t)buf[3] << 8) + (int16_t)buf[2];
    gyro[2] = ((int16_t)buf[5] << 8) + (int16_t)buf[4];
    data->roll = (float)gyro[0] / unit * DEG2SEC;
    data->pitch = (float)gyro[1] / unit * DEG2SEC;
    data->yaw = (float)gyro[2] / unit * DEG2SEC;
}

void ReadAccSensorTime(float *time) {
    uint8_t buf[SENSORTIME_LEN];
    ReadMultiDataFromAcc(SENSORTIME_0_ADDR, SENSORTIME_LEN, buf);
    *time = buf[0] * SENSORTIME_0_UNIT + buf[1] * SENSORTIME_1_UNIT + buf[2] * SENSORTIME_2_UNIT;
}

void ReadAccTemperature(float *temp) {
    uint8_t buf[TEMP_LEN];
    ReadMultiDataFromAcc(TEMP_MSB_ADDR, TEMP_LEN, buf);
    uint16_t temp_uint11 = (buf[0] << 3) + (buf[1] >> 5);
    int16_t temp_int11;
    if (temp_uint11 > 1023) {
        temp_int11 = (int16_t)temp_uint11 - 2048;
    } else {
        temp_int11 = (int16_t)temp_uint11;
    }
    *temp = temp_int11 * TEMP_UNIT + TEMP_BIAS;
}
gyro_raw_data_t test_gyrodata[3];
acc_raw_data_t test_accdata[3];
void bmi088_test(void)
{
ReadGyroData(test_gyrodata); 
	delay_ms(200);
	//ReadGyroData(test_gyrodata); 
	printf("BMI088DATA_GYROpitch:%f\r\n",test_gyrodata->pitch);
	printf("BMI088DATA_GYROroll:%f\r\n",test_gyrodata->roll);
	printf("BMI088DATA_GYROyaw:%f\r\n",test_gyrodata->yaw);
	ReadAccData(test_accdata);
	delay_ms(200);
	//ReadAccData(test_accdata);
	printf("BMI088DATA_ACCX:%f\r\n",test_accdata->x);
	printf("BMI088DATA_ACCY:%f\r\n",test_accdata->y);
	printf("BMI088DATA_ACCZ:%f\r\n",test_accdata->z);
  OLED_ShowSignedNum(4, 1, test_gyrodata->pitch,3);
	OLED_ShowSignedNum(4, 5, test_gyrodata->roll,3);
	OLED_ShowSignedNum(4, 9, test_gyrodata->yaw,3);
//	  OLED_ShowSignedNum(4, 1, test_accdata->x,3);
//	OLED_ShowSignedNum(4, 5, test_accdata->y,3);
//	OLED_ShowSignedNum(4, 9, test_accdata->z,3);
}

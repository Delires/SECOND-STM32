#ifndef __SPIFlash_H
#define __SPIFlash_H	 
#include "sys.h"
#include "stm32f10x.h"
/**********************************************************/
/**为增加移植性  用宏定义  改变串口直接改变宏即可 ********/
/**宏定义真的超级能增强程序的移植性，硬件部分一定要宏定义 太舒服了 ********/
/**********************************************************/

/**
SPI1
SPI1_NSS----PA4
SPI1_SCK----PA5
SPI1_MISO----PA6  ----AD0/SD0
SPI1_MOSI----PA7  ----SDA/SDI
**/




/*************************************************************************/
/***********************硬件部分定义 可改变用来兼容匹配 ***********************************/
/*************************************************************************/
/*********************** 宏定义 SPI通讯的引脚  ***********************************/
//战舰用的SPI2 PB12/13/14/15 
#define           SPI_NUMBER                    SPI1  

#define           SPI_GPIO_PORT                 GPIOA      
#define           SPI_NSS_CS_PIN                GPIO_Pin_4 
#define           SPI_SCK_PIN                   GPIO_Pin_5
#define           SPI_MISO_PIN                  GPIO_Pin_6 
#define           SPI_MOSI_PIN                  GPIO_Pin_7 


/*********************** SPI通讯的一些函数名 ***********************************/
//SPI使能  定义
#define           SPI_APBxClock_Fun             RCC_APB2PeriphClockCmd
#define           SPI_CLK                       RCC_APB2Periph_SPI1
                                                
//GPIO使能 定义
#define           GPIO_SPI_APBxClock_Fun        RCC_APB2PeriphClockCmd
#define           GPIO_SPI_CLK                  RCC_APB2Periph_GPIOA

//宏定义函数 设置SPI通讯使能(0使能)
#define           FLASH_SPI_CS_HIGH             GPIO_SetBits(SPI_GPIO_PORT, SPI_NSS_CS_PIN) 
#define           FLASH_SPI_CS_LOW              GPIO_ResetBits(SPI_GPIO_PORT, SPI_NSS_CS_PIN)            
                




/*************************************************************************/
/*********************** 9250相关寄存器名 ***********************************/
/*************************************************************************/
/**MPU9250片选信号**/
#define MPU9250_CS    PDout(3) //NCS=1或不接 有数据    NCS=0 GA无数据M有数据   

/**
这六个个寄存器表示的是陀螺仪在工厂生产测试期间生成的自测试输出。此值将用于检查最终用户执行的自测试输出**/
#define SELF_TEST_X_GYRO   0x00  //Gx自检值寄存器 
#define SELF_TEST_Y_GYRO   0x01  
#define SELF_TEST_Z_GYRO   0x02 
#define SELF_TEST_X_ACCEL  0x0D //Ax自检值寄存器
#define SELF_TEST_Y_ACCEL  0x0E 
#define SELF_TEST_Z_ACCEL  0x0F 

/**偏置寄存器 用于校准加速计和陀螺仪  所欧拉角要用这些补偿**/
#define XG_OFFSET_H        0x13
#define XG_OFFSET_L        0x14 
#define YG_OFFSET_H        0x15
#define YG_OFFSET_L        0x16
#define ZG_OFFSET_H        0x17
#define ZG_OFFSET_L        0x18

/**加速度  陀螺仪 配置**/
#define	SMPLRT_DIV		0x19    	  //传感器数据更新速率，典型值：0x07(125Hz)     (手册2.4)
#define	CONFIG			0x1A	      //低通滤波频率，典型值：0x06(5Hz)       (手册2.5)
#define	GYRO_CONFIG		0x1B	      //陀螺仪自检及测量范围                  (手册2.6)
#define	ACCEL_CONFIG	0x1C	      //加速计自检、测量范围                  (手册2.7)
#define ACCEL_CONFIG2   0x1D          //加速度计旁路低通滤波器(DLPF)          (手册2.8)

/**中断INT相关寄存器**/
#define INT_PIN_CFG     0x37         //配置引脚pin 和 使能bypass(旁路模式)    (寄存器55)
#define INT_ENABLE      0x38         //中断使能                               (寄存器55)
#define INT_STATUS      0x3A         //INT中断标志位

/**数据值存储寄存器**/
#define	ACCEL_XOUT_H	0x3B   //加速度计输出数据，由高低16位组成             (寄存器55-64)
#define	ACCEL_XOUT_L	0x3C   //xyz三方向
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41   //温度输出数据，高低两位                       (寄存器65-66)
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43   //陀螺仪输出数据 高低两位组成                  (寄存器67-72)
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48


/**9250启动相关寄存器**/
#define USER_CTRL       0x6A   //用户控制寄存器(FIFO,IIC/SPI/重置)            (寄存器106)

#define PWR_MGMT_1      0x6B   //电源管理1 0x00
#define PWR_MGMT_2      0x6C   //电源管理2 0x00

#define	WHO_AM_I		0x75   //读取MPU6500的器件ID1地址寄存器(默认数值0x71，只读) 与

#define USER_CTRL       0x6A  //用户配置为0X10时为SPI模式


/** MPU9250相应从设备地址 **/
#define MPU9250_ADDR          0x68   //mpu9250 地址(用户自己根据接线定义)
#define MPU9250_ADDR_WRITE    0xD0   //如果AD0脚(9脚)接地,从设备地址为0X68(不包含最低位).
#define MPU9250_ADDR_READ     0xD1   //如果接V3.3,从设备地址为0X69(不包含最低位).
							         //注意这是7位地址 要转换为8位地址 0写 1读
#define MPU6500_ID1			  0X71  	//MPU6500的器件ID1 
#define MPU6500_ID2			  0X73  	//MPU6500的器件ID2

#define MAG_ADDRESS_WRITE     0x18   //磁力计8位写地址 0x0c<<1



/** MPU9250内部封装了一个AK8963磁力计,地址和ID如下**/
#define AK8963_ADDR				0X0C     //AK8963的7位I2C地址
#define AK8963_ID				0X48	//AK8963的器件ID (0x00寄存器有描述)
//AK8963的内部寄存器
#define MAG_WIA					0x00	//AK8963的器件ID寄存器地址
#define MAG_ST1					0x02    //状态寄存器1  数据准备好0位为1 读取后自动变0
#define MAG_CNTL1          	  	0X0A    //设置运行模式：单次转换，连续，自检等
#define MAG_CNTL2            	0X0B    //复位
//读取数据的寄存器
#define MAG_XOUT_L		0x03   //磁力计输出数据
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08

//空指令 维持SPI通讯
#define         DUMMY        0xFF



/*************************************************************************/
/*********************** SPI的相关函数 ***********************************/
/*************************************************************************/
void SPI_Commit_Init(void);

u8 SPI_FLASH_Send_Byte(u8 data); //写1byte+读
u8 SPI_FLASH_Read_Byte(void);  //读1byte 调用上函数


u8 MPU9250_Read_Reg(u8 reg);
u8 MPU9250_Write_Reg(u8 reg,u8 value);

/*************************************************************************/
/*********************** 9250的相关函数 ***********************************/
/*************************************************************************/
void Init_MPU9250(void);
u8 SPI_FLASH_Read_ID(void);
bool Check_AK8963_Exist(void);





/*************************************************************************/
/*********************** Debug部分  Linux ********************************/
/*************************************************************************/

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))


/*信息输出*/
#define  FLASH_DEBUG(message, connect...)      printf("\r\nDebug: File[%s]  Line[%d]  "message"   \r\n",__FILE__, __LINE__, ##connect);                   
                                                     //结构为 "    "message" "
													 //"message" 是调用  FLASH_DEBUG("")  时里头的值
													 //"       " 是原本的值FLASH_DEBUG("")里有没有都会打印




#endif





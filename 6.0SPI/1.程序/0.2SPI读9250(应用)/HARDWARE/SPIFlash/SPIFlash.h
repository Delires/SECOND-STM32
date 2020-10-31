#ifndef __SPIFlash_H
#define __SPIFlash_H	 
#include "sys.h"
#include "stm32f10x.h"
/**********************************************************/
/**Ϊ������ֲ��  �ú궨��  �ı䴮��ֱ�Ӹı�꼴�� ********/
/**�궨����ĳ�������ǿ�������ֲ�ԣ�Ӳ������һ��Ҫ�궨�� ̫����� ********/
/**********************************************************/

/**
SPI1
SPI1_NSS----PA4
SPI1_SCK----PA5
SPI1_MISO----PA6  ----AD0/SD0
SPI1_MOSI----PA7  ----SDA/SDI
**/




/*************************************************************************/
/***********************Ӳ�����ֶ��� �ɸı���������ƥ�� ***********************************/
/*************************************************************************/
/*********************** �궨�� SPIͨѶ������  ***********************************/
//ս���õ�SPI2 PB12/13/14/15 
#define           SPI_NUMBER                    SPI1  

#define           SPI_GPIO_PORT                 GPIOA      
#define           SPI_NSS_CS_PIN                GPIO_Pin_4 
#define           SPI_SCK_PIN                   GPIO_Pin_5
#define           SPI_MISO_PIN                  GPIO_Pin_6 
#define           SPI_MOSI_PIN                  GPIO_Pin_7 


/*********************** SPIͨѶ��һЩ������ ***********************************/
//SPIʹ��  ����
#define           SPI_APBxClock_Fun             RCC_APB2PeriphClockCmd
#define           SPI_CLK                       RCC_APB2Periph_SPI1
                                                
//GPIOʹ�� ����
#define           GPIO_SPI_APBxClock_Fun        RCC_APB2PeriphClockCmd
#define           GPIO_SPI_CLK                  RCC_APB2Periph_GPIOA

//�궨�庯�� ����SPIͨѶʹ��(0ʹ��)
#define           FLASH_SPI_CS_HIGH             GPIO_SetBits(SPI_GPIO_PORT, SPI_NSS_CS_PIN) 
#define           FLASH_SPI_CS_LOW              GPIO_ResetBits(SPI_GPIO_PORT, SPI_NSS_CS_PIN)            
                




/*************************************************************************/
/*********************** 9250��ؼĴ����� ***********************************/
/*************************************************************************/
/**MPU9250Ƭѡ�ź�**/
#define MPU9250_CS    PDout(3) //NCS=1�򲻽� ������    NCS=0 GA������M������   

/**
���������Ĵ�����ʾ�����������ڹ������������ڼ����ɵ��Բ����������ֵ�����ڼ�������û�ִ�е��Բ������**/
#define SELF_TEST_X_GYRO   0x00  //Gx�Լ�ֵ�Ĵ��� 
#define SELF_TEST_Y_GYRO   0x01  
#define SELF_TEST_Z_GYRO   0x02 
#define SELF_TEST_X_ACCEL  0x0D //Ax�Լ�ֵ�Ĵ���
#define SELF_TEST_Y_ACCEL  0x0E 
#define SELF_TEST_Z_ACCEL  0x0F 

/**ƫ�üĴ��� ����У׼���ټƺ�������  ��ŷ����Ҫ����Щ����**/
#define XG_OFFSET_H        0x13
#define XG_OFFSET_L        0x14 
#define YG_OFFSET_H        0x15
#define YG_OFFSET_L        0x16
#define ZG_OFFSET_H        0x17
#define ZG_OFFSET_L        0x18

/**���ٶ�  ������ ����**/
#define	SMPLRT_DIV		0x19    	  //���������ݸ������ʣ�����ֵ��0x07(125Hz)     (�ֲ�2.4)
#define	CONFIG			0x1A	      //��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)       (�ֲ�2.5)
#define	GYRO_CONFIG		0x1B	      //�������Լ켰������Χ                  (�ֲ�2.6)
#define	ACCEL_CONFIG	0x1C	      //���ټ��Լ졢������Χ                  (�ֲ�2.7)
#define ACCEL_CONFIG2   0x1D          //���ٶȼ���·��ͨ�˲���(DLPF)          (�ֲ�2.8)

/**�ж�INT��ؼĴ���**/
#define INT_PIN_CFG     0x37         //��������pin �� ʹ��bypass(��·ģʽ)    (�Ĵ���55)
#define INT_ENABLE      0x38         //�ж�ʹ��                               (�Ĵ���55)
#define INT_STATUS      0x3A         //INT�жϱ�־λ

/**����ֵ�洢�Ĵ���**/
#define	ACCEL_XOUT_H	0x3B   //���ٶȼ�������ݣ��ɸߵ�16λ���             (�Ĵ���55-64)
#define	ACCEL_XOUT_L	0x3C   //xyz������
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41   //�¶�������ݣ��ߵ���λ                       (�Ĵ���65-66)
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43   //������������� �ߵ���λ���                  (�Ĵ���67-72)
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48


/**9250������ؼĴ���**/
#define USER_CTRL       0x6A   //�û����ƼĴ���(FIFO,IIC/SPI/����)            (�Ĵ���106)

#define PWR_MGMT_1      0x6B   //��Դ����1 0x00
#define PWR_MGMT_2      0x6C   //��Դ����2 0x00

#define	WHO_AM_I		0x75   //��ȡMPU6500������ID1��ַ�Ĵ���(Ĭ����ֵ0x71��ֻ��) ��

#define USER_CTRL       0x6A  //�û�����Ϊ0X10ʱΪSPIģʽ


/** MPU9250��Ӧ���豸��ַ **/
#define MPU9250_ADDR          0x68   //mpu9250 ��ַ(�û��Լ����ݽ��߶���)
#define MPU9250_ADDR_WRITE    0xD0   //���AD0��(9��)�ӵ�,���豸��ַΪ0X68(���������λ).
#define MPU9250_ADDR_READ     0xD1   //�����V3.3,���豸��ַΪ0X69(���������λ).
							         //ע������7λ��ַ Ҫת��Ϊ8λ��ַ 0д 1��
#define MPU6500_ID1			  0X71  	//MPU6500������ID1 
#define MPU6500_ID2			  0X73  	//MPU6500������ID2

#define MAG_ADDRESS_WRITE     0x18   //������8λд��ַ 0x0c<<1



/** MPU9250�ڲ���װ��һ��AK8963������,��ַ��ID����**/
#define AK8963_ADDR				0X0C     //AK8963��7λI2C��ַ
#define AK8963_ID				0X48	//AK8963������ID (0x00�Ĵ���������)
//AK8963���ڲ��Ĵ���
#define MAG_WIA					0x00	//AK8963������ID�Ĵ�����ַ
#define MAG_ST1					0x02    //״̬�Ĵ���1  ����׼����0λΪ1 ��ȡ���Զ���0
#define MAG_CNTL1          	  	0X0A    //��������ģʽ������ת�����������Լ��
#define MAG_CNTL2            	0X0B    //��λ
//��ȡ���ݵļĴ���
#define MAG_XOUT_L		0x03   //�������������
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08

//��ָ�� ά��SPIͨѶ
#define         DUMMY        0xFF



/*************************************************************************/
/*********************** SPI����غ��� ***********************************/
/*************************************************************************/
void SPI_Commit_Init(void);

u8 SPI_FLASH_Send_Byte(u8 data); //д1byte+��
u8 SPI_FLASH_Read_Byte(void);  //��1byte �����Ϻ���


u8 MPU9250_Read_Reg(u8 reg);
u8 MPU9250_Write_Reg(u8 reg,u8 value);

/*************************************************************************/
/*********************** 9250����غ��� ***********************************/
/*************************************************************************/
void Init_MPU9250(void);
u8 SPI_FLASH_Read_ID(void);
bool Check_AK8963_Exist(void);





/*************************************************************************/
/*********************** Debug����  Linux ********************************/
/*************************************************************************/

/*�ȴ���ʱʱ��*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))


/*��Ϣ���*/
#define  FLASH_DEBUG(message, connect...)      printf("\r\nDebug: File[%s]  Line[%d]  "message"   \r\n",__FILE__, __LINE__, ##connect);                   
                                                     //�ṹΪ "    "message" "
													 //"message" �ǵ���  FLASH_DEBUG("")  ʱ��ͷ��ֵ
													 //"       " ��ԭ����ֵFLASH_DEBUG("")����û�ж����ӡ




#endif





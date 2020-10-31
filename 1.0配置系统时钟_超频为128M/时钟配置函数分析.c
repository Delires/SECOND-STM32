static void SetSysClockTo72(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

  /* ������Щʱ�� SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/

  /* Enable HSE */
   RCC->CR |= ((uint32_t)RCC_CR_HSEON);  //����HSE������ �ⲿʱ�ӿ��������Ż�����


  /* Wait till HSE is ready and if Time out is reached exit */
  /* �ȴ�HSE����  ���޾�������ʱ���� */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;  //�������RCC->CR��HSERDY��Ϊ��Ϊ1  ����& ��Ϊ1
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));  //Ϊ1 �Ҽ�ʱû��ʱ ����

//RESET=0 !RESET=1  �������λ=1�����ɹ������������
  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }

//�����ɹ�  ����FLAH  �ο�flash�����д�ֲ�
  if (HSEStatus == (uint32_t)0x01)
  {
    /*ʹ��Ԥȡַ�ó������ ARM��������ˮ�߲��� ִ��-ȡַ-Ԥȡַ   Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);//��FLASH_ACR_LATENCYλ��0  Ȼ���ٸ�ֵ
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2; //��������ACRLATENCY�������ȴ�״̬ ��ʱ�Ӿ�����
                                                 //��ΪȡַҪ�ȴ�һ�������ڣ����Ƶ�ʹ����Ҫ�ȴ�����״̬������ȴ�ʱ��̫�� ȡַ��û��ɾͽ�����һ���˾ͻᷢ������


    /* ����AHB HCLK = SYSCLK=72M */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

    /* ����APB2 PCLK2 = HCLK=72m */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

    /* ����APB1 PCLK1 = HCLK/2=36M */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

    /* �������໷PLL��ƵHSE   PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |
                                        RCC_CFGR_PLLMULL)); //16 17 21~18λ��0
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);
                                     //HSE����ƵΪʱ��Դ  HSE����Ƶ ��Ƶϵ��9��

//�������ñ�����ʹ��ǰ
    /* ʹ�� Enable PLL */
    RCC->CR |= RCC_CR_PLLON;


    /*�����PLL������־λ�Ƿ�Ϊ1  Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /*ѡ��PLL��Ϊϵͳʱ�� Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL; //ѡ��PLLΪʱ��Դ

    /* �ȴ�sysytemʱ���л���ɵı�־λ Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08) //0x08h = 1000
    {            //���óɹ�����&Ϊ 0x80
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
    /* �������ʧ�ܣ��ٴ˴���Ӵ�������� */
  }
}

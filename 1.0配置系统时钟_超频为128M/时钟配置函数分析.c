static void SetSysClockTo72(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

  /* 配置这些时钟 SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/

  /* Enable HSE */
   RCC->CR |= ((uint32_t)RCC_CR_HSEON);  //开启HSE的振荡器 外部时钟开启振荡器才会起振


  /* Wait till HSE is ready and if Time out is reached exit */
  /* 等待HSE就绪  若无就绪做超时处理 */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;  //如果就绪RCC->CR的HSERDY这为就为1  二者& 则为1
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));  //为1 且计时没超时 跳出

//RESET=0 !RESET=1  如果启动位=1启动成功，则继续进行
  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }

//启动成功  启动FLAH  参考flash闪存编写手册
  if (HSEStatus == (uint32_t)0x01)
  {
    /*使能预取址让程序进行 ARM的三步流水线操作 执行-取址-预取址   Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);//把FLASH_ACR_LATENCY位清0  然后再赋值
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2; //配置闪存ACRLATENCY的两个等待状态 由时钟决定，
                                                 //因为取址要等待一定的周期，如果频率过快就要等待两个状态。否则等待时间太短 取址还没完成就进行下一个了就会发生错误


    /* 配置AHB HCLK = SYSCLK=72M */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

    /* 配置APB2 PCLK2 = HCLK=72m */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

    /* 配置APB1 PCLK1 = HCLK/2=36M */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

    /* 配置锁相环PLL倍频HSE   PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |
                                        RCC_CFGR_PLLMULL)); //16 17 21~18位清0
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);
                                     //HSE不分频为时钟源  HSE不分频 倍频系数9倍

//所有配置必须在使能前
    /* 使能 Enable PLL */
    RCC->CR |= RCC_CR_PLLON;


    /*如果读PLL启动标志位是否为1  Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /*选择PLL作为系统时钟 Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL; //选择PLL为时钟源

    /* 等待sysytem时钟切换完成的标志位 Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08) //0x08h = 1000
    {            //配置成功二者&为 0x80
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
    /* 如果启动失败，再此处添加错误处理代码 */
  }
}

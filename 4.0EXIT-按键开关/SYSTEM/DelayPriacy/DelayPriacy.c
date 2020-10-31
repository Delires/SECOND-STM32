#include "stm32f10x.h"
#include "DelayPriacy.h"

void Delay(uint32_t Count)
{
	uint32_t i;
	for(; Count!=0; Count--)
	{
		i = 5000;
		while(i--);
	}
}




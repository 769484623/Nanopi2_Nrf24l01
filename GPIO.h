#ifndef S4418_GPIO
#define S4418_GPIO
#include "GPIO_type.h"
#include <linux/irq.h>
#include <linux/interrupt.h>
typedef struct
{
	GPIO_Pin Pin;
	unsigned Mode;
	unsigned Pull;
}GPIO_Typedef;
#define Pull_Up 1
#define Pull_Down 0
#define Input 0
#define Output 1
void GPIO_SetBit(GPIO_BASE Base,GPIO_Pin Pin);
void GPIO_ResetBit(GPIO_BASE Base,GPIO_Pin Pin);
void GPIO_Init(GPIO_BASE Base,GPIO_Typedef* Init);
void GPIO_Free(GPIO_BASE Base,GPIO_Pin Pin);
void Set_GPIO_IRQ(GPIO_BASE Base,GPIO_Pin Pin,irq_handler_t handler,unsigned long flags, const char *name);
GPIO_Pin PadToPHY_Pin(unsigned Pin);
GPIO_BASE PadToPHY_Base(unsigned Base);
unsigned PHYToPad_Pin(GPIO_Pin Pin);
unsigned PHYToPad_Base(GPIO_BASE Base);
#endif

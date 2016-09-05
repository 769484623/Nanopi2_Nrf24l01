#ifndef S4418_GPIO_TYPE
#define S4418_GPIO_TYPE
#include <mach/platform.h>
#define GPIOA_BASE PHY_BASEADDR_GPIOA
#define GPIOB_BASE PHY_BASEADDR_GPIOB
#define GPIOC_BASE PHY_BASEADDR_GPIOC
#define GPIOD_BASE PHY_BASEADDR_GPIOD
#define GPIOE_BASE PHY_BASEADDR_GPIOE
#define GPIO_Pin_0 (unsigned)1<<0
#define GPIO_Pin_1 (unsigned)1<<1
#define GPIO_Pin_2 (unsigned)1<<2
#define GPIO_Pin_3 (unsigned)1<<3
#define GPIO_Pin_4 (unsigned)1<<4
#define GPIO_Pin_5 (unsigned)1<<5
#define GPIO_Pin_6 (unsigned)1<<6
#define GPIO_Pin_7 (unsigned)1<<7
#define GPIO_Pin_8 (unsigned)1<<8
#define GPIO_Pin_9 (unsigned)1<<9
#define GPIO_Pin_10 (unsigned)1<<10
#define GPIO_Pin_11 (unsigned)1<<11
#define GPIO_Pin_12 (unsigned)1<<12
#define GPIO_Pin_13 (unsigned)1<<13
#define GPIO_Pin_14 (unsigned)1<<14
#define GPIO_Pin_15 (unsigned)1<<15
#define GPIO_Pin_16 (unsigned)1<<16
#define GPIO_Pin_17 (unsigned)1<<17
#define GPIO_Pin_18 (unsigned)1<<18
#define GPIO_Pin_19 (unsigned)1<<19
#define GPIO_Pin_20 (unsigned)1<<20
#define GPIO_Pin_21 (unsigned)1<<21
#define GPIO_Pin_22 (unsigned)1<<22
#define GPIO_Pin_23 (unsigned)1<<23
#define GPIO_Pin_24 (unsigned)1<<24
#define GPIO_Pin_25 (unsigned)1<<25
#define GPIO_Pin_26 (unsigned)1<<26
#define GPIO_Pin_27 (unsigned)1<<27
#define GPIO_Pin_28 (unsigned)1<<28
#define GPIO_Pin_29 (unsigned)1<<29
#define GPIO_Pin_30 (unsigned)1<<30
#define GPIO_Pin_31 (unsigned)1<<31

typedef unsigned GPIO_BASE;
typedef unsigned GPIO_Pin;
typedef unsigned GPIO_Command;
#endif

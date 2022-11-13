#include "gpio-board.h"
#include "bflb_core.h"
#include "bflb_gpio.h"
#include "bl602_glb.h"
#include "bl602_gpio.h"
#include "board-config.h"
#include "rtc-board.h"
#include "utilities.h"

bool reset_alredy_init = 0;
GLB_GPIO_Cfg_Type pinConfigs[32];

GpioIrqHandler *gpioIrqHandler = NULL;
void irq(int irq, void* args) {
  if(irq == 16+44) {
    if(GLB_Get_GPIO_IntStatus(RADIO_DIO_1))  {
      if(gpioIrqHandler != NULL){
        gpioIrqHandler(NULL);
      }
      GLB_GPIO_IntClear(RADIO_DIO_1, SET);
      GLB_GPIO_IntClear(RADIO_DIO_1, RESET);
    }
  } else {
  }
  bflb_irq_clear_pending(irq);
}

void GpioMcuInit( Gpio_t *obj, PinNames pin, PinModes mode, PinConfigs config, PinTypes type, uint32_t value )
{
  // Convert GPIO modes
  uint8_t gpioMode = GPIO_MODE_INPUT;
  switch(mode) {
  case PIN_INPUT:
    gpioMode = GPIO_MODE_INPUT;
    break;
  case PIN_OUTPUT:
    gpioMode = GPIO_MODE_OUTPUT;
    break;
  case PIN_ALTERNATE_FCT:
    gpioMode = GPIO_MODE_AF;
    break;
  case PIN_ANALOGIC:
    gpioMode = GPIO_MODE_ANALOG;
    break;
  }

  // Convert pull types
  uint8_t pullType = GPIO_PULL_DOWN;
  switch(type) {
  case PIN_NO_PULL: pullType = GPIO_PULL_NONE; break;
  case PIN_PULL_UP: pullType = GPIO_PULL_UP; break;
  case PIN_PULL_DOWN: pullType = GPIO_PULLDOWN; break;
  }

  obj->pin = pin;

  // Do NOT initialize RESET multiple times
  // Reset is only "written" using GpioMcuInit() instead of GpioMcuWrite()
  if(obj->pin == RADIO_RESET && reset_alredy_init) {
    if(mode == PIN_OUTPUT)
      GpioMcuWrite(obj, 0);
    else
      GpioMcuWrite(obj, 1);

    return;
  }

  pinConfigs[pin].drive = 0;
  pinConfigs[pin].smtCtrl = 1;
  pinConfigs[pin].gpioFun = GPIO_FUN_GPIO;
  pinConfigs[pin].gpioMode = gpioMode;
  pinConfigs[pin].gpioPin = pin;
  pinConfigs[pin].pullType = pullType;

  GLB_GPIO_Init(&pinConfigs[pin]);

  if(obj->pin == RADIO_RESET) {
    reset_alredy_init = true;
  }
}

void GpioMcuSetContext( Gpio_t *obj, void* context )
{
  obj->Context = context;
}

void GpioMcuSetInterrupt( Gpio_t *obj, IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler )
{
  if(irqMode != NO_IRQ) {
    GLB_GPIO_INT_TRIG_Type irqTriggerType = GLB_GPIO_INT_TRIG_POS_PULSE;
    switch (irqMode) {
    case IRQ_RISING_EDGE: irqTriggerType = GLB_GPIO_INT_TRIG_POS_PULSE; break;
    case IRQ_FALLING_EDGE: irqTriggerType = GLB_GPIO_INT_TRIG_NEG_PULSE; break;
    }

    GLB_GPIO_IntMask(obj->pin, MASK);
    GLB_Set_GPIO_IntMod(obj->pin, GLB_GPIO_INT_CONTROL_SYNC, irqTriggerType);
    bflb_irq_attach(16 + 44, irq, NULL);
    // Enable IRQ
    putreg8(1, 0x02800000UL + 0x400 + 16 + 44);
  }

  GLB_GPIO_Init(&pinConfigs[obj->pin]);

  gpioIrqHandler = irqHandler;
  GLB_GPIO_IntMask(obj->pin, UNMASK);
}

void GpioMcuRemoveInterrupt( Gpio_t *obj )
{
  GLB_GPIO_IntMask(obj->pin, MASK);
}

void GpioMcuWrite( Gpio_t *obj, uint32_t value )
{
  GLB_GPIO_Write(obj->pin, value);
}

void GpioMcuToggle( Gpio_t *obj )
{
  uint32_t value = GLB_GPIO_Read(obj->pin);
  if(value != 0) {
    GLB_GPIO_Write(obj->pin, 0);
  } else {
    GLB_GPIO_Write(obj->pin, 1);
  }
}

uint32_t GpioMcuRead( Gpio_t *obj )
{
  if(obj->pin == RADIO_BUSY) {
    uint32_t ret = GLB_GPIO_Read(RADIO_BUSY);
    return ret;
  }
  return 0;
}
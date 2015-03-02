#include "header.h"
#include "declarations.h"

irqreturn_t ser_interrupt(int irq, void *dev_id)
{
	unsigned char byte;
	byte = inb(IIR);
	/*this was intended for serial port*/
	if (!(byte & ERBFI))
		return IRQ_NONE;
	tasklet_schedule(&tasklet);
	return IRQ_HANDLED;
}

void ser_tasklet(unsigned long data)
{
	unsigned char byte;
	struct ScullDev *lsculldev;
	struct ScullQset *lscullqset;
	lsculldev = &sculldev[0];
	lscullqset = lsculldev->qset;
	/*read data*/
	byte = inb(RBR);
	memcpy(lscullqset->data[0], &byte, sizeof(unsigned char));
	lsculldev->data_size = 1;
	wake_up_interruptible(&lsculldev->dev_wait_q);
}

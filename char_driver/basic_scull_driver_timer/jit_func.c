#include"header.h"
#include"declarations.h"

void jit_func(unsigned long arg)
{
	int i;
	struct jit_data *data = (struct jit_data*)arg;
	if (!data->buf) {
		wake_up_interruptible(&data->waitq);	
		return;
	}
	i = sprintf(data->buf,"jiffies in read = %li ,jiffies in write = %li\n",data->r_time,data->w_time);
	data->count += i;
	i = sprintf(data->buf + i,"in atomic context %d , in interrupt context %lu\n",in_atomic(),in_interrupt());
	data->count += i;
	data->flag = 1;
	wake_up_interruptible(&data->waitq);	
}

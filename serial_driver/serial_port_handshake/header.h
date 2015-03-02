#ifndef HEADER_H
#define HEADER_H

#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/cdev.h>
#include<linux/moduleparam.h>
#include<linux/stat.h>
#include<linux/uaccess.h>
#include<linux/semaphore.h>
#include<linux/ioport.h>
#include<linux/bitops.h>
#include<linux/delay.h>

#ifndef DEBUG
#define DEBUG
#endif

#define MAJOR_NUM	0
#define MINOR_NUM	0
#define DEV_NAME	"my_driver"
#define NOD		1

#define QSET_SIZE	3
#define QUANTUM_SIZE	1
#define DATA_SIZE	0
#define DEVICE_SIZE	8

#define	NUM_REGS	8
#define	PORT_ADDRESS	0x3f8
#define RBR		PORT_ADDRESS+0
#define THR		PORT_ADDRESS+0
#define IER		PORT_ADDRESS+1
#define IIR		PORT_ADDRESS+2
#define FCR		PORT_ADDRESS+2
#define LCR		PORT_ADDRESS+3
#define MCR		PORT_ADDRESS+4
#define LSR		PORT_ADDRESS+5
#define MSR		PORT_ADDRESS+6
#define SCR		PORT_ADDRESS+7
#define DLL		PORT_ADDRESS+0
#define DLM		PORT_ADDRESS+1

#define LCR_WLS_5		0x00
#define LCR_WLS_6		0x01
#define LCR_WLS_7		0x02
#define LCR_WLS_8		0x03
#define LCR_STB			0x04
#define LCR_PEN			0x08
#define LCR_EPS			0x10
#define LCR_DLAB		0x80

#define LSR_DR			0x00
#define LSR_THRE		0x20
#define LSR_TEMT		0x40

#define MSR_CTS			0x10
#define MSR_DSR 		0x20

#define MCR_DTR			0x10
#define MCR_RTS			0x02

#endif

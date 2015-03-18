
/*******************************************************************
*
* CAUTION: This file is automatically generated by libgen.
* Version: Xilinx EDK 14.4 EDK_P.49d
* DO NOT EDIT.
*
* Copyright (c) 1995-2012 Xilinx, Inc.  All rights reserved.

* 
* Description: WARP/XilNet Configuration Parameters
*
*******************************************************************/

#ifndef _XILNET_CONFIG_H
#define _XILNET_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif


/***************************** Include Files *********************************/
#include <xilnet_xilsock.h>
#include <xilnet_eth.h>
#include <xparameters.h>

#include <xllfifo.h>

/*************************** Constant Definitions ****************************/

// Xilnet Global Defines
#define XILNET_NUM_ETH_DEVICES                   2                             // Define the number of Ethernet devices

// Define UDP connections
#define XILNET_MAX_UDP_CONNS                     5                             // maximum no of open udp conns
#define NO_OF_XILSOCKS                           XILNET_MAX_UDP_CONNS          // Number of sockets per Ethernet Device

// Define Ethernet interface types
#define XILNET_AXI_DMA_INF                       1
#define XILNET_AXI_FIFO_INF                      2
#define XILNET_TEMAC_INF                         3

// Define which interfaces are used
#define XILNET_AXI_FIFO_INF_USED


/************************* Ethernet 0 Definitions ****************************/

// Ethernet device 0
#define ETH_A_MAC                                XILNET_ETH_0                  // External name of Ethernet device
#define XILNET_ETH_0                             0                             // Internal name of Ethernet device

#define XILNET_ETH_0_INF_DEVICE_ID               0
#define XILNET_ETH_0_NUM_RECV_BUF                0
#define XILNET_ETH_0_BUF_SIZE                    0
#define XILNET_ETH_0_RXBD_CNT                    0                             // Number of RxBDs to use
#define XILNET_ETH_0_TXBD_CNT                    0                             // Number of TxBDs to use
#define XILNET_ETH_0_RXBD_SPACE_BYTES            0
#define XILNET_ETH_0_TXBD_SPACE_BYTES            0


/************************* Ethernet 1 Definitions ****************************/

// Ethernet device 1
#define ETH_B_MAC                                XILNET_ETH_1                  // External name of Ethernet device
#define XILNET_ETH_1                             1                             // Internal name of Ethernet device

#define XILNET_ETH_1_INF_DEVICE_ID               XPAR_ETH_B_FIFO_DEVICE_ID
#define XILNET_ETH_1_NUM_RECV_BUF                1
#define XILNET_ETH_1_BUF_SIZE                    1514

// Variables for ETH 1
extern XLlFifo              ETH_1_FIFO_Instance;

// Buffers for ETH 1
extern unsigned int recvBuffer_eth_1[XILNET_ETH_1_NUM_RECV_BUF*((XILNET_ETH_1_BUF_SIZE+3)/4)];
extern unsigned int sendBuffer_eth_1[(XILNET_ETH_1_BUF_SIZE+3)/4];


/*********************** Global Variable Definitions *************************/

// Ethernet Device Structure
extern xilnet_eth_device eth_device[XILNET_NUM_ETH_DEVICES];

/*************************** Function Prototypes *****************************/

void xilnet_init_eth_device_struct(unsigned int);

#endif

/*****************************************************************************/

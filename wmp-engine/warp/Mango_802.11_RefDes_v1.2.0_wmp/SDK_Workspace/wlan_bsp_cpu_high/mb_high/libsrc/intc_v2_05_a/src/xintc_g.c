
/*******************************************************************
*
* CAUTION: This file is automatically generated by libgen.
* Version: Xilinx EDK 14.4 EDK_P.49d
* DO NOT EDIT.
*
* Copyright (c) 1995-2012 Xilinx, Inc.  All rights reserved.

* 
* Description: Driver configuration
*
*******************************************************************/

#include "xparameters.h"
#include "xintc.h"


extern void XNullHandler (void *);

/*
* The configuration table for devices
*/

XIntc_Config XIntc_ConfigTable[] =
{
	{
		XPAR_MB_HIGH_INTC_DEVICE_ID,
		XPAR_MB_HIGH_INTC_BASEADDR,
		XPAR_MB_HIGH_INTC_KIND_OF_INTR,
		XPAR_MB_HIGH_INTC_HAS_FAST,
		XPAR_MB_HIGH_INTC_IVAR_RESET_VALUE,
		XIN_SVC_SGL_ISR_OPTION,
		{
			{
				XNullHandler,
				(void *) XNULL
			},
			{
				XNullHandler,
				(void *) XNULL
			},
			{
				XNullHandler,
				(void *) XNULL
			},
			{
				XNullHandler,
				(void *) XNULL
			},
			{
				XNullHandler,
				(void *) XNULL
			},
			{
				XNullHandler,
				(void *) XNULL
			},
			{
				XNullHandler,
				(void *) XNULL
			},
			{
				XNullHandler,
				(void *) XNULL
			},
			{
				XNullHandler,
				(void *) XNULL
			},
			{
				XNullHandler,
				(void *) XNULL
			}
		}

	}
};

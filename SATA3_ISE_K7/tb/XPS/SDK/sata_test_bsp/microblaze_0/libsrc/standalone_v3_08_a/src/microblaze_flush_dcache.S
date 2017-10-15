/******************************************************************************
* Copyright (c) 2008 Xilinx, Inc.  All rights reserved. 
* 
* Xilinx, Inc. 
* XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
* COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS 
* ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
* STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
* IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
* FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION. 
* XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
* THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
* ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
* FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
* AND FITNESS FOR A PARTICULAR PURPOSE. 
*
* microblaze_flush_dcache()
*    
*    Flush the entire DCache 
*
* $Id: microblaze_flush_dcache.S,v 1.1.2.1 2011/05/17 04:37:29 sadanan Exp $
*
*******************************************************************************/

#include "xparameters.h"

#define MICROBLAZE_MSR_DCACHE_ENABLE        0x00000080
#define MICROBLAZE_MSR_INTR_ENABLE          0x00000002
    
#ifndef XPAR_MICROBLAZE_DCACHE_LINE_LEN
#define XPAR_MICROBLAZE_DCACHE_LINE_LEN   1
#endif
    
	.text
	.globl	microblaze_flush_dcache
	.ent	microblaze_flush_dcache
	.align	2

microblaze_flush_dcache:

        andi    r5, r5, -(4 * XPAR_MICROBLAZE_DCACHE_LINE_LEN)  /* Align to cache line */

    	addik	r6, r5, XPAR_MICROBLAZE_DCACHE_BYTE_SIZE        /* Compute end */
        andi    r6, r6, -(4 * XPAR_MICROBLAZE_DCACHE_LINE_LEN)  /* Align to cache line */
    
L_start:
	wdc.flush r5, r0                                        /* Flush the Cache */
    
    	cmpu	r18, r5, r6                                     /* Are we at the end? */
	blei	r18, L_done                                     

     	brid	L_start                                         /* Branch to the beginning of the loop */
	addik	r5, r5, (XPAR_MICROBLAZE_DCACHE_LINE_LEN * 4)   /* Increment the addrees by 4 (delay slot) */

L_done:	
	rtsd	r15, 8                                          /* Return */
        nop
	.end	microblaze_flush_dcache

	
  

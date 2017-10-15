/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
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
 */

/*
 * helloworld.c: simple test application
 */

#include <stdio.h>
#include "platform.h"
#include "xuartlite.h"
#include "xuartlite_l.h"
#include "xparameters.h"
#include "xio.h"
#include "global.h"
#include "console.h"





/*XPAR_TEST_LOGIC_0_BASEADDR      addr
  XPAR_TEST_LOGIC_0_BASEADDR+4    cmd_R_W;
  XPAR_TEST_LOGIC_0_BASEADDR+8    W_data;
  XPAR_TEST_LOGIC_0_BASEADDR+12   R_data;
*/





static void PrintTitleAndVersion(void);

int main()
{
    u32 i;
    // show program title and version
    PrintTitleAndVersion();

    //reset sata controller
    Xil_Out32(SATA_BASEADDR + (0),1);
    for (i=0 ; i < 0xFFFFF; i++);
    Xil_Out32(SATA_BASEADDR + (0),0);

    RunConsole();
    return 0;
}

/****************************************************************************
     Function: PrintTitleAndVersion
     Engineer: Shameerudheen P T
        Input: none
       Output: none
  Description: print program title and version
Date           Initials    Description
30-Jul-2013    SPT          Initial
****************************************************************************/
static void PrintTitleAndVersion(void)
{
	xil_printf("%s\n\r", SATA_TEST_PROG_TITLE);
	xil_printf("%s\n\r", SATA_TEST_PROG_VERSION);
	xil_printf("\n\r");
}

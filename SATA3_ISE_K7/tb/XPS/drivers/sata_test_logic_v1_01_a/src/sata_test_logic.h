/*****************************************************************************
* Filename:          F:\SATA\sata_phy_board_test\sata_phy_ready_to_download\XPS/drivers/sata_test_logic_v1_01_a/src/sata_test_logic.h
* Version:           1.01.a
* Description:       sata_test_logic Driver Header File
* Date:              Mon Dec 30 12:22:06 2013 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#ifndef SATA_TEST_LOGIC_H
#define SATA_TEST_LOGIC_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xil_io.h"

/************************** Constant Definitions ***************************/


/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : user logic slave module register 0
 */
#define SATA_TEST_LOGIC_USER_SLV_SPACE_OFFSET (0x00000000)
#define SATA_TEST_LOGIC_SLV_REG0_OFFSET (SATA_TEST_LOGIC_USER_SLV_SPACE_OFFSET + 0x00000000)

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/

/**
 *
 * Write a value to a SATA_TEST_LOGIC register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the SATA_TEST_LOGIC device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void SATA_TEST_LOGIC_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define SATA_TEST_LOGIC_mWriteReg(BaseAddress, RegOffset, Data) \
 	Xil_Out32((BaseAddress) + (RegOffset), (Xuint32)(Data))

/**
 *
 * Read a value from a SATA_TEST_LOGIC register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the SATA_TEST_LOGIC device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 SATA_TEST_LOGIC_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define SATA_TEST_LOGIC_mReadReg(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (RegOffset))


/**
 *
 * Write/Read 32 bit value to/from SATA_TEST_LOGIC user logic slave registers.
 *
 * @param   BaseAddress is the base address of the SATA_TEST_LOGIC device.
 * @param   RegOffset is the offset from the slave register to write to or read from.
 * @param   Value is the data written to the register.
 *
 * @return  Data is the data from the user logic slave register.
 *
 * @note
 * C-style signature:
 * 	void SATA_TEST_LOGIC_mWriteSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Value)
 * 	Xuint32 SATA_TEST_LOGIC_mReadSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define SATA_TEST_LOGIC_mWriteSlaveReg0(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (SATA_TEST_LOGIC_SLV_REG0_OFFSET) + (RegOffset), (Xuint32)(Value))

#define SATA_TEST_LOGIC_mReadSlaveReg0(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (SATA_TEST_LOGIC_SLV_REG0_OFFSET) + (RegOffset))

/************************** Function Prototypes ****************************/


/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the SATA_TEST_LOGIC instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus SATA_TEST_LOGIC_SelfTest(void * baseaddr_p);
/**
*  Defines the number of registers available for read and write*/
#define TEST_AXI_LITE_USER_NUM_REG 1


#endif /** SATA_TEST_LOGIC_H */

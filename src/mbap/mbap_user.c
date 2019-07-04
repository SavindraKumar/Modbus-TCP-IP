//! @addtogroup ModbusTCPUserFunction
//! @brief User functions
//! @{
//!
//****************************************************************************/
//! @file mbap_user.c
//! @brief User functions for reading/writing modbus data
//! @author Savindra Kumar(savindran1989@gmail.com)
//! @bug No known bugs.
//!
//****************************************************************************/
//                           Includes
//****************************************************************************/
//standard header files
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//user defined files
#include "mbap_user.h"
#include "mbap_debug.h"
#include "mbap_conf.h"

//****************************************************************************/
//                           Defines and typedefs
//****************************************************************************/

//****************************************************************************/
//                           external variables
//****************************************************************************/
int16_t g_sInputRegsBuf[MAX_INPUT_REGISTERS]                = {1, 2, 3};
int16_t g_sHoldingRegsBuf[MAX_HOLDING_REGISTERS]            = {5, 6, 7};
uint8_t g_ucDiscreteInputsBuf[DISCRETE_INPUT_BUF_SIZE]      = {0xef};
uint8_t g_ucCoilsBuf[COILS_BUF_SIZE]                        = {5};
int16_t g_sHoldingRegsLowerLimitBuf[MAX_HOLDING_REGISTERS]  = {0, 0, 0};
int16_t g_sHoldingRegsHigherLimitBuf[MAX_HOLDING_REGISTERS] = {200, 200, 200};

//****************************************************************************/
//                           Local variables
//****************************************************************************/

//****************************************************************************/
//                           Local Functions
//****************************************************************************/
//
//! @brief Read discrete inputs from user data
//! @param[in]   usStartAddress Discrete inputs start address
//! @param[in]   sNumOfData     Number of discrete inputs to read
//! @param[out]  pucRecBuf      Receive buffer holds read discrete inputs
//! @return      None
//
static void ReadDiscreteInputs(uint16_t usStartAddress,
	                           int16_t sNumOfData,
	                           uint8_t *pucRecBuf);

//
//! @brief Read coils from user data
//! @param[in]   usStartAddress Coils start address
//! @param[in]   sNumOfData     Number of coils to read
//! @param[out]  pucRecBuf      Receive buffer holds read coils
//! @return      None
//
static void ReadCoils(uint16_t usStartAddress,
	                  int16_t sNumOfData,
	                  uint8_t *pucRecBuf);

//
//! @brief Read input registers from user data
//! @param[in]   usStartAddress Input registers start address
//! @param[in]   sNumOfData     Number of input registers to read
//! @param[out]  pucRecBuf      Receive buffer holds input registers
//! @return      None
//
static void ReadInputRegisters(uint16_t usStartAddress,
	                           uint16_t usNumOfData,
	                           uint8_t *pucRecBuf);

//
//! @brief Read holding registers from user data
//! @param[in]   usStartAddress Holding registers start address
//! @param[in]   sNumOfData     Number of holding registers to read
//! @param[out]  pucRecBuf      Receive buffer holds holding registers
//! @return      None
static void ReadHoldingRegisters(uint16_t usStartAddress,
	                             uint16_t usNumOfData,
	                             uint8_t *pucRecBuf);


//****************************************************************************/
//                    G L O B A L  F U N C T I O N S
//****************************************************************************/
void mu_Init(void)
{
    ModbusData_t ModbusData;

    //Init modbus data
    ModbusData.usInputRegisterStartAddress   = INPUT_REGISTER_START_ADDRESS;
    ModbusData.usMaxInputRegisters           = MAX_INPUT_REGISTERS;
    ModbusData.usHoldingRegisterStartAddress = HOLDING_REGISTER_START_ADDRESS;
    ModbusData.usMaxHoldingRegisters         = MAX_HOLDING_REGISTERS;
    ModbusData.psHoldingRegisterLowerLimit   = g_sHoldingRegsLowerLimitBuf;
    ModbusData.psHoldingRegisterHigherLimit  = g_sHoldingRegsHigherLimitBuf;
    ModbusData.usDiscreteInputStartAddress   = DISCRETE_INPUTS_START_ADDRESS;
    ModbusData.usMaxDiscreteInputs           = MAX_DISCRETE_INPUTS;
    ModbusData.usCoilsStartAddress           = COILS_START_ADDRESS;
    ModbusData.usMaxCoils                    = MAX_COILS;
	ModbusData.ptfnReadInputRegisters        = ReadInputRegisters;
	ModbusData.ptfnReadHoldingRegisters      = ReadHoldingRegisters;
	ModbusData.ptfnReadDiscreteInputs        = ReadDiscreteInputs;
	ModbusData.ptfnReadCoils                 = ReadCoils;

	//pass modbus data data pointer to modbus tcp application
    mbap_DataInit(&ModbusData);
}

//****************************************************************************/
//                           L O C A L  F U N C T I O N S
//****************************************************************************/
static void ReadInputRegisters(uint16_t usStartAddress,
	                           uint16_t usNumOfData,
	                           uint8_t *pucRecBuf)
{
	MBT_DEBUGF(MBT_CONF_DEBUG_LEVEL_MSG, "Read Input Registers User function\r\n");

	while (usNumOfData > 0)
	{
		*pucRecBuf++ = (uint8_t)(g_sInputRegsBuf[usStartAddress] >> 8);
		*pucRecBuf++ = (uint8_t)(g_sInputRegsBuf[usStartAddress] & 0xFF);
		usStartAddress++;
		usNumOfData--;
	}
}//end ReadInputRegisters

static void ReadDiscreteInputs(uint16_t usStartAddress,
	                           int16_t sNumOfData,
	                           uint8_t *pucRecBuf)
{
	MBT_DEBUGF(MBT_CONF_DEBUG_LEVEL_MSG, "Read Discrete Inputs User function\r\n");

    while (sNumOfData > 0)
	{
    	uint16_t  usTmpBuf;
    	uint16_t  usMask;
    	uint16_t  usByteOffset;
    	uint16_t  usNPreBits;

    	usByteOffset = usStartAddress  / 8 ;
    	usNPreBits   = usStartAddress - usByteOffset * 8;
    	usMask       = (1 << sNumOfData)  - 1 ;
    	usTmpBuf     = g_ucDiscreteInputsBuf[usByteOffset];
    	usTmpBuf    |= g_ucDiscreteInputsBuf[usByteOffset + 1] << 8;
    	// throw away unneeded bits
    	usTmpBuf     = usTmpBuf >> usNPreBits;
    	// mask away bits above the requested bitfield
    	usTmpBuf     = usTmpBuf & usMask;
    	*pucRecBuf++ = (uint8_t)usTmpBuf;

    	sNumOfData     = sNumOfData - 8;
    	usStartAddress = usStartAddress - 8;
	}
}//end ReadDiscreteInputs

static void ReadCoils(uint16_t usStartAddress,
	                  int16_t sNumOfData,
	                  uint8_t *pucRecBuf)
{
	MBT_DEBUGF(MBT_CONF_DEBUG_LEVEL_MSG, "Read Coils User function\r\n");

	while (sNumOfData > 0)
	{
		uint16_t  usTmpBuf;
		uint16_t  usMask;
		uint16_t  usByteOffset;
		uint16_t  usNPreBits;

		usByteOffset = usStartAddress / 8;
		usNPreBits = usStartAddress - usByteOffset * 8;
		usMask = (1 << sNumOfData) - 1;
		usTmpBuf  = g_ucCoilsBuf[usByteOffset];
		usTmpBuf |= g_ucCoilsBuf[usByteOffset + 1] << 8;
		// throw away unneeded bits
		usTmpBuf = usTmpBuf >> usNPreBits;
		// mask away bits above the requested bitfield
		usTmpBuf = usTmpBuf & usMask;
		*pucRecBuf++ = (uint8_t)usTmpBuf;

		sNumOfData = sNumOfData - 8;
	}
}//end ReadCoils

static void ReadHoldingRegisters(uint16_t usStartAddress,
	                             uint16_t usNumOfData,
	                             uint8_t *pucRecBuf)
{
	MBT_DEBUGF(MBT_CONF_DEBUG_LEVEL_MSG, "Read Holding Registers User function\r\n");

	while (usNumOfData > 0)
	{
		*pucRecBuf++ = (uint8_t)(g_sHoldingRegsBuf[usStartAddress] >> 8);
		*pucRecBuf++ = (uint8_t)(g_sHoldingRegsBuf[usStartAddress] & 0xFF);
		usStartAddress++;
		usNumOfData--;
	}
}//end ReadHoldingRegisters
//****************************************************************************/
//                             End of file
//****************************************************************************/
/** @}*/

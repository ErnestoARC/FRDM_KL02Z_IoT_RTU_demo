/*! @file : sdk_hal_i2c0.c
 * @author  Ernesto Andres Rincon Cruz
 * @version 1.0.0
 * @date    11/01/2021
 * @brief   Driver for I2C0
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "sdk_hal_i2c0.h"
#include "fsl_i2c.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/


/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
 /*--------------------------------------------*/
status_t i2c0Inicializar(uint32_t baud_rate) {
	i2c_master_config_t masterConfig;
//	status_t status;

	I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = baud_rate;

    I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(I2C0_CLK_SRC));
	return(kStatus_Success);
}

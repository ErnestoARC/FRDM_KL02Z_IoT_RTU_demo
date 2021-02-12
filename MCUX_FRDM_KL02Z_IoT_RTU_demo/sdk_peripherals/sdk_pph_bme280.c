/*! @file : sdk_pph_bme280.c
 * @author  Ernesto Andres Rincon Cruz
 * @version 1.0.0
 * @date    9/02/2021
 * @brief   Driver para sensor de temperatura BME280
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "sdk_pph_bme280.h"
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
//------------------------------------
 status_t bme280WhoAmI(void) {
	status_t status;
	uint8_t i2c_data;

	status = i2c1MasterReadByte(&i2c_data, 1, BME280_ADDRESS, BME850_REG_ID);

	if (status == kStatus_Success) {
		if (i2c_data == 0x60)
			return (kStatus_Success);
		else
			return (kStatus_Fail);
	} else {
		return (status);
	}
}
 //------------------------------------
 status_t bme280Init(void){
 	status_t status;
 	uint8_t i2c_data;

 	i2c_data = 0x03;
 	status = i2c1MasterWriteByte(&i2c_data, 1, BME280_ADDRESS, BME850_REG_CTRL_MEAS);

 	if(status!=kStatus_Success)
 		return(status);

 	return(kStatus_Success);
 }
 //------------------------------------
status_t bme280ReadData(bme280_data_t *data) {
	status_t status;
	uint8_t i2c_data[8];

	status = i2c1MasterReadByte(&i2c_data[0], 8, BME280_ADDRESS, BME850_REG_PRESS_MSB);
	if (status == kStatus_Success) {
		data->presion = (uint32_t)(i2c_data[0] << 16);
		data->presion |= (uint32_t)(i2c_data[1] << 8);
		data->presion |= (uint32_t)(i2c_data[2]);

		data->temperatura = (uint32_t)(i2c_data[3] << 16);
		data->temperatura |= (uint32_t)(i2c_data[4] << 8);
		data->temperatura |= (uint32_t)(i2c_data[5]);

		data->humedad |= (uint32_t)(i2c_data[6] << 8);
		data->humedad |= (uint32_t)(i2c_data[7]);

		return (kStatus_Success);
	}else{
		return (status);
	}
}


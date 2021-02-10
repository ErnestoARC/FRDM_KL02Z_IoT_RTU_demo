/*! @file : sdk_hal_adc.c
 * @author  Ernesto Andres Rincon Cruz
 * @version 1.0.0
 * @date    10/02/2021
 * @brief   Driver para ADC
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "sdk_hal_adc.h"
#include "fsl_adc16.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC16_BASE 			ADC0
#define DEMO_ADC16_CHANNEL_GROUP 	0U

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
status_t adcInit(void){
	adc16_config_t adc16ConfigStruct;
	    /*
	     * adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
	     * adc16ConfigStruct.clockSource = kADC16_ClockSourceAsynchronousClock;
	     * adc16ConfigStruct.enableAsynchronousClock = true;
	     * adc16ConfigStruct.clockDivider = kADC16_ClockDivider8;
	     * adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;
	     * adc16ConfigStruct.longSampleMode = kADC16_LongSampleDisabled;
	     * adc16ConfigStruct.enableHighSpeed = false;
	     * adc16ConfigStruct.enableLowPower = false;
	     * adc16ConfigStruct.enableContinuousConversion = false;
	     */
	    ADC16_GetDefaultConfig(&adc16ConfigStruct);	//Obtiene configuracion por defecto
	    ADC16_Init(ADC0, &adc16ConfigStruct);	//inicializa ADC con la configuración previa
	    ADC16_EnableHardwareTrigger(ADC0, false); //configura tipo de inicio conversión "manual"

	    return(kStatus_Success);
 }

status_t adcIniciarLectura(uint8_t canal_adc, uint32_t *resultado_adc) {
	adc16_channel_config_t adc16ChannelConfigStruct;

	adc16ChannelConfigStruct.channelNumber = canal_adc; //Selecciona canal ADC a utilizar
	adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;//Deshabilita IRQ por ADC
	ADC16_SetChannelConfig(ADC0, DEMO_ADC16_CHANNEL_GROUP,
			&adc16ChannelConfigStruct);	//establece canal e inicia lectura ADC

	//Espera a terminar proceso de conversor ADC
	while (0U	== (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(DEMO_ADC16_BASE,	DEMO_ADC16_CHANNEL_GROUP))) {
	}

	*resultado_adc = ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);

	return (kStatus_Success);
}

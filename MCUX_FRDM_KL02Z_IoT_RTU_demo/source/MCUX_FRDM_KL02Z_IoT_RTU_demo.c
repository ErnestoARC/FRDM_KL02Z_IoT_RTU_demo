/*! @file : MCUX_FRDM_KL02Z_IoT_RTU_demo.c
 * @author  Ernesto Andres Rincon Cruz
 * @version 1.0.0
 * @date    8/01/2021
 * @brief   Funcion principal main
 * @details
 *			v0.1 dato recibido por puerto COM es contestado en forma de ECO
 *			v0.2 dato recibido por puerto COM realiza operaciones especiales
 *					A/a=invierte estado de LED conectado en PTB10
 *					v=apaga LED conectado en PTB7
 *					V=enciende LED conectado en PTB7
 *					r=apaga LED conectado en PTB6
 *			v0.3 nuevo comando por puerto serial para prueba de MMA8451Q
 *					M=detecta acelerometro MM8451Q en bus I2C0
 *
 *
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL02Z4.h"
#include "fsl_debug_console.h"

#include "sdk_hal_uart0.h"
#include "sdk_hal_gpio.h"
#include "sdk_hal_i2c0.h"
#include "sdk_hal_i2c1.h"
#include "sdk_hal_adc.h"

#include "sdk_mdlw_leds.h"
#include "sdk_pph_mma8451Q.h"
#include "sdk_pph_ec25au.h"
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
uint8_t mensaje_de_texto[]="Hola desde EC25";
uint8_t ec25_detectado=0;
uint8_t mma8451Q_detectado=0;

uint8_t adc_base_de_tiempo=0;
/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
void waytTime(void) {
	uint32_t tiempo = 0xFFFFF;
	do {
		tiempo--;
	} while (tiempo != 0x0000);
}


/*
 * @brief   Application entry point.
 */
int main(void) {
	uint8_t estado_actual_ec25;
	uint32_t dato_adc;

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    BOARD_InitDebugConsole();
#endif

    printf("Inicializa UART0:");
    //inicializa puerto UART0 y solo avanza si es exitoso el proceso
    if(uart0Inicializar(115200)!=kStatus_Success){	//115200bps
    	printf("Error");
    	return 0 ;
    };
    printf("OK\r\n");

    printf("Inicializa I2C0:");
    //inicializa puerto I2C0 y solo avanza si es exitoso el proceso
    if(i2c0MasterInit(100000)!=kStatus_Success){	//100kbps
    	printf("Error");
    	return 0 ;
    }
    printf("OK\r\n");

    printf("Inicializa I2C1:");
    //inicializa puerto I2C1 y solo avanza si es exitoso el proceso
    if(i2c1MasterInit(100000)!=kStatus_Success){	//100kbps
    	printf("Error");
    	return 0 ;
    }
    printf("OK\r\n");

    //inicializa conversor analogo a Digital
    //Se debe usar  PinsTools para configurar los pines que van a ser analogicos
    if(adcInit()!=kStatus_Success){
    	return 0 ;
    }

    printf("Detectando MMA8451Q:");

    //LLamado a funcion que indeitifica acelerometro MMA8451Q
    if (mma8451QWhoAmI() == kStatus_Success){
    	printf("OK\r\n");
    	(void)mma8451QInit();	//inicializa acelerometro MMA8451Q
    }

    printf("Inicializa modem EC25\r\n");
    //inicializa todas las funciones necesarias para trabajar con el modem EC25
    ec25Inicializacion();

    printf("Enviando mensaje de texto por modem EC25\r\n");
    ec25EnviarMensajeDeTexto(&mensaje_de_texto[0], sizeof(mensaje_de_texto));

	//Ciclo infinito encendiendo y apagando led verde
	//inicia el SUPERLOOP
    while(1) {
    	waytTime();		//base de tiempo fija aproximadamente 200ms

    	adc_base_de_tiempo++;//incrementa base de tiempo para tomar una lectura ADC
    	if(adc_base_de_tiempo>10){	// >10 equivale aproximadamente a 2s
    		adc_base_de_tiempo=0;	//reinicia contador de tiempo
    		adcTomarCaptura(PTB8_ADC0_SE11_CH14, &dato_adc);	//inicia lectura por ADC y guarda en variable dato_adc
    		printf("ADC PTB8:%d\r\n",dato_adc);	//imprime resultado ADC
    	}

		estado_actual_ec25 = ec25Polling();	//actualiza maquina de estados encargada de avanzar en el proceso interno del MODEM
											//retorna el estado actual de la FSM

    	switch(estado_actual_ec25){
    	case kFSM_RESULTADO_ERROR:
    		toggleLedRojo();
    		apagarLedVerde();
    		apagarLedAzul();
    		break;

    	case kFSM_RESULTADO_EXITOSO:
    		apagarLedRojo();
    		toggleLedVerde();
    		apagarLedAzul();
    		break;

    	case kFSM_RESULTADO_ERROR_RSSI:
    		toggleLedRojo();
    		apagarLedVerde();
    		toggleLedAzul();
    		break;

    	default:
    		apagarLedRojo();
    		apagarLedVerde();
    		toggleLedAzul();
    		break;
    	}
    }

    return 0 ;
}

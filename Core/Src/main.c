/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <malloc.h>
#include "BME280_STM32.h"
#include <uart_reception.h>
#include <task_handling.h>
#include <data_buffer.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
int __io_putchar(int ch)
{
 // Write character to ITM ch.0
 ITM_SendChar(ch);
 return(ch);
}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_rx;
DMA_HandleTypeDef hdma_i2c1_tx;

I2S_HandleTypeDef hi2s2;
I2S_HandleTypeDef hi2s3;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart6_rx;
DMA_HandleTypeDef hdma_usart6_tx;

/* USER CODE BEGIN PV */
/***********************************	RPM variables	***********************************/
volatile uint32_t pulsos_hall = 0;
volatile bool hall_polarity=0;

volatile float rpm_motor=0,rpm_freno=0, rpm_freno_media = 0.0f;

volatile float suma_rpm_freno = 0.0f;
volatile int contador_rpm_freno = 0;

volatile bool motor_stopped = 0;
volatile bool freno_stopped = 0;

uint32_t tiemout_motor_stopped = 0;

volatile bool polarity_encoder=0,polarity_encoder_b=0;

volatile uint32_t valor1_hall = 0,valor2_hall = 0,periodo_hall = 0,overflow_count_hall = 0, overflow_count_encoder = 0;

volatile uint32_t periodo_encoder = 0;

volatile uint32_t valor1_encoder_a = 0,valor2_encoder_a = 0;

//uint32_t valor1_encoder_b,valor2_encoder_b;
/***********************************	RPM variables	***********************************/

/***********************************	DS18B20 variables	***********************************/

typedef enum {START_MEASURE, PRESENCE_PULSE_1, SKIPPING_ROM1, REQUESTING_CONVERSION, PRESENCE_PULSE_2, SKIPPING_ROM2, REQUEST_TEMP, RECEIVING_BYTE_LSB,  RECEIVING_BYTE_MSB, FINISHED}DS18B20_state_t;

volatile bool flag_uart6_sent = 0;
volatile bool flag_timer = 0;

volatile int presence = 0;
uint8_t RxData[8], RxData_LSB[8], RxData_MSB[8], Temp_LSB = 0, Temp_MSB = 0;
int16_t Temp = 0;
float Temperature_DS18B20 = 0;
uint8_t Tx_PRESENCE_PULSE[] = {0xF0}, Rx_PRESENCE_PULSE[] = {0x00};
uint8_t data_global = 0xF0;

uint8_t FF_byte[] = {0xFF};
uint8_t C0_byte[] = {0xC0};


uint32_t delay_ds18b20 = 0;
DS18B20_state_t DS18B20_state = START_MEASURE, *DS18B20_state_ptr = &DS18B20_state;

/***********************************	DS18B20 variables	***********************************/
volatile Bool flag_tx_sent = 0;
extern volatile Bool flag_message_copied;
extern volatile uint16_t overflow_start, head;

extern volatile char rx_buffer[BUFFER_SIZE]; //buffer unico de recepcion
extern char tx_buffer[BUFFER_SIZE]; //buffer de tranmisión que al menos para la conexion es unico para la transmitir datos
extern char message_buffer[BUFFER_SIZE]; // Buffer para almacenar un mensaje completo

extern Bool client_connected;
extern com_state_wifi_card* current_wifi_com_status;
extern com_state_wifi_card com_wifi_card_values[];


extern Bool flag_read_bme280, flag_cipsend, flag_sample_sending;

float Temperature_BME280 = 0, Pressure_BME280 = 0, Humidity_BME280 = 0;

uint32_t time_tx = 0, delay_connection = 0, delay_bme280 = 0;
int tx_buffer_size = 0;

volatile bool flag_rx_bme280 = 0;
extern BME280_state_t BME280_state;
/*********	 Variables for sample sending	*********/
uint32_t data_timeout = 0, time_sample = 0, time_init = 0;
size_t number_of_DataRecords = 0, package_size = 0;
uint8_t* buffer_to_send;

struct DataRecord *sample;
const struct DataRecord *bundle;
/*********	 Variables for sample sending	*********/

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if(Size == 128){ // Poner el tamaño del buffer a mano
		overflow_start = head;
	}
	else if (Size > 0) {
		memset(message_buffer, 0, BUFFER_SIZE);
    	if(overflow_start > Size){
    		copy_message(overflow_start, BUFFER_SIZE, 1);
    		head = 0;
    	}
    	copy_message(head, Size, 0);
    	flag_message_copied = 1;
    	//printf("%s", message_buffer);
    }
    head = Size;
    // 🚀 Reiniciar la recepción UART en DMA
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t*)rx_buffer, BUFFER_SIZE);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART2){
		flag_tx_sent = 0;//incluir en send_tx()
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t*)rx_buffer, BUFFER_SIZE);
	}

	if(huart->Instance == USART6){
		flag_uart6_sent = 1;
	}
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c){
	if(hi2c->Instance == hi2c1.Instance){
		flag_rx_bme280 = 1;
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    if(htim==&htim3){
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){
			if(hall_polarity == 0){
				valor1_hall = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
				hall_polarity = 1;
				overflow_count_hall = 0; // Reset overflow count for new measurement
			}
			else if(hall_polarity == 1){
				valor2_hall = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
				__HAL_TIM_SET_COUNTER(htim, 0);

				// Calculate period considering possible overflow
				if(valor2_hall >= valor1_hall){
					periodo_hall = valor2_hall - valor1_hall + overflow_count_hall * (htim->Init.Period + 1);
				}
				else {
					// Handle case where counter overflowed between captures
					periodo_hall = (valor2_hall + (htim->Init.Period + 1)) - valor1_hall + overflow_count_hall * (htim->Init.Period + 1);
				}

				hall_polarity = 0;
			}
		}
		if(periodo_hall!=0){
		    rpm_motor=(float)((float)1000*1000/periodo_hall)*60;
		    motor_stopped = 0;
		}
    }

    if(htim==&htim1){
    	if(polarity_encoder==0){
    		valor1_encoder_a = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    		polarity_encoder=1;
    		overflow_count_encoder = 0;
		}
    	else{
    		valor2_encoder_a = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    		__HAL_TIM_SET_COUNTER(htim, 0);

    		// Calculate period considering possible overflow
    		if(valor2_encoder_a >= valor1_encoder_a){
    			periodo_encoder = valor2_encoder_a - valor1_encoder_a + overflow_count_encoder * (htim->Init.Period + 1);
			}
    		else {
    			// Handle case where counter overflowed between captures
    			periodo_encoder = (valor2_encoder_a + (htim->Init.Period + 1)) - valor1_encoder_a + overflow_count_encoder * (htim->Init.Period + 1);
			}
    		polarity_encoder=0;
		}
		if(periodo_encoder!=0){
			rpm_freno=(float)(((float)1000*1000/(periodo_encoder*600))*60);
			suma_rpm_freno += rpm_freno;
			contador_rpm_freno++;
			freno_stopped = 0;
		}
    }
}

// Callback to handle timer overflow
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim == &htim3){ // Ensure this is the correct timer
        overflow_count_hall++;
    }

    if(htim==&htim1){
    	overflow_count_encoder++;
    }
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2S2_Init(void);
static void MX_I2S3_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_TIM1_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/***********************************	DS18B20 functions	***********************************/
void MX_USART6_UART_Init_baud_selecting(uint32_t baud_rate)
{
	huart6.Instance = USART6;
	huart6.Init.BaudRate = baud_rate;
	huart6.Init.WordLength = UART_WORDLENGTH_8B;
	huart6.Init.StopBits = UART_STOPBITS_1;
	huart6.Init.Parity = UART_PARITY_NONE;
	huart6.Init.Mode = UART_MODE_TX_RX;
	huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart6.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_HalfDuplex_Init(&huart6) != HAL_OK)
	{
		Error_Handler();
	}
}

void DS18B20_Write(uint8_t data){
	uint8_t buffer[8];
	for(int i = 0;i < 8;i++){
		if((data & (1<<i)) != 0){
			buffer[i] = 0xFF;
		}
		else{
			buffer[i] = 0x00;
		}
	}
	HAL_UART_Transmit_DMA(&huart6,buffer,8);
}

void DS18B20_Start(void){
	MX_USART6_UART_Init_baud_selecting(9600);
	HAL_UART_Transmit_DMA(&huart6,Tx_PRESENCE_PULSE,1);
	HAL_UART_Receive_DMA(&huart6,Rx_PRESENCE_PULSE,1);
}

bool DS18B20_Presence_Pulse_check(void){
	if(Rx_PRESENCE_PULSE[0] == 0xF0){
		return 0;
	}
	return 1;
}

void DS18B20_Read(uint8_t *rx){
	uint8_t buffer[8];
	for(int i = 0;i < 8;i++){
		buffer[i] = 0xFF;
	}
	HAL_UART_Transmit_DMA(&huart6,buffer,8);
	HAL_UART_Receive_DMA(&huart6,rx,8);
}

uint8_t byte_parser(uint8_t *data){
	uint8_t value = 0;

	for(int i = 0;i < 8;i++){
		if(data[i] == 0xFF){
			value |= 1<<i;
		}
	}

	return value;
}

void action(DS18B20_state_t state){
	switch(state){
		case PRESENCE_PULSE_1:
			DS18B20_Start();
			break;
		case SKIPPING_ROM1:
			MX_USART6_UART_Init_baud_selecting(115200);
			DS18B20_Write(0xCC);
			break;
		case REQUESTING_CONVERSION:
			DS18B20_Write(0x44);
			delay_ds18b20 = HAL_GetTick();
			break;
		case PRESENCE_PULSE_2:
			DS18B20_Start();
			break;
		case SKIPPING_ROM2:
			MX_USART6_UART_Init_baud_selecting(115200);
			DS18B20_Write(0xCC);
			break;
		case REQUEST_TEMP:
			DS18B20_Write(0xBE);
			break;
		case RECEIVING_BYTE_LSB:
			DS18B20_Read(RxData_LSB);
			break;
		case RECEIVING_BYTE_MSB:
			DS18B20_Read(RxData_MSB);
			break;
		case FINISHED:
			Temp_LSB = byte_parser(RxData_LSB);
			Temp_MSB = byte_parser(RxData_MSB);

			Temp = (Temp_MSB << 8) | Temp_LSB;
			Temperature_DS18B20 = (float) Temp/16.0;
			delay_ds18b20 = HAL_GetTick();
		default:
			break;
	}
}

bool DS18B20_state_handling(DS18B20_state_t *state){
	switch(*state){
		case START_MEASURE:
			*state = PRESENCE_PULSE_1;
			return 1;
			break;
		case PRESENCE_PULSE_1:
			if(flag_uart6_sent == 1){
				if(DS18B20_Presence_Pulse_check()){
					*state = SKIPPING_ROM1;
				}
				//printf("Rx_Presence_pulse = 0x%02X\n", Rx_PRESENCE_PULSE[0]);
				flag_uart6_sent = 0;
				return 1;
			}
			return 0;
			break;
		case SKIPPING_ROM1:
			if(flag_uart6_sent == 1){
				*state = REQUESTING_CONVERSION;
				flag_uart6_sent = 0;
				return 1;
			}
			return 0;
			break;
		case REQUESTING_CONVERSION:
			if(flag_uart6_sent == 1){
				*state = PRESENCE_PULSE_2;
				flag_uart6_sent = 0;
				Rx_PRESENCE_PULSE[0] = 0xF0;
				return 1;
			}
			return 0;
			break;
		case PRESENCE_PULSE_2:
			if(flag_uart6_sent == 1){
				if(DS18B20_Presence_Pulse_check()){
					*state = SKIPPING_ROM2;
				}
				flag_uart6_sent = 0;
				return 1;
			}
			return 0;
			break;
		case SKIPPING_ROM2:
			if(flag_uart6_sent == 1){
				*state = REQUEST_TEMP;
				flag_uart6_sent = 0;
				return 1;
			}
			return 0;
			break;
		case REQUEST_TEMP:
			if(flag_uart6_sent == 1){
				*state = RECEIVING_BYTE_LSB;
				flag_uart6_sent = 0;
				return 1;
			}
			return 0;
			break;
		case RECEIVING_BYTE_LSB:
			if(flag_uart6_sent == 1){
				*state = RECEIVING_BYTE_MSB;
				flag_uart6_sent = 0;
				return 1;
			}
			return 0;
			break;
		case RECEIVING_BYTE_MSB:
			if(flag_uart6_sent == 1){
				*state = FINISHED;
				flag_uart6_sent = 0;
				return 1;
			}
			return 0;
			break;
		case FINISHED:
			Rx_PRESENCE_PULSE[0] = 0xF0;
			*state = START_MEASURE;
			return 1;
		default:
			break;
		}
	return 0;
}

uint32_t HAL_GetTick(void)
{
	if(flag_sample_sending){
		time_sample = uwTick - time_init;
	}
	return uwTick;
}
/***********************************	BME280 functions	***********************************/
void print_state(Conection_State_t state) {
    switch (state) {
        case STATE_CHECKING_COM:
            printf("STATE_CHECKING_COM\n");
            break;
        case STATE_NO_ECHO:
            printf("STATE_NO_ECHO\n");
            break;
        case STATE_CWQAP:
            printf("STATE_CWQAP\n");
            break;
        case STATE_CIPSERVER_CLOSE:
            printf("STATE_CIPSERVER_CLOSE\n");
            break;
        case STATE_SETTING_CWMODE:
            printf("STATE_SETTING_CWMODE\n");
            break;
        case STATE_SETTING_CIPMUX:
            printf("STATE_SETTING_CIPMUX\n");
            break;
        case STATE_CREATE_OWN_WIFI:
            printf("STATE_CREATE_OWN_WIFI\n");
            break;
        case STATE_CIPSERVERMAXCONN:
            printf("STATE_CIPSERVERMAXCONN\n");
            break;
        case STATE_CREATE_SERVER:
            printf("STATE_CREATE_SERVER\n");
            break;
        case STATE_SERVER_ACTIVE:
            printf("STATE_WAITING_CLIENT\n");
            break;
        default:
            printf("UNKNOWN STATE\n");
            break;
    }
}

Bool send_tx(){
	if((flag_tx_sent == 1)&&(HAL_GetTick() - time_tx > 100)){
		flag_tx_sent = 0;
	}
	tx_buffer_size = strlen(tx_buffer);
	if((tx_buffer_size != 0)&&(flag_tx_sent == 0)){
		HAL_UART_Transmit_DMA(&huart2,(uint8_t *)tx_buffer,tx_buffer_size);
		time_tx = HAL_GetTick();
		flag_tx_sent = 1;
		return 1;
	}
	return 0;
}

void update_buffer(char *buffer, size_t buffer_size, const char *new_content) {
    if (strcmp(buffer, new_content) == 0) {
        return; // Si son iguales, no hace nada
    }

    memset(buffer, 0, buffer_size);

    strncpy(buffer, new_content, buffer_size - 1); // Protege contra desbordamientos
    buffer[buffer_size - 1] = '\0'; // Asegura la terminación nula
}

void exit_passthrough(){
	HAL_Delay(1000);
	strcpy(tx_buffer, "+"); // exit passthrough mode
	send_tx();
	HAL_Delay(2);
	send_tx();
	HAL_Delay(2);
	send_tx();
	HAL_Delay(1000);
}

void print_dataRecord(const struct DataRecord* record){
	printf("%lu %.2f %.2f %.2f %.2f %.2f %.2f\n",record->timeStamp,record->samples[0],record->samples[1],record->samples[2],record->samples[3],record->samples[4],record->samples[5]);
}

void print_buf8(uint8_t* buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf("%u ", buffer[i]);
    }
    printf("\n");
}

float obtenerMedia(void) {
    if (contador_rpm_freno == 0) return 0.0f;  // Evitar división por 0
    return suma_rpm_freno / contador_rpm_freno;
}

void reiniciarMedia(void) {
	suma_rpm_freno = 0.0f;
	contador_rpm_freno = 0;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2S2_Init();
  MX_I2S3_Init();
  MX_SPI1_Init();
  MX_USB_HOST_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_USART6_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  BME280_Config(OSRS_2, OSRS_16, OSRS_1, MODE_NORMAL, T_SB_0p5, IIR_16);

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t*)rx_buffer, BUFFER_SIZE);
  strcpy(tx_buffer, "ATE0\r\n");

  dbuf_clear();

  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

    /* USER CODE BEGIN 3 */
    /*if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)==1){ //GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
    	pulsos_hall = 1;
    }*/
    if(flag_message_copied == 1){
    	from_message_to_tasks(message_buffer);
    	flag_message_copied = 0;
    }
    if((client_connected == 0)&&(HAL_GetTick() - delay_connection > 200)){
    	flag_sample_sending = 0;
    	delay_connection = HAL_GetTick();
    	update_buffer(tx_buffer, BUFFER_SIZE, (*current_wifi_com_status).command);
    	send_tx();
    }
    if((flag_cipsend == 1)){
    	if (buffer_to_send != NULL)
    	{
    		HAL_UART_Transmit_DMA(&huart2,buffer_to_send,package_size);
    		free(buffer_to_send);
    	}
    	flag_cipsend = 0;
    }
    if((flag_read_bme280 == 1)){
    	if(BME280_Measure()){
    		flag_read_bme280 = 0;
    	}
    	//printf("Reading BME280\n");
    }
    if(flag_sample_sending){
    	if((HAL_GetTick() - delay_ds18b20 > 1000)){
    		if(DS18B20_state_handling(DS18B20_state_ptr)){
    			action(DS18B20_state);
    	    }
    	}

    	if((HAL_GetTick() - delay_bme280 > 1000)){
    		if(BME280_Measure()){
    			delay_bme280 = HAL_GetTick();
    		}
    	}

    	if(HAL_GetTick() - data_timeout > 50){
    		data_timeout = HAL_GetTick();

    		if(HAL_GetTick() - tiemout_motor_stopped > 200){
    			tiemout_motor_stopped = HAL_GetTick();
    			if(motor_stopped == 1){
    				rpm_motor = 0;
    			}
    		}
    		if(freno_stopped == 1){
    			rpm_freno_media = 0;
    		}
    		sample = dbuf_current_wr_slot();
    		if (sample)
    		{
    			rpm_freno_media = obtenerMedia();
    			reiniciarMedia();
    			sample->timeStamp = time_sample;
    			sample->samples[0] = Temperature_BME280;
    			sample->samples[1] = Pressure_BME280;
    			sample->samples[2] = Humidity_BME280;
    			sample->samples[3] = Temperature_DS18B20;
    			sample->samples[4] = rpm_freno_media;
    			sample->samples[5] = rpm_motor;
    			dbuf_push_record();
    		}
    		number_of_DataRecords = dbuf_record_count();
    		if (number_of_DataRecords >= 4) {
    			bundle = dbuf_current_rd_slot();
    		    if (bundle) {
    		        package_size = DataRecord_size * number_of_DataRecords;
    		        buffer_to_send = (uint8_t *)malloc(DataRecord_size * number_of_DataRecords * sizeof(uint8_t));
    		        memcpy(buffer_to_send,bundle,package_size);
    		        sprintf(tx_buffer, "AT+CIPSEND=0,%d\r\n", package_size);
    		        send_tx();
    		        dbuf_pop_record_bundle();
    		    }
    		}
    		motor_stopped = 1;
    		freno_stopped = 1;
    	}
    }
    else if(time_sample != 0){
    	time_sample = 0;
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 200;
  PeriphClkInitStruct.PLLI2S.PLLI2SM = 5;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2S2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S2_Init(void)
{

  /* USER CODE BEGIN I2S2_Init 0 */

  /* USER CODE END I2S2_Init 0 */

  /* USER CODE BEGIN I2S2_Init 1 */

  /* USER CODE END I2S2_Init 1 */
  hi2s2.Instance = SPI2;
  hi2s2.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
  hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_96K;
  hi2s2.Init.CPOL = I2S_CPOL_LOW;
  hi2s2.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_ENABLE;
  if (HAL_I2S_Init(&hi2s2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S2_Init 2 */

  /* USER CODE END I2S2_Init 2 */

}

/**
  * @brief I2S3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S3_Init(void)
{

  /* USER CODE BEGIN I2S3_Init 0 */

  /* USER CODE END I2S3_Init 0 */

  /* USER CODE BEGIN I2S3_Init 1 */

  /* USER CODE END I2S3_Init 1 */
  hi2s3.Instance = SPI3;
  hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_96K;
  hi2s3.Init.CPOL = I2S_CPOL_LOW;
  hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S3_Init 2 */

  /* USER CODE END I2S3_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 96-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 4;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 96-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 4;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_HalfDuplex_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ESP01_IO0_GPIO_Port, ESP01_IO0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ESP01_RST_Pin|ESP01_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD5_Pin|LD6_Pin|LD7_Pin|LD8_Pin
                          |LD4_Pin|LD3_Pin|LD5D14_Pin|LD6D15_Pin
                          |Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : DATA_Ready_Pin */
  GPIO_InitStruct.Pin = DATA_Ready_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DATA_Ready_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : INT1_Pin INT2_Pin MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = INT1_Pin|INT2_Pin|MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : OTG_FS_PowerSwitchOn_Pin ESP01_IO0_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin|ESP01_IO0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ESP01_RST_Pin ESP01_EN_Pin */
  GPIO_InitStruct.Pin = ESP01_RST_Pin|ESP01_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LD5_Pin LD6_Pin LD7_Pin LD8_Pin
                           LD4_Pin LD3_Pin LD5D14_Pin LD6D15_Pin
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = LD5_Pin|LD6_Pin|LD7_Pin|LD8_Pin
                          |LD4_Pin|LD3_Pin|LD5D14_Pin|LD6D15_Pin
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

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
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <malloc.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/*********************************** Maquina de estados typedefs ****************************************/
typedef enum {
	STATE_WAITING_COM,
    STATE_COM_OK,
    STATE_CWMODE_OK,
    STATE_CWJAP_OK,
    STATE_CIPMUX_OK,
	STATE_CONNECTED,
	STATE_DISCONNECTED,
}Conection_State_t;

typedef enum{
	TRYING_TO_CONNECT,
	NO_TASK,
	SEND_TO_PC,
	READ_BME280,
}task_t;

typedef enum {
	OK,
	CONNECT,
	BUSY,
	ERR,
	AT,
	EMPTY,
	SEND_FROM_PC,
	SERVER_CLOSED,
	CIPSEND_READY,
	CIPSEND_PC,
	UNKNOWN
}response_t; // Don't use ERROR the stm already uses it

typedef struct {
        const char *keyword;
        response_t response;
        task_t task_corresponding_the_response;
} KeywordResponse;

KeywordResponse keywords[] = {
        {"CONNECT", CONNECT, TRYING_TO_CONNECT},
        {"OK", OK, TRYING_TO_CONNECT},
        {"ERROR", ERR, NO_TASK},
        {"busy", BUSY, NO_TASK},
        {"AT", AT, NO_TASK},
		{"SEND_FROM_PC", SEND_FROM_PC, TRYING_TO_CONNECT},
		{"CIPSEND_PC", CIPSEND_PC, SEND_TO_PC},
		{">", CIPSEND_READY, SEND_TO_PC},
		{"CLOSED", SERVER_CLOSED, TRYING_TO_CONNECT},
		{"READ_BME280",READ_BME280, READ_BME280},
};

char comando_AT[]="AT\r\n";

char comando_AT_CWMODE[]="AT+CWMODE=3\r\n";//Poner el ESP8266 en modo AP y conexión WIFI

char comando_AT_CWJAP[]="AT+CWJAP=\"MiFibra-9990\",\"rvbunQ6h\"\r\n";//Conectar el ESP8266 a la red WIFI

char comando_AT_CIPMUX[]="AT+CIPMUX=0\r\n";//Poner el ESP8266 en modo single connection

char comando_AT_CIPSTART[]="AT+CIPSTART=\"TCP\",\"192.168.1.21\",8000\r\n";//Comenzar la comunicacion TCP en la IP designada

float Temperature=0, Pressure=0, Humidity=0;
//sprintf(buffer, "Temp: %.2f C, Press: %.2f Pa, Hum: %.2f %%\n", Temperature, Pressure, Humidity);

typedef enum { FALSE = 0, TRUE = 1 } Bool;

response_t type_of_response = EMPTY;
response_t *response_array;

typedef struct {
        const Conection_State_t state;
        const response_t response;
        const char command[50];
        const Conection_State_t next_state;
} com_state_wifi_card;

com_state_wifi_card com_wifi_card_values[] = {
        {STATE_WAITING_COM, OK,"AT\r\n",STATE_COM_OK},
        {STATE_COM_OK, OK,"AT+CWMODE=3\r\n",STATE_CWMODE_OK},
        {STATE_CWMODE_OK, OK,"AT+CWJAP=\"MiFibra-9990\",\"rvbunQ6h\"\r\n",STATE_CWJAP_OK},
        {STATE_CWJAP_OK, OK,"AT+CIPMUX=0\r\n",STATE_CIPMUX_OK},
		{STATE_CIPMUX_OK, CONNECT,"AT+CIPSTART=\"TCP\",\"192.168.1.21\",8000\r\n",STATE_DISCONNECTED},
		{STATE_DISCONNECTED, CONNECT,"AT+CIPSTART=\"TCP\",\"192.168.1.21\",8000\r\n",STATE_DISCONNECTED},
};

typedef struct {
	const response_t response;
	const task_t next_state;
}idle_keys;

idle_keys idle_transitions[] = {
        {CLOSED, TRYING_TO_CONNECT},
		{SEND_TO_PC,SEND_TO_PC},
		{READ_BME280,READ_BME280},
};

typedef struct {
	const task_t state;
	const task_t next_state;
}task_transitions;

task_transitions task_transition[] = {
		{TRYING_TO_CONNECT, CONNECTED_IDLE},
		{SEND_TO_PC, CONNECTED_IDLE},
		{READ_BME280, SEND_TO_PC},
};



com_state_wifi_card* current_wifi_com_status=&com_wifi_card_values[0];
response_t* responses = NULL;
/*********************************** Maquina de estados typedefs ****************************************/
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

I2S_HandleTypeDef hi2s2;
I2S_HandleTypeDef hi2s3;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */
#define BUFFER_SIZE 100
#define CIPSEND_LENGTH 14
char rx_buffer[BUFFER_SIZE];
char rx_data[BUFFER_SIZE];
char tx_buffer[BUFFER_SIZE];
char data_to_send[BUFFER_SIZE];

int data_length = 0;

int tx_buffer_size = 0, rx_buffer_init = 0;

char **lines;


void DMA1_Stream5_IRQHandler(void)
{
    // Llama al manejador del HAL para procesar eventos estándar
    HAL_DMA_IRQHandler(&hdma_usart2_rx);

    // Tu código personalizado
    HAL_UART_Receive_DMA(&huart2,(uint8_t *)rx_buffer,BUFFER_SIZE);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2){
		//HAL_UART_Receive_DMA(&huart2,(uint8_t *)rx_buffer,BUFFER_SIZE);
		DMA1->LIFCR = DMA_HIFCR_CTCIF5 | DMA_HIFCR_CHTIF5 | DMA_HIFCR_CTEIF5;
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2){
		//HAL_UART_Receive_DMA(&huart2,(uint8_t *)rx_buffer,BUFFER_SIZE);
		DMA1->LIFCR = DMA_HIFCR_CTCIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTEIF6;
	}
}

Bool check_dma_transfer_complete(void) {
    if (__HAL_DMA_GET_FLAG(huart2.hdmarx, DMA_FLAG_TCIF1_5) == RESET) {
        // Activar una bandera o procesar los datos

        // Limpia el flag de transferencia completa
        __HAL_DMA_CLEAR_FLAG(huart2.hdmarx, DMA_FLAG_TCIF1_5);
        return 1;
    }
    return 0;
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
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void update_buffer(char *buffer, size_t buffer_size, const char *new_content) {
    if (strcmp(buffer, new_content) == 0) {
        return; // Si son iguales, no hace nada
    }

    memset(buffer, 0, buffer_size);

    strncpy(buffer, new_content, buffer_size - 1); // Protege contra desbordamientos
    buffer[buffer_size - 1] = '\0'; // Asegura la terminación nula
}

Bool handle_wifi_card_state(response_t reponse, com_state_wifi_card **current_wifi_com_status){
	update_buffer(tx_buffer, BUFFER_SIZE,(*current_wifi_com_status)->command);

	if (reponse == (*current_wifi_com_status)->response) {
		for(int k=0; k < (sizeof(com_wifi_card_values)/sizeof(com_state_wifi_card));k++){
			if((*current_wifi_com_status)->next_state==com_wifi_card_values[k].state){

				update_buffer(tx_buffer, BUFFER_SIZE, com_wifi_card_values[k].command);
				(*current_wifi_com_status) = &com_wifi_card_values[k];

				if(((*current_wifi_com_status)->state == STATE_DISCONNECTED)){
					memset(tx_buffer,0,BUFFER_SIZE);
					return 1;
				}
				else{
					return 0;
				}
			}
		}
	}
	return 0;
}

char **split_lines(const char *buffer, int *line_count)
{
    char **lines = NULL;
    int count = 0;
    const char *start = buffer;
    const char *end;

    while ((end = strchr(start, '\n')) != NULL) {
        size_t line_length = end - start;
        lines = (char **)realloc(lines, (count + 1) * sizeof(char *));
        if (lines == NULL) {
            printf("Error: No se pudo asignar memoria.\n");
            return NULL;
        }
        lines[count] = (char *)malloc((line_length + 1) * sizeof(char));
        if (lines[count] == NULL) {
            printf("Error: No se pudo asignar memoria para la línea %d.\n", count);
            return NULL;
        }
        strncpy(lines[count], start, line_length);
        lines[count][line_length] = '\0'; // Asegurar terminación nula
        count++;
        start = end + 1; // Continuar después de '\n'
    }

    // Añadir la última línea si no termina en '\n'
    if (*start != '\0') {
        lines = (char **)realloc(lines, (count + 1) * sizeof(char *));
        size_t line_length = strlen(start);
        lines[count] = (char *)malloc((line_length + 1) * sizeof(char));
        strncpy(lines[count], start, line_length);
        lines[count][line_length] = '\0';
        count++;
    }

    *line_count = count;
    return lines;
}

//__attribute__((optimize("O0")))
int find_first_non_null(const char *str, int size)
{
	int i = 0;
	while (i < size) {
		//printf("i = %d\n",i);
		if (str[i] != '\0') {
			return i; // Retorna la posición del primer carácter no nulo
	    }
	    i++;
	}

    return -1;
}

int find_null_position(const char *str, int size)
{
    if (str == NULL) {
        return -1; // Manejo de error si la cadena es NULL
    }

    for (int i = 0; i < size ; i++) {
        if ((str[i] == '\0')&&(i !=0)) {
            return i; // Retorna la posición del carácter nulo
        }
    }

    // No debería llegar aquí porque toda cadena válida debe tener un '\0'
    return -1;
}

void copy_from_first_non_null(char *dest, const char *src, int size)
{
    int i = 0;

    // Buscar el primer carácter no nulo en la cadena de origen
    while (i < size && src[i] == '\0') {
        i++;  // Avanzar hasta encontrar el primer carácter no nulo
    }

    // Copiar a la cadena de destino a partir de ese carácter
    int j = 0;
    for (; i < size && src[i] != '\0'; i++, j++) {
        dest[j] = src[i];
    }

    // Asegurarse de que la cadena de destino termine con '\0'
    dest[j] = '\0';
}

response_t match_respones(char* line){
	for (int k = 0; k < sizeof(keywords) / sizeof(keywords[0]); k++) {
		if (strstr(line, keywords[k].keyword) != NULL) {
			return keywords[k].response;
		}
	}
	if (strlen(line) == 0) {
		return EMPTY;
	}
	return UNKNOWN;
}

size_t get_responses(response_t **responses) // time duration, between 1 and 2 milisecond
{
	int number_of_lines_in_response = 0;

	// free(response_array);
	rx_buffer_init = find_first_non_null(rx_buffer,BUFFER_SIZE);

  if (rx_buffer_init == -1) {
    printf("rx_buffer_init = -1\n");
    *responses = NULL; // Asegura que no apuntamos a datos inválidos
    return 0;
  }

	//hacemos lineas de la cadena
	lines = split_lines(&rx_buffer[rx_buffer_init], &number_of_lines_in_response);
	memset(rx_buffer, 0, BUFFER_SIZE);

	*responses = (response_t *)malloc(number_of_lines_in_response * sizeof(response_t));

  if (*responses == NULL) {
    return 0;
  }

  if (lines != NULL) {
    for (int i = 0; i < number_of_lines_in_response; i++) {
      printf("%s",lines[i]);
      (*responses)[i] = match_respones(lines[i]);
      free(lines[i]);
    }
  }
  free(lines);
  return number_of_lines_in_response;
  // printf("Miliseconds when process_responses finishes: %lu ms\n", HAL_GetTick());
}

Bool send_tx(){

	tx_buffer_size = find_null_position(tx_buffer, BUFFER_SIZE);
	printf("send_tx() %s",tx_buffer);

	if(tx_buffer_size != -1){
		HAL_UART_Transmit_DMA(&huart2,(uint8_t *)tx_buffer,tx_buffer_size);
		HAL_UART_Receive_DMA(&huart2,(uint8_t *)rx_buffer,BUFFER_SIZE);
		return 1;
	}
	return 0;
}

Bool send_custom_chain(task_t task, Bool at_cipsend=0, int size=0){
	if(task == TRYING_TO_CONNECT){// si no llega cadena es porque o enviamos cipsend o es de los comandos de conexion
		if(at_cipsend == 1){// si es cipsend solo habria que pasar el size y el bool en 1

		}
		send_tx();
		return 1;
	}

	switch(task){
		case TRYING_TO_CONNECT:
			break;
		case READ_BME280:
			sprintf(tx_buffet, "AT+CIPSEND=%d\r\n", size);
			break;
		case SEND_TO_PC:
			sprintf(tx_buffet, "AT+CIPSEND=%d\r\n", size);
			break;
	}

	if(size != -1){
		HAL_UART_Transmit_DMA(&huart2,(uint8_t *)cadena,size);
		HAL_UART_Receive_DMA(&huart2,(uint8_t *)rx_buffer,BUFFER_SIZE);
		return 1;
	}
	return 0;
}

task_t assigne_task(response_t *reponses, size_t reponses_size){
	for(int i = 0;i < reponses_size;i++){
		for (size_t k = 0; k < sizeof(idle_transitions) / sizeof(idle_keys); k++) {
			if(idle_transitions[k].response==reponses[i]){
				return idle_transitions[k].next_state;

			}
		}
	}
	return CONNECTED_IDLE;
}

void task_handler(task_t *state, com_state_wifi_card** current_wifi_com_status, Bool* connected_to_server){
  static size_t responses_count = 0;
  static Bool cipsend_ready = 0;
  int cipsend = 0;

  if(check_dma_transfer_complete()){
	  responses_count = get_responses(&responses);
	  cipsend = responses_count;
  }

  if(*connected_to_server == 0){
	  send_tx();
  }

  for(int i = 0;i < responses_count;i++){
	  for(int k=0; k < (sizeof(keywords)/sizeof(KeywordResponse));k++){
		  if(*connected_to_server == 0){
			  *connected_to_server = handle_wifi_card_state(responses[i],&(*current_wifi_com_status));
		  }
		  else if(keywords[k].response == responses[i]){
			  switch(keywords[k].task_corresponding_the_response){
			  	  case TRYING_TO_CONNECT:
			  		  if(responses[i] == CLOSED){
			  			  *connected_to_server = 0;
			  		  }
			  		  break;;
			  	  case SEND_TO_PC:

			  		  printf("Estoy en SEND_TO_PC state\n");
			  		  break;
			  	  case READ_BME280:
			  		  printf("Estoy en READ_BME280 state\n");
			  		  break;
			  	  case NO_TASK:
			  		  cipsend--; //si el cipsend no es 0 enviamos el cipsend strcat
			  		  break;
			  }
		  }
	  }
	  if(responses[i] == CLOSED){ break;}
  }
}

Bool connected_to_server = 0, *connected_to_server_ptr = &connected_to_server;
task_t state = TRYING_TO_CONNECT, *state_ptr = &state;
//task_t **state_ptr_ptr = &state_ptr; // Puntero doble que apunta a state_ptr

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
  /* USER CODE BEGIN 2 */
  uint32_t time_communication_handling = 0;

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

  strcpy(tx_buffer, comando_AT);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

    /* USER CODE BEGIN 3 */
    if((HAL_GetTick()-time_communication_handling>1000)){
    	time_communication_handling = HAL_GetTick();
    	task_handler(state_ptr, &current_wifi_com_status, connected_to_server_ptr);
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
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
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
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
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

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

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

#include "main.h"
#include <string.h>
#include <stdio.h>


// Define LaneData_t as custom data type
typedef struct {
    uint8_t sensor1_val; // Will hold Near/Stop-line sensor state
    uint8_t sensor2_val; // Will hold Far-back sensor state

    // Hardware abstractions using auto-generated CubeMX pins
    GPIO_TypeDef* led_port;
    uint16_t red_pin;
    uint16_t yellow_pin;
    uint16_t green_pin;
} LaneData_t;

// Private UART variable
UART_HandleTypeDef huart2;

// Function prototype
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void lane_info(char lane, LaneData_t *data);
void all_traffic_to_red(void);
void lane_classifier(char* pending_lane, int num_pending_lanes, char lane_class[][10], int* green_time, LaneData_t* all_lane_data);
int lane_selector(int green_time[], int num_pending_lanes);
void display_debug_info(int num_pending_lanes, char selected_lane, int selected_green_time, char* selected_class);
void update_traffic_light(char selected_lane, int selected_green_time);
void update_pending_lane(char pending_lane[], int *num_pending_lanes, char selected_lane);


// Main function
int main(void)
{
  HAL_Init();
  SystemClock_Config();

  // Initialize GPIO for sensors and LEDs
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  // Turns all traffic light to red
  all_traffic_to_red();

  // Declare all required operational variables
  int num_pending_lanes = 4;
  char pending_lane[4] = {'N', 'E', 'S', 'W'};

  char lane_class[4][10];
  int green_time[4] = {0};
  LaneData_t all_lane_data[4];

  while (1)
  {
      if (num_pending_lanes == 0) {
          // Reset pending_lane when pending_lane is empty list
          pending_lane[0] = 'N';
          pending_lane[1] = 'E';
          pending_lane[2] = 'S';
          pending_lane[3] = 'W';
          num_pending_lanes = 4;

      } else {
          // Process and classify lanes
          lane_classifier(pending_lane, num_pending_lanes, lane_class, green_time, all_lane_data);

          // Select lane to give green signal
          int selected_lane_index = lane_selector(green_time, num_pending_lanes);

          // Selected lane info
          char selected_lane = pending_lane[selected_lane_index];
          int selected_green_time = green_time[selected_lane_index];
          char* selected_class = lane_class[selected_lane_index];

          // Debugger
          display_debug_info(num_pending_lanes, selected_lane, selected_green_time, selected_class);

          // Update traffic light on selected lane if lane is not empty
          if (selected_green_time > 0) {
              update_traffic_light(selected_lane, selected_green_time);
          }

          // Remove processed lane
          update_pending_lane(pending_lane, &num_pending_lanes, selected_lane);
      }
  }
}


// Functions
void lane_info(char lane, LaneData_t *data) {
    switch(lane) {
        case 'N':
            data->sensor1_val = HAL_GPIO_ReadPin(N_IR_1_GPIO_Port, N_IR_1_Pin);
            data->sensor2_val = HAL_GPIO_ReadPin(N_IR_2_GPIO_Port, N_IR_2_Pin);
            data->led_port    = N_red_GPIO_Port;
            data->red_pin     = N_red_Pin;
            data->yellow_pin  = N_yellow_Pin;
            data->green_pin   = N_green_Pin;
            break;

        case 'E':
            data->sensor1_val = HAL_GPIO_ReadPin(E_IR_1_GPIO_Port, E_IR_1_Pin);
            data->sensor2_val = HAL_GPIO_ReadPin(E_IR_2_GPIO_Port, E_IR_2_Pin);
            data->led_port    = E_red_GPIO_Port;
            data->red_pin     = E_red_Pin;
            data->yellow_pin  = E_yellow_Pin;
            data->green_pin   = E_green_Pin;
            break;

        case 'S':
            data->sensor1_val = HAL_GPIO_ReadPin(S_IR_1_GPIO_Port, S_IR_1_Pin);
            data->sensor2_val = HAL_GPIO_ReadPin(S_IR_2_GPIO_Port, S_IR_2_Pin);
            data->led_port    = S_red_GPIO_Port;
            data->red_pin     = S_red_Pin;
            data->yellow_pin  = S_yellow_Pin;
            data->green_pin   = S_green_Pin;
            break;

        case 'W':
            data->sensor1_val = HAL_GPIO_ReadPin(W_IR_1_GPIO_Port, W_IR_1_Pin);
            data->sensor2_val = HAL_GPIO_ReadPin(W_IR_2_GPIO_Port, W_IR_2_Pin);
            data->led_port    = W_red_GPIO_Port;
            data->red_pin     = W_red_Pin;
            data->yellow_pin  = W_yellow_Pin;
            data->green_pin   = W_green_Pin;
            break;

        default:
            break;
    }
}

void all_traffic_to_red(void) {
    // Clear all signal registers safely
    HAL_GPIO_WritePin(N_red_GPIO_Port, N_red_Pin | N_yellow_Pin | N_green_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(E_red_GPIO_Port, E_red_Pin | E_yellow_Pin | E_green_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(S_red_GPIO_Port, S_red_Pin | S_yellow_Pin | S_green_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(W_red_GPIO_Port, W_red_Pin | W_yellow_Pin | W_green_Pin, GPIO_PIN_RESET);

    // Turn Red LEDs ON globally
    HAL_GPIO_WritePin(N_red_GPIO_Port, N_red_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(E_red_GPIO_Port, E_red_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(S_red_GPIO_Port, S_red_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(W_red_GPIO_Port, W_red_Pin, GPIO_PIN_SET);

    // Delay 1s
    HAL_Delay(1000);
}

void lane_classifier(char* pending_lane, int num_pending_lanes, char lane_class[][10], int* green_time, LaneData_t* all_lane_data) {

    for (int i = 0; i < num_pending_lanes; i++) {
        // Fetch the sensors data
        lane_info(pending_lane[i], &all_lane_data[i]);

        // Evaluate lane activity based on the sensor data
        if (all_lane_data[i].sensor1_val == 1) {
            strcpy(lane_class[i], "Empty");
            green_time[i] = 0;
        } else {
            if (all_lane_data[i].sensor2_val == 1) {
                strcpy(lane_class[i], "Normal");
                green_time[i] = 5;
            } else {
                strcpy(lane_class[i], "Heavy");
                green_time[i] = 8;
            }
        }
    }
}

int lane_selector(int green_time[], int num_pending_lanes) {

    // Find the first largest value in green_time list and return its index
    int max = green_time[0];
    int max_index = 0;

    for (int i = 1; i < num_pending_lanes; i++) {
        // Use '>' to get the FIRST max index
        if (green_time[i] > max) {
            max = green_time[i];
            max_index = i;
        }
    }

    return max_index;
}

void display_debug_info(int num_pending_lanes, char selected_lane, int selected_green_time, char* selected_class){
    // Debugger to identify selected lane
    printf("--- LANE SELECTED No.%d ---\r\n", 5-num_pending_lanes);
    printf("Lane: %c\r\n", selected_lane);
    printf("Status: %s\r\n", selected_class);
    printf("Green Time: %d seconds\r\n", selected_green_time);
    printf("---------------------------\r\n\r\n");
}

void update_traffic_light(char selected_lane, int selected_green_time) {

    LaneData_t active_lane_hw;

    // Fetch the LEDs pin info
    lane_info(selected_lane, &active_lane_hw);

    // Red OFF, Green ON for selected_green_time duration
    HAL_GPIO_WritePin(active_lane_hw.led_port, active_lane_hw.red_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(active_lane_hw.led_port, active_lane_hw.green_pin, GPIO_PIN_SET);
    HAL_Delay(selected_green_time * 1000);

    // Green OFF, Yellow ON for 3s duration
    HAL_GPIO_WritePin(active_lane_hw.led_port, active_lane_hw.green_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(active_lane_hw.led_port, active_lane_hw.yellow_pin, GPIO_PIN_SET);
    HAL_Delay(3000);

    // Yellow OFF, Red ON for 1s duration due to delay purpose
    HAL_GPIO_WritePin(active_lane_hw.led_port, active_lane_hw.yellow_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(active_lane_hw.led_port, active_lane_hw.red_pin, GPIO_PIN_SET);
    HAL_Delay(1000);
}

void update_pending_lane(char pending_lane[], int *num_pending_lanes, char selected_lane) {

	// Remove processed lane and update the number of pending lanes
    int result_size = 0;
    for (int i = 0; i < *num_pending_lanes; i++) {
        if (pending_lane[i] != selected_lane) {
            pending_lane[result_size] = pending_lane[i];
            result_size++;
        }
    }
    *num_pending_lanes = result_size; // size becomes 0 if all active items are removed
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOC, S_red_Pin|S_yellow_Pin|S_green_Pin|W_red_Pin
                          |W_yellow_Pin|W_green_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOA, N_red_Pin|N_yellow_Pin|N_green_Pin|LD2_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOB, E_red_Pin|E_yellow_Pin|E_green_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = S_IR_1_Pin|S_IR_2_Pin|W_IR_1_Pin|W_IR_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = S_red_Pin|S_yellow_Pin|S_green_Pin|W_red_Pin
                          |W_yellow_Pin|W_green_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = N_IR_1_Pin|N_IR_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = N_red_Pin|N_yellow_Pin|N_green_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = E_IR_1_Pin|E_IR_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = E_red_Pin|E_yellow_Pin|E_green_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

static void MX_USART2_UART_Init(void)
{
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
}

// Retargets printf to UART
int _write(int file, char *ptr, int len) {
    extern UART_HandleTypeDef huart2;
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}

#include "car.h"
#include "main.h"
#include "stdio.h"

Car car;

void Car::init()
{
  robot_loop_tick = HAL_GetTick();
  left_motor.init();
  right_motor.init();
  left_encoder.init();
  right_encoder.init();
  left_pid.init();
  right_pid.init();

  main_uart.init();
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}

void Car::run()
{
  if (HAL_GetTick() - robot_loop_tick < ROBOT_LOOP_PERIOD)
  {
    return;
  }
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
  robot_loop_tick = HAL_GetTick();
  main_uart.get_received_data((uint8_t *)&twist);
  set_twist(twist);
  // update();
  // left_motor.set_speed(0.2f);
  printf("hello\n");
}

void Car::set_twist(const Twist &_twist)
{
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *uart)
{
  if (uart->Instance == car.main_uart.get_uart_instance())
  {
    car.main_uart.process_receive_callback();
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *uart)
{
  if (uart->Instance == car.main_uart.get_uart_instance())
  {
    car.main_uart.process_transmit_callback();
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *uart)
{
  if (uart->Instance == car.main_uart.get_uart_instance())
  {
    car.main_uart.init();
  }
}

void run_car()
{
  car.init();
  while (1)
  {
    car.run();
  }
}
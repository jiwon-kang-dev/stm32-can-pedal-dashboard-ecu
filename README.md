# STM32 CAN Pedal-Dashboard ECU Simulator

## Project Overview

This project aims to implement a CAN-based ECU simulator using two STM32 Nucleo-F446RE boards.

One board will act as a Pedal ECU, reading an analog pedal input using ADC.
The other board will act as a Dashboard ECU, receiving pedal data through CAN and controlling LED output.

## Hardware

- STM32 Nucleo-F446RE x2
- SN65HVD230 CAN Transceiver x2
- 10kΩ Potentiometer
- LED
- 120Ω termination resistor x2
- Breadboard and jumper wires
- Logic Analyzer

## Development Environment

- STM32CubeIDE
- STM32CubeMX
- Tera Term
- Windows

## Features

- UART debug output
- ADC pedal input reading
- PWM LED control
- CAN message transmission and reception
- Timeout detection
- Fail-safe LED behavior
- Non-blocking super loop structure

## Current Progress

- [x] STM32 LED Blink
- [x] USART2 UART output to PC using ST-LINK Virtual COM Port
- [x] Register-level UART test
- [x] HAL-based UART output
- [x] ADC input reading using potentiometer
- [x] ADC value output through UART
- [x] Convert ADC value to pedal percentage
- [ ] PWM LED control
- [ ] CAN transmit
- [ ] CAN receive
- [ ] Timeout and fail-safe logic
- [ ] Final demo

## Demo

### ADC Test

![ADC output](images/adc_teraterm_output.png)

## Notes

The initial UART test was verified using direct register-level code.
The STM32 successfully transmitted "Hello STM32" to Tera Term via USART2.

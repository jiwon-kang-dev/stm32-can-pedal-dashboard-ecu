# 02. UART and ADC HAL Test

## Goal

Verify UART communication and ADC input reading using STM32CubeMX and HAL library.

The purpose of this test is to read an analog voltage from a potentiometer and print the ADC value to Tera Term through USART2.

## Hardware

- STM32 Nucleo-F446RE
- 10kΩ potentiometer
- Breadboard
- Jumper wires
- Tera Term

## Wiring

The potentiometer was connected using a breadboard.

| Potentiometer Pin | STM32 Connection |
|---|---|
| One side pin | 3.3V |
| Center pin | A0 / PA0 / ADC1_IN0 |
| Other side pin | GND |

Actual breadboard connection:

- Potentiometer pins: f5, f7, f9
- 3V3 connected to j5
- A0 connected to j7
- GND connected to j9

## STM32CubeMX Configuration

- Board: NUCLEO-F446RE
- USART2: Asynchronous mode
- PA2: USART2_TX
- PA3: USART2_RX
- PA0: ADC1_IN0
- ADC resolution: 12-bit
- UART baud rate: 115200 bps

## Result

- USART2 successfully transmitted messages to Tera Term.
- ADC values were printed through UART.
- The ADC value changed when the potentiometer was rotated.
- Small fluctuations of 1~2 ADC counts were observed even when the potentiometer was not touched, which is normal ADC noise.

Example output:

```text
ADC: 1210
ADC: 1211
ADC: 1210
ADC: 1212
```

## What I Learned

- Created a CubeMX/HAL-based STM32 project
- Configured USART2 for UART debugging
- Configured PA0 as ADC1_IN0
- Read potentiometer input using ADC
- Printed ADC values to Tera Term through UART
- Observed small ADC noise, which is normal

## Next Step

- Convert ADC value from 0~4095 to 0~100%
- Control LED brightness using PWM

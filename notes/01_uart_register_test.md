# 01. UART Register Test

## Goal

Verify basic STM32 execution and UART transmission using USART2.

## Hardware

- STM32 Nucleo-F446RE
- ST-LINK Virtual COM Port
- Tera Term

## Result

- LD2 toggled successfully.
- Tera Term received the correct message:
  - `Hello STM32`
- UART baud rate: 115200 bps
- USART2 TX pin: PA2
- USART2 RX pin: PA3

## Issue

The initial CubeMX/HAL project structure was inconsistent.
`Core/Src/main.c` was edited, but the actual build target was `Src/main.c`.

## Solution

A direct register-level UART test was used to verify that the board, USART2, and ST-LINK Virtual COM Port were working correctly.

## What I Learned

- How to upload and run code on STM32 Nucleo-F446RE
- How to verify code execution using LD2
- How to transmit UART data to PC using Tera Term
- Why project structure and build target paths matter in STM32CubeIDE

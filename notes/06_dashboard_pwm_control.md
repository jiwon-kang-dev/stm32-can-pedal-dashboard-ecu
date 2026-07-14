# Dashboard PWM Control Using Received CAN Data

## Goal

The goal of this test is to control the Dashboard ECU LED brightness using pedal data received over CAN.

The Pedal ECU reads the potentiometer value through ADC, converts it to a pedal percentage, and transmits it over CAN.

The Dashboard ECU receives the pedal percentage and converts it into a PWM duty value to control the onboard LED brightness.

---

## System Overview

```text
Pedal ECU
Potentiometer
     ↓
ADC
     ↓
Pedal Percentage 0~100%
     ↓
CAN ID 0x100

Dashboard ECU
CAN Receive
     ↓
Pedal Percentage
     ↓
PWM Duty 0~999
     ↓
LD2 LED Brightness
```

---

## Hardware

- STM32 NUCLEO-F446RE x2
- CAN transceiver module x2
- Potentiometer x1
- 120 ohm termination resistor x2
- Breadboard x2
- Jumper wires
- Tera Term for UART monitoring

---

## Board Roles

### Pedal ECU

The Pedal ECU reads the potentiometer value and sends the pedal percentage over CAN.

```text
Potentiometer → ADC1_IN0 / PA0
CAN1_TX       → PB9
CAN1_RX       → PB8
UART2_TX      → PA2
UART2_RX      → PA3
```

### Dashboard ECU

The Dashboard ECU receives CAN data and controls the onboard LED using PWM.

```text
CAN1_TX  → PB9
CAN1_RX  → PB8
UART2_TX → PA2
UART2_RX → PA3
TIM2_CH1 → PA5 / LD2
```

---

## CAN Configuration

```text
CAN ID: 0x100
Frame Type: Standard ID
DLC: 1 byte
Data[0]: Pedal percentage, 0~100
Bitrate: 500 kbps
CAN1_TX: PB9
CAN1_RX: PB8
```

---

## PWM Configuration

The received pedal percentage is converted into a PWM duty value.

```text
Pedal 0%   → PWM duty 0
Pedal 50%  → PWM duty about 499
Pedal 100% → PWM duty 999
```

TIM2 was used for PWM generation.

```text
TIM2 Channel 1: PWM Generation CH1
PWM Pin: PA5 / LD2
Prescaler: 83
Counter Period: 999
```

---

## Implementation

When the Dashboard ECU receives a CAN frame with ID `0x100`, it reads `RxData[0]` as the pedal percentage.

```c
uint8_t pedal_percent = RxData[0];
uint32_t pwm_duty = 0;

if (pedal_percent > 100)
{
  pedal_percent = 100;
}

pwm_duty = (pedal_percent * 999) / 100;

__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm_duty);
```

The PWM duty value is updated based on the received pedal percentage.

---

## Result

The Dashboard ECU successfully controlled the onboard LD2 LED brightness using pedal data received over CAN.

When the potentiometer was rotated on the Pedal ECU side, the Dashboard ECU received the updated pedal value and changed the LED brightness accordingly.

### Pedal ECU UART Output

![CAN TX Pedal PWM Output](../images/06_can_tx_pedal_pwm_output.png)

### Dashboard ECU UART Output

![Dashboard PWM Output](../images/06_dashboard_pwm_output.png)

### Dashboard LED Brightness Test

Low brightness:

![Dashboard LED Brightness Low](../images/06_dashboard_led_brightness_low.jpg)

High brightness:

![Dashboard LED Brightness High](../images/06_dashboard_led_brightness_high.jpg)

---

## Verified Output

Pedal ECU:

```text
ADC: xxxx, TX Pedal: xx%
```

Dashboard ECU:

```text
RX ID: 0x100, Pedal: xx%, PWM: xxx
```

---

## What I Learned

- How to control PWM output using received CAN data
- How to map pedal percentage to PWM duty cycle
- How to use TIM2_CH1 on PA5 for onboard LED brightness control
- How to verify CAN-based ECU interaction using UART logs
- How to build a simple Pedal ECU and Dashboard ECU behavior

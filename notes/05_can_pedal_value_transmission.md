# CAN Pedal Value Transmission Test

## Goal

The goal of this test is to transmit pedal input data from one STM32 board to another STM32 board using CAN communication.

The Pedal ECU reads a potentiometer value through ADC, converts it to a pedal percentage, and sends the value over CAN.

The Dashboard ECU receives the CAN message and prints the received pedal value through UART.

---

## System Overview

```text
Potentiometer
     ↓
STM32 Pedal ECU
     ↓ ADC
Pedal Percentage 0~100%
     ↓ CAN ID 0x100
CAN Transceiver
     ↓ CANH / CANL
CAN Transceiver
     ↓
STM32 Dashboard ECU
     ↓ UART
Tera Term Output
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

The Pedal ECU reads the potentiometer value and transmits the pedal percentage over CAN.

```text
Potentiometer → ADC1_IN0 / PA0
CAN1_TX       → PB9
CAN1_RX       → PB8
UART2_TX      → PA2
UART2_RX      → PA3
```

### Dashboard ECU

The Dashboard ECU receives the CAN message from the Pedal ECU and prints the received value through UART.

```text
CAN1_TX  → PB9
CAN1_RX  → PB8
UART2_TX → PA2
UART2_RX → PA3
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

## Wiring

The two CAN transceivers were connected using CANH and CANL.

```text
Transceiver 1 CANH ↔ Transceiver 2 CANH
Transceiver 1 CANL ↔ Transceiver 2 CANL
GND common between both boards
```

Two 120 ohm termination resistors were used.

```text
Board 1 side: CANH ↔ 120Ω ↔ CANL
Board 2 side: CANH ↔ 120Ω ↔ CANL
```

The STM32 CAN pins were connected through the Morpho connector pins.

```text
PB9 / CAN1_TX → Transceiver TXD
PB8 / CAN1_RX → Transceiver RXD
```

---

## Issue During Test

At first, the CAN TX mailbox became full and the transmitted message was not received.

The UART output showed:

```text
Loop, Free mailbox: 0
No free TX mailbox
```

This happened because the CAN TX/RX wires were not connected to the correct PB8/PB9 pins.

After moving the CAN TX/RX wires to the correct Morpho connector pins, CAN communication worked successfully.

After fixing the wiring, the UART output showed:

```text
Loop, Free mailbox: 3
TX Pedal: 50
```

---

## Result

The Pedal ECU successfully transmitted the pedal percentage value over CAN.

The Dashboard ECU successfully received the CAN frame with ID `0x100` and printed the received pedal value through UART.

### Pedal ECU UART Output

![CAN TX Pedal Output](../images/05_can_tx_pedal_output.png)

### Dashboard ECU UART Output

![CAN RX Dashboard Output](../images/05_can_rx_dashboard_output.png)

### CAN Bus Wiring

![CAN Bus Wiring Top View](../images/05_can_bus_wiring_top_view.jpg)

![CAN Bus Wiring Side View](../images/05_can_bus_wiring_side_view.jpg)

---

## Verified Output

Pedal ECU:

```text
ADC: xxxx, TX Pedal: xx%
```

Dashboard ECU:

```text
RX ID: 0x100, Pedal: xx
```

---

## What I Learned

- How to configure CAN1 on STM32 NUCLEO-F446RE
- How to connect CAN transceivers using CANH and CANL
- How to use 120 ohm termination resistors
- How to transmit ADC-based pedal data over CAN
- How to debug CAN TX mailbox issues
- How to verify CAN communication using UART logs

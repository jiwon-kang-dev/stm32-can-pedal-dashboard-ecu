# Non-blocking Pedal ECU CAN Transmission

## Goal

The goal of this step is to remove the blocking delay from the Pedal ECU CAN transmission loop.

Previously, the Pedal ECU used `HAL_Delay(100)` after each CAN transmission.  
This made the MCU wait for 100 ms and prevented the main loop from executing other tasks during that time.

In this step, the delay-based structure was changed to a non-blocking structure using `HAL_GetTick()`.

---

## Previous Structure

```c
ADC Read
CAN Transmit
HAL_Delay(100);
```

In this structure, the MCU stops and waits for 100 ms after each transmission.

---

## Improved Structure

```c
currentTime = HAL_GetTick();

if ((currentTime - lastTxTime) >= PEDAL_TX_PERIOD_MS)
{
  lastTxTime = currentTime;

  ADC Read
  CAN Transmit
}
```

In this structure, the main loop keeps running continuously.  
The Pedal ECU only transmits a CAN message when 100 ms has passed since the last transmission.

---

## Key Variables

```c
#define PEDAL_TX_PERIOD_MS 100

uint32_t lastTxTime = 0;
```

```text
PEDAL_TX_PERIOD_MS:
Defines the CAN transmission period as 100 ms.

lastTxTime:
Stores the last CAN transmission time.
```

---

## Implementation Concept

`HAL_GetTick()` returns the elapsed time in milliseconds after the MCU starts.

```c
uint32_t currentTime = HAL_GetTick();
```

The Pedal ECU compares the current time with the last transmission time.

```c
if ((currentTime - lastTxTime) >= PEDAL_TX_PERIOD_MS)
```

If more than 100 ms has passed, the Pedal ECU reads the ADC value, converts it to pedal percentage, and sends it over CAN.

```c
pedal_percent = (adc_value * 100) / 4095;
TxData[0] = (uint8_t)pedal_percent;
HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
```

---

## Result

The Pedal ECU successfully transmitted ADC-based pedal percentage values over CAN without using `HAL_Delay()`.

The Dashboard ECU continued to receive CAN messages normally and controlled the LED brightness using the received pedal value.

The timeout and fail-safe behavior also continued to work correctly when the Pedal ECU stopped transmitting.

---

## What I Learned

- How to replace `HAL_Delay()` with a time-checking structure using `HAL_GetTick()`
- How to implement periodic CAN transmission without blocking the main loop
- How to manage transmission timing using `lastTxTime`
- Why non-blocking loop structures are useful in embedded systems

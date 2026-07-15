# Non-blocking Dashboard ECU Receive and Timeout Loop

## Goal

The goal of this step is to organize the Dashboard ECU receive loop into a clearer non-blocking structure.

The Dashboard ECU continuously checks for received CAN messages and also checks for CAN timeout using `HAL_GetTick()`.

No `HAL_Delay()` is used in the receive loop.

---

## Improvements

Previously, important values were written directly in the code.

```c
0x100
1000
999
```

These values were replaced with meaningful definitions.

```c
#define PEDAL_CAN_ID      0x100
#define CAN_TIMEOUT_MS    1000
#define PWM_MAX_DUTY      999
```

This improves code readability and maintainability.

---

## Key Definitions

```text
PEDAL_CAN_ID:
CAN ID used for pedal position messages.

CAN_TIMEOUT_MS:
Timeout threshold for CAN message loss.

PWM_MAX_DUTY:
Maximum PWM duty value for TIM2 Channel 1.
```

---

## Non-blocking Loop Structure

The Dashboard ECU main loop continuously runs without using a blocking delay.

```c
uint32_t currentTime = HAL_GetTick();
```

The loop first checks whether a CAN message has been received.

```c
if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) > 0)
{
  HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxData);
}
```

When a valid pedal CAN message is received, the last receive time is updated.

```c
lastRxTime = currentTime;
```

The received pedal percentage is converted to PWM duty.

```c
pwm_duty = (pedal_percent * PWM_MAX_DUTY) / 100;
```

Then the Dashboard ECU updates the LED brightness.

```c
__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm_duty);
```

---

## Timeout Check

The timeout condition is checked in every loop.

```c
if ((currentTime - lastRxTime) > CAN_TIMEOUT_MS)
{
  if (timeoutFlag == 0)
  {
    timeoutFlag = 1;

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);

    snprintf(msg, sizeof(msg), "CAN Timeout - Fail Safe Active\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
  }
}
```

If no CAN message is received for more than 1000 ms, the Dashboard ECU enters fail-safe mode and turns off the LED by setting the PWM duty to 0.

---

## Restore Behavior

When CAN communication is restored, the Dashboard ECU clears the timeout state.

```c
if (timeoutFlag == 1)
{
  timeoutFlag = 0;

  snprintf(msg, sizeof(msg), "CAN Restored\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
}
```

After restoration, the Dashboard ECU resumes normal PWM control using the received pedal value.

---

## Result

The Dashboard ECU successfully receives CAN pedal messages, controls the LED brightness using PWM, and detects CAN timeout without using a blocking delay.

The timeout and fail-safe behavior continued to work correctly after the receive loop structure was improved.

---

## What I Learned

- How to organize a CAN receive loop without using `HAL_Delay()`
- How to use `HAL_GetTick()` for timeout checking
- How to improve embedded code readability using `#define`
- How to separate configuration values from logic
- How to maintain fail-safe behavior in a non-blocking loop

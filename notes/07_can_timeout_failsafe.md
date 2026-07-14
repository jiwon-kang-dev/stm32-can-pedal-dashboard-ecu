# CAN Timeout and Fail-safe Test

## Goal

The goal of this test is to detect CAN communication loss on the Dashboard ECU and activate a fail-safe behavior.

When the Dashboard ECU does not receive a CAN message for more than 1 second, it detects a timeout and turns off the LED by setting the PWM duty to 0.

When CAN communication is restored, the Dashboard ECU returns to normal PWM control using the received pedal value.

---

## System Behavior

```text
Normal State:
Pedal ECU sends CAN message periodically
Dashboard ECU receives pedal value
Dashboard ECU controls LD2 brightness using PWM

Timeout State:
No CAN message for more than 1000 ms
Dashboard ECU detects CAN timeout
PWM duty is set to 0
LD2 LED turns off

Restored State:
CAN message is received again
Dashboard ECU clears timeout state
PWM control resumes
```

---

## Implementation Concept

The Dashboard ECU stores the last CAN receive time whenever a valid CAN message is received.

```c
lastRxTime = HAL_GetTick();
```

`HAL_GetTick()` returns the elapsed time in milliseconds after the MCU starts.

The timeout condition is checked by comparing the current time with the last receive time.

```c
if ((HAL_GetTick() - lastRxTime) > 1000)
{
  // Timeout detected
}
```

If no CAN message is received for more than 1000 ms, the Dashboard ECU enters fail-safe mode.

```c
__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
```

This sets the PWM duty to 0 and turns off the LD2 LED.

---

## Key Variables

```c
uint32_t lastRxTime = 0;
uint8_t timeoutFlag = 0;
```

```text
lastRxTime:
Stores the time when the last valid CAN message was received.

timeoutFlag:
Stores the current timeout state.
0 = Normal state
1 = Timeout state
```

The `timeoutFlag` prevents the timeout message from being printed repeatedly.

---

## Test Method

1. Run both Pedal ECU and Dashboard ECU.
2. Confirm that the Dashboard ECU receives CAN messages and controls LED brightness.
3. Disconnect the Pedal ECU USB cable.
4. Confirm that the Dashboard ECU detects timeout after about 1 second.
5. Confirm that the LD2 LED turns off.
6. Reconnect the Pedal ECU USB cable.
7. Confirm that the Dashboard ECU prints `CAN Restored`.
8. Confirm that LED brightness control resumes.

---

## Result

The Dashboard ECU successfully detected CAN message loss.

When the Pedal ECU stopped transmitting CAN messages, the Dashboard ECU entered fail-safe mode and turned off the LED.

When CAN communication was restored, the Dashboard ECU returned to normal PWM control.

---

## Verified Output

### Timeout Detection

![CAN Timeout Fail-safe Output](../images/07_timeout_failsafe_output.png)

### Fail-safe LED OFF

![Fail-safe LED OFF](../images/07_failsafe_led_off.jpg)

### CAN Restored

![CAN Restored Output](../images/07_can_restored_output.png)

### Demo Video

[Timeout Fail-safe Demo Video](../images/07_timeout_failsafe_demo.mp4)

---

## What I Learned

- How to detect CAN communication timeout using `HAL_GetTick()`
- How to store the last CAN receive time
- How to implement fail-safe behavior when CAN messages are lost
- How to prevent repeated timeout messages using a state flag
- How to restore normal operation when CAN communication resumes

# 04. PWM LED Control Using Pedal Percentage

## Goal

Control the brightness of the onboard LED using PWM based on the pedal percentage value.

The potentiometer input is read through ADC, converted into a pedal percentage value from 0% to 100%, and then mapped to a PWM duty value.

## Input

- Potentiometer connected to A0 / PA0 / ADC1_IN0
- ADC raw value range: 0 ~ 4095
- Pedal percentage range: 0 ~ 100%

## PWM Output

- LED: Onboard LD2
- PWM pin: PA5
- Timer: TIM2
- Channel: TIM2_CH1
- PWM period: 999

## Conversion

```c
pedal_percent = (adc_value * 100) / 4095;
pwm_duty = (pedal_percent * 999) / 100;
```

## Result

The onboard LD2 brightness changed according to the potentiometer input.

When the potentiometer value increased, the pedal percentage increased and the PWM duty value increased as well.

Example output:

```text
ADC: 0, Pedal: 0%, PWM: 0
ADC: 528, Pedal: 12%, PWM: 119
ADC: 984, Pedal: 24%, PWM: 239
ADC: 1562, Pedal: 38%, PWM: 379
```

## Test Evidence

### Tera Term Output

![PWM LED control output](../images/pwm_led_control_output.png)

## What I Learned

- How to configure a timer channel for PWM output
- How to map ADC values to PWM duty cycle
- How to control LED brightness using PWM
- How ADC, UART, and PWM can work together in one embedded loop

## Next Step

- Use two STM32 boards for CAN communication
- Send the pedal percentage value from the Pedal ECU to the Dashboard ECU
- Control the Dashboard ECU LED based on received CAN data

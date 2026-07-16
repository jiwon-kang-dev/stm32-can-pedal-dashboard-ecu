# CAN Logic Analyzer Verification

## Goal

The goal of this test is to verify the CAN communication signal using a logic analyzer.

The Pedal ECU CAN TXD signal and the Dashboard ECU CAN RXD signal were measured simultaneously.

The CAN transmission period, CAN identifier, DLC, and pedal data were also verified using the PulseView CAN protocol decoder.

---

## Measurement Setup

```text
Logic Analyzer GND
→ Common GND

Logic Analyzer CH1
→ Pedal ECU transceiver TXD

Logic Analyzer CH2
→ Dashboard ECU transceiver RXD
```

PulseView channel assignment:

```text
D0 = Pedal ECU TXD
D1 = Dashboard ECU RXD
```

Capture settings:

```text
Sample rate: 12 MHz
Capture samples: 5 M samples
CAN nominal bitrate: 500000 bit/s
```

---

## TXD and RXD Comparison

The TXD signal from the Pedal ECU and the RXD signal from the Dashboard ECU were captured simultaneously.

![CAN TXD and RXD Comparison](../images/10_logic_analyzer_tx_rx_comparison.png)

The two signals showed the same CAN frame pattern, confirming that the transmitted frame was successfully delivered through the CAN bus to the Dashboard ECU.

Small differences may appear near the ACK field because the receiving CAN node participates in the acknowledgement process.

---

## CAN Transmission Period

The time interval between two consecutive CAN frames was measured using PulseView cursors.

![CAN Transmission Period](../images/10_logic_analyzer_can_period.png)

Measured transmission period:

```text
Approximately 99.09 ms
```

The firmware transmission period was configured as:

```c
#define PEDAL_TX_PERIOD_MS 100
```

The measured value confirms that the Pedal ECU transmits the CAN message at approximately 100 ms intervals.

---

## CAN Frame Decode

The Dashboard ECU RXD signal was decoded using the PulseView CAN decoder.

![CAN Frame Decode](../images/10_logic_analyzer_can_decode.png)

Decoded CAN frame:

```text
Identifier: 256 (0x100)
Frame format: Standard CAN
DLC: 1
Data byte 0: 0x43
```

The transmitted data byte represents the pedal percentage.

```text
0x43 hexadecimal = 67 decimal
Pedal position = 67%
```

This confirms that the CAN message format implemented in the firmware matches the actual transmitted frame.

---

## Result

The following items were successfully verified:

- CAN TXD signal generation from the Pedal ECU
- CAN RXD signal reception by the Dashboard ECU
- Successful CAN frame delivery between the two ECUs
- Approximately 100 ms CAN transmission period
- Standard CAN identifier `0x100`
- DLC of 1 byte
- Pedal percentage data transmitted in `Data[0]`

---

## What I Learned

- How to connect a logic analyzer to CAN TXD and RXD signals
- How to capture digital CAN signals using PulseView
- How to measure the CAN transmission period using cursors
- How to configure a CAN protocol decoder
- How to verify CAN ID, DLC, and payload data from an actual waveform
- How to compare firmware configuration with measured hardware behavior

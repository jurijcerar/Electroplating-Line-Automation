# Electroplating Line Automation

An industrial automation project that simulates an automated electroplating (galvanizing) plant using Siemens PLC technology. The system combines PLC-based process control, an HMI, and CAN-based fieldbus communication to model a realistic industrial production line.

---

## System Overview

Workpieces are transported through multiple chemical treatment tanks by a motorized trolley. Each workpiece follows a configurable process — which tanks to visit and for how long — before returning automatically to the pickup station.

```
[Pickup] → [PreTreatment Tank 1] → [PreTreatment Tank 2] → [Electrolyte Tank 1] → [Electrolyte Tank 2] → [Pickup]
                ↑                                                                                             ↑
           Transport trolley (horizontal movement + lift/lower mechanism)
```

### Hardware

| Component | Model |
|---|---|
| PLC | Siemens S7-1200 |
| HMI | Siemens KTP series (touch panel) |
| CANopen master module | Siemens CM CANopen |
| CANopen sensor node | STM32F103 (Olimexino-STM32) |
| Development environment | TIA Portal V19 |

---

## Repository Structure

```
├── DALU-Projekt_HMI_V19/           # TIA Portal project
│   ├── DALU-Projekt_HMI_V19.ap19  # Main project file — open this in TIA Portal
│   ├── HSP_V19_0079_001_S71200_CANopen_1.0.isp19  # Hardware support package for CM CANopen
│   ├── CM_CANopen_HSP_TIA_V19.zip # CANopen function block library
│   ├── CM_CANopen_HSP_TIA19_Beta.txt  # HSP release notes
│   ├── siem0024.gse / .gsg         # PROFIBUS GSD device files
│
└── canopennode/STM32/              # Firmware for the STM32 CANopen sensor node
    ├── Makefile
    ├── src/
    │   ├── application.c/.h        # Application logic: ADC read + CANopen OD write
    │   ├── main.c                  # Entry point, CANopen init loop
    │   ├── hw_config.c/.h          # System/timer/USART setup
    │   ├── stm32_olimexino.c/.h    # Board HAL: LEDs, ADC, UART
    │   ├── stm32_it.c/.h           # Interrupt handlers (TIM2 → 1ms tick)
    │   ├── CO_*.c/.h               # CANopenNode stack (PDO, SDO, NMT, Emergency…)
    │   ├── CANopen.c/.h            # Stack init + main process loop
    │   ├── CO_OD.c/.h              # Object Dictionary (auto-generated from EDS)
    │   └── stdlib/                 # STM32F10x standard peripheral library
    ├── ld/stm32_rom.ld             # Linker script
    └── eds/
        ├── TemperatureSensor.eds   # CANopen EDS device description
        ├── CO_OD.c / CO_OD.h       # Object Dictionary source (from EDS)
```

---

## How It Works

### PLC Side (TIA Portal)

The S7-1200 PLC runs the main automation logic in SCL. The program is structured into function blocks:

**Automatic mode sequence:**
1. Operator places workpiece at pickup station and presses Start.
2. PLC runs a pre-process test cycle (trolley traversal, sensor check).
3. Trolley moves to the pickup station, lowers, picks up the workpiece, raises.
4. For each process step defined in the workpiece's recipe:
   - Trolley moves horizontally to the target tank position.
   - Trolley lowers workpiece into the tank.
   - Timer counts down the configured immersion time.
   - Trolley raises the workpiece.
5. After all steps, trolley returns to pickup station and lowers workpiece.
6. Signal tower indicates "done".

**Safety logic:**  
Every movement command checks position sensors and occupancy flags before executing. Invalid states (e.g. trolley not raised before horizontal move, tank already occupied) are blocked in software and flagged as errors.

**Manual mode:**  
Activated from the HMI or hardware switch. Disables automatic sequencing and lets an operator jog the trolley and lift mechanism individually for maintenance or error recovery.

### HMI (KTP panel)

The touch panel connects to the PLC via PROFIBUS/PN and provides:

- Live visualization of trolley position and active process state
- Start / Stop / Emergency Stop buttons
- Mode switch (Auto / Manual)
- Alarm/event log with acknowledgement
- Temperature readout from the CANopen sensor node

### CANopen Fieldbus (STM32 node)

The STM32 firmware implements a minimal CANopen device (DS301) using the [CANopenNode](https://github.com/CANopenNode/CANopenNode) stack. It acts as a remote I/O node that periodically transmits temperature sensor data to the PLC's CM CANopen module via PDO.

**Signal chain:**
```
Analog sensor → PC0 (ADC1 CH10) → 12-bit ADC → voltage conversion → temperature formula → OD entry → TPDO → CAN bus → PLC
```

**Temperature formula (in `application.c`):**
```c
float voltage     = (adc_raw / 4096.0) * 3.3;
float temperature = voltage * 104.667 - 53.0;   // sensor-specific calibration
```

The result is stored in `OD_AI_PV32Bit[ODA_AI_PV32Bit_AI_PV32Bit1]` and transmitted automatically via the configured TPDO mapping.

---

## Getting Started

### TIA Portal project

1. Install **TIA Portal V19** (or higher with V19 compatibility).
2. Install the CM CANopen HSP:
   - In TIA Portal → Options → Support Packages → Install from file
   - Select `DALU-Projekt_HMI_V19/HSP_V19_0079_001_S71200_CANopen_1.0.isp19`
3. Open `DALU-Projekt_HMI_V19/DALU-Projekt_HMI_V19.ap19`.
4. If prompted about the CANopen function block library, import `CM_CANopen_HSP_TIA_V19.zip` via the library manager.

> **Note:** The `.ap19` file is the complete self-contained TIA Portal project archive. All PLC program blocks, HMI screens, hardware configuration, and tag tables are inside it.

### STM32 firmware

**Prerequisites:**
- `arm-none-eabi-gcc` toolchain (tested with GCC 6 2017-q2)
- `make`

**Build:**
```bash
cd canopennode/STM32
make all
```

Output: `output/main.bin` and `output/main.elf`.

**Flash** (via UART bootloader, using the included stm32flash tool on Windows):
```bash
make install COM=COM8
```

Or use any SWD programmer (ST-Link, J-Link) with the `.elf` file.

**Toolchain path:** The `Makefile` has a hardcoded Windows/Cygwin path. Edit the `TOOLCHAIN_PATH` variable at the top of the Makefile to match your environment:
```makefile
# Linux/macOS (if toolchain is in PATH):
TOOLCHAIN_PATH =
# Windows Cygwin example:
TOOLCHAIN_PATH = "/cygdrive/c/Program Files (x86)/GNU Tools ARM Embedded/6 2017-q2-update/bin/"
```

---

## Object Dictionary

The CANopen Object Dictionary is defined in `canopennode/STM32/eds/TemperatureSensor.eds`. The generated C source (`CO_OD.c/h`) is in both `eds/` and `src/` — `src/` is what actually gets compiled. If you regenerate the OD from the EDS (e.g. using [libedssharp](https://github.com/robincornelius/libedssharp)), copy the output to `src/`.

The temperature value is mapped at:
- **OD index:** per the EDS `AI_PV32Bit` array entry
- **TPDO:** transmitted on timer event to the PLC master

---

## Technology Stack

| Layer | Technology |
|---|---|
| PLC language | SCL (Structured Control Language) |
| HMI runtime | Siemens WinCC (embedded in KTP) |
| Fieldbus (PLC↔HMI) | PROFINET or MPI |
| Fieldbus (PLC↔sensor) | CAN / CANopen DS301 |
| MCU | STM32F103 (Cortex-M3, 72 MHz) |
| CANopen stack | CANopenNode (open-source, GPL-2.0) |

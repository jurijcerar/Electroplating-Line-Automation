# Siemens PLC Electroplating Automation System

This repository contains a complete industrial automation project that simulates an automated electroplating (galvanizing) plant using Siemens PLC technology.  
The system combines PLC-based process control, a human–machine interface (HMI), and CAN-based fieldbus communication to model a realistic industrial production line.

The project is designed to resemble real-world automation scenarios, including safety handling, diagnostics, manual control, and decentralized sensor integration.

---

## System Overview

The automated electroplating system processes workpieces through multiple chemical treatment steps using a transport trolley and several process tanks.  
Each workpiece follows a predefined process configuration that determines which tanks are used and how long the workpiece remains in each tank.

The system includes:

- Four process solution tanks (pretreatment and electrolytes)
- A transport trolley with lifting and lowering mechanism
- Multiple selectable surface coating processes
- Operator control console
- Signal tower (traffic light) for machine state indication
- Touch-based HMI for visualization and interaction
- CAN / CANopen fieldbus integration for sensor data

---

## Core Features

### Automated Process Control
- Fully automatic execution of electroplating sequences
- Time-controlled immersion in process tanks
- Sequential trolley movement between stations
- Pre-process system test run
- Automatic return of finished workpieces to the pickup point

### Material Detection
- Recognition of different workpiece materials
- Material-dependent process handling or visualization
- Clear status indication via LEDs or HMI

### Transport Trolley Logic
- Controlled movement between stations
- Safe lifting and lowering operations
- Prevention of invalid or unsafe states
- Sensor-based position and occupancy checks

---

## Human–Machine Interface (HMI)

The HMI acts as a minimal SCADA system and provides:

- Live visualization of:
  - Trolley position
  - Active process state
  - Workpiece processing status
  - Errors and malfunctions
- Operator interaction:
  - Start and resume automatic processes
  - Emergency stop
  - Mode switching (automatic / manual)
- Event logging:
  - Informational events
  - Warnings
  - Blocking errors
- Historical data review and event acknowledgment

---

## Manual Control and Maintenance Mode

A dedicated manual mode allows:

- Error correction and recovery
- Diagnostic movement of the transport trolley
- Maintenance operations
- Safe intervention during abnormal conditions

Manual mode overrides automatic operation and is clearly indicated by the signal tower.

---

## Safety and Error Handling

Safety is a core design aspect of the system:

- Explicit safety conditions derived from realistic operation scenarios
- Logical prevention of unsafe states
- Detection of sensor and actuator failures
- Defined responses for error conditions
- Visual indication of system status and faults

---

## Fieldbus Integration (CAN / CANopen)

The project includes decentralized automation via CANopen:

- A microcontroller-based CANopen node simulates a temperature sensor
- Sensor data is transmitted via CAN to the PLC
- Temperature values are visualized on the HMI
- Threshold violations generate warnings or alarms

This demonstrates real industrial fieldbus communication and integration.

---

## Technology Stack

- **PLC**: Siemens S7
- **Programming Language**: SCL (primary)
- **Development Environment**: Siemens TIA Portal
- **HMI**: Siemens KTP series
- **Fieldbus**: CAN / CANopen
- **Target System**: Industrial automation lab hardware

---

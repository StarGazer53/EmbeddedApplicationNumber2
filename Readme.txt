# Smart Alarm Clock with FPGA Timer  
## Final Project Release – Milestone 5  

### Overview
The Smart Alarm Clock is an embedded systems application developed on the Terasic DE10-Standard platform. The system integrates an ARM Hard Processor System (HPS) application with FPGA-based hardware using memory-mapped I/O (MMIO) through the lightweight HPS-to-FPGA bridge.

The application provides alarm functionality, real-time display output, and hardware interaction through switches, push-buttons, LEDs, and HEX displays. The system demonstrates integration between software control logic and FPGA-based I/O.

---

### Features
- Alarm enable/disable using hardware switches  
- Alarm trigger based on system time  
- LED indication for system state and alarm activity  
- HEX display output for time visualization  
- Memory-mapped I/O communication with FPGA  
- Dual operation modes:
  - Stub mode (software simulation)
  - FPGA MMIO mode (hardware interaction)

---

### System Architecture
The system is composed of:

- ARM HPS Application (C program)
- FPGA peripheral registers (switches, keys, LEDs, HEX)
- Lightweight HPS-to-FPGA bridge for communication

The ARM application reads inputs from FPGA registers and updates outputs based on system logic.

---

### Project Structure
Smart Alarm Clock with FPGA Timer
Milestone 4 – Iteration 3
ARM HPS + FPGA MMIO Integration
Caleb Aguiar – SWE-450

WHAT THIS PROGRAM DOES

This application runs on the ARM Hard Processor System (HPS) of the Terasic DE10-Standard board and integrates with FPGA peripherals using memory-mapped I/O (MMIO).

The program:

Reads physical switch input (SW[9:0]) from FPGA registers

Reads push-button input (KEY[3:0], active-low)

Displays time on the HEX 7-segment displays

Controls LEDR[9:0] outputs

Implements alarm enable, trigger, and reset logic

Alarm Behavior:

SW0 enables the alarm

When the current time reaches the preset alarm time (30 seconds), the alarm triggers

When triggered, LEDs blink

Pressing KEY0 resets the alarm

Time is displayed continuously on HEX displays

The system supports two execution modes:

Stub Mode (simulation – no hardware required)

FPGA MMIO Mode (real hardware interaction using /dev/mem)

HOW TO COMPILE

Navigate to the project directory:

cd ~/Embedded-Application-

(or your project folder location)

Compile in Stub Mode (Safe Mode)

This mode does not require FPGA hardware access.

make clean
make

Compile in FPGA MMIO Mode (Hardware Mode)

This enables real hardware access using /dev/mem.

make clean
make CFLAGS_EXTRA='-DUSE_FPGA_IO=1'

HOW TO RUN

Stub Mode:

./release2_app

FPGA MMIO Mode (must use sudo):

sudo ./release2_app

HOW TO DEMONSTRATE FUNCTIONALITY

Start the program in MMIO mode:
sudo ./release2_app

Ensure SW0 is OFF:

Alarm is disabled

LEDs remain off

Turn SW0 ON:

Alarm becomes enabled

When time reaches 30 seconds:

LEDs blink (alarm active)

Press KEY0:

Alarm resets

LEDs stop blinking

Press Ctrl+C to exit cleanly

CONFIGURATION

Hardware base address:

LW_BRIDGE_BASE = 0xFF200000

Default register offsets:

LEDR 0x0000
HEX3_HEX0 0x0020
HEX5_HEX4 0x0030
SW 0x0040
KEY 0x0050

If the FPGA design changes, update these values in include/config.h.
# Milestone 1: STM32F4 Blinky Application

## Project Overview
This repository contains the firmware code for a simple Blinky application designed for our STM32 development board. This project serves as Milestone 1, verifying that our development setup is functioning correctly and familiarizing the team with the hardware and IDE before proceeding to more complex logic.

---

## Hardware and Software Used
* **Microcontroller Board:** STM NUCLEO-F446RE
* **Connection:** Mini-USB cable
* **IDE/Toolchain:** STM32CubeIDE

---

## Video Demonstration
**▶️ [Insert Link to your Video Here]**

---

## Setup Instructions: Running the Blinky App

### Step 1: Hardware Setup
1. Connect the NUCLEO-F446RE development board to the computer using a mini-USB cable.
2. Verify that the power indicator LED on the board turns on.

### Step 2: Project Configuration
1. Launch STM32CubeIDE and create a new STM32 project.
2. Select the NUCLEO-F446RE board from the target selector.
3. In the Device Configuration Tool, locate the pin connected to the onboard LED (PA5) and configure it as `GPIO_Output`.
4. Generate the initialization code.

### Step 3: Programming the Logic
1. Open the `main.c` file located in the generated `Src` folder.
2. Locate the infinite `while (1)` loop inside the main function.
3. Add the Hardware Abstraction Layer (HAL) functions to toggle the LED and create a delay:
   ```c
   // Toggle the onboard LED state (PA5)
   HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); 
   
   // Create a 500ms delay
   HAL_Delay(500);

### Step 4: Build and Flash
1. Click the Build button (hammer icon) to compile the project and check for any syntax errors.
2. Click the Debug/Run button to flash the compiled firmware onto the NUCLEO-F446RE board.
3. The onboard LED will now blink continuously, confirming that the development environment is set up correctly.

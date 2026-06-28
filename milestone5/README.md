# Milestone 5: Smart Adaptive Traffic Light System 

## Project Overview
This repository contains the complete firmware code and project files for a four-way junction Smart Adaptive Traffic Light System. The system utilizes an STM32 microcontroller and IR sensors to dynamically adjust green light durations based on real-time traffic flow. It features embedded decision-making algorithms that prioritize heavy traffic lanes while ensuring fairness via an anti-starvation protocol.

---

## Hardware and Software Used
* Microcontroller: STM32 NUCLEO-F446RE
* Sensors: 8x Infrared (IR) Sensors (2 per lane for density estimation)
* Actuators: 12x LEDs (Red, Yellow, Green for 4 directions)
* Connection: Mini-USB cable
* IDE/Toolchain: STM32CubeIDE and STM32CubeMX

---

## Software Setup: Importing and Running the Project

This repository contains the fully configured STM32 project folder. Follow these steps to open the project directly in your IDE:

### Step 1: Download the Repository
1. Click the Code button at the top of this GitHub repository and select Download ZIP, or clone the repository using Git:
   ```bash
   git clone [YOUR_GITHUB_REPO_URL]

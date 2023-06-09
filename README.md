# TEST-SUITE_ChibiOS
This project, called "TEST-SUITE," is a C project developed using ChibiStudio (Eclipse + ChibiOS) and implemented on an STM32 Nucleo-64 (NUCLEO-F401RE) board. It aims to test various functionalities and components of the board, such as LEDs, joystick, OLED display, buzzer, and more. The project leverages the ChibiOS real-time operating system and utilizes C programming language.

## Project Description

### Development Environment
The development environment used for programming and controlling the board is ChibiStudio, a collection of programs based on Eclipse specifically designed to support ChibiOS. ChibiOS is a highly compact and efficient real-time operating system for embedded applications. The tools used in ChibiStudio include Eclipse IDE, GNU GCC ARM Compiler & Tools, Open OCD Tool & scripts, and ChibiOS libraries.

ChibiStudio provides two types of perspectives: C/C++ development for project creation, management, and code writing, and Debug for testing and fixing functional errors.

The project's structure consists of various folders, including the Debug folder for execution configuration, and configuration headers such as `chconf.h`, `halconf.h`, and `mcuconf.h`. The Makefile contains a script used by the compiler for code building, and the `main.c` file serves as the entry point of the application.

## Development Board
The development board used in this project is an STM32 Nucleo-64 (NUCLEO-F401RE). It is a 32-bit microcontroller board based on the ARM Cortex-M architecture. The board provides GPIO (General Purpose Input Output) pins that allow communication with various peripherals.
![STM32 Nucleo-64](https://user-images.githubusercontent.com/71086591/179379789-9ab43abd-b920-42ec-8091-00f09c9c4434.jpg)

### Components and Functionalities

#### GPIO (General Purpose Input Output)
The board's GPIO pins are organized in groups of 16 elements and can be configured as input or output independently. Each group is called a "Port" and is identified by a letter (GPIOA, GPIOB, GPIOC, etc.). Each pin is identified by the combination of the letter P, the port identifier (A, B, C, ...), and the pin identifier (0, 1, 2, ..., 15). GPIO pins can be programmed in various modes such as input, output, analog, or alternate mode.

#### RGB LED
The project utilizes an RGB LED, which consists of three different colored LEDs (red, green, and blue) combined into a single component. By controlling the input pins of the LED, it is possible to turn on or off each color independently, creating various color combinations.
![RGB LED](https://user-images.githubusercontent.com/71086591/179380009-5ee7dafe-1c7c-4f2e-9451-a0145f3cacb1.png)

#### ChibiOS PAL Driver
The ChibiOS PAL Driver (Port Abstraction Layer) is a driver provided by ChibiOS that simplifies the interaction with GPIO pins. It offers various functions for configuring and controlling individual pins, groups of pins, entire ports, and lines. The PAL driver provides an abstraction of the hardware structure, making it easier to develop code. Proper initialization of each pad is necessary before usage, and the configuration is performed using functions such as `palSetPadMode()`, `palSetGroupMode()`, and `palSetPortMode()`.
![CHIBIOS PAL DRIVER](https://user-images.githubusercontent.com/71086591/179380012-096c766b-7156-4e55-9ec4-0cf3720565b3.png)

#### USART (Universal Asynchronous Receiver-Transmitter)
The project utilizes the USART interface for serial communication with external devices. The ChibiOS Serial Driver is implemented to facilitate communication through the USART interface. It provides functions for initializing the USART peripheral, sending and receiving data, and configuring parameters such as baud rate, data length, parity, and stop bits.

Parallel communication: Each bit is transmitted through a dedicated line. It requires a line for each bit plus one line for synchronization.
![Parallel Communication](https://user-images.githubusercontent.com/71086591/179380170-a0e331e3-4b2e-404b-bde2-9094241c2b42.png)

Serial communication: The bits are transmitted through a single line, called the bus. It requires more complex synchronization mechanisms.
![Serial Communication](https://user-images.githubusercontent.com/71086591/179380182-2f79cee4-a6e4-4961-8648-53e7f5e42597.jpg)

#### Shell Interface
The project includes a Shell interface that allows asynchronous command-based interaction through the USART interface. Users can send commands to the board via a serial terminal, and the board executes the corresponding actions. The Shell interface provides a convenient way to test and control different functionalities of the board.

#### STM32 ADC
The STM32 ADC is an essential component of the microcontroller that converts analog signals into digital values. It has key specifications such as the full-scale voltage range (FSR), resolution, and sampling rate. The ADC architecture consists of a sample & hold circuit, a digital-to-analog converter, a comparator, and a successive approximation register (SAR). The ADC supports single and continuous conversion modes. The STM32 microcontroller provides multiple input sources that can be used as channels for conversion. This summarizes the STM32 ADC and its main features.
![Title](https://user-images.githubusercontent.com/71086591/179380210-f1d7265c-3a29-4d5f-ab43-92f2df9eaa3a.jpg)

#### Joystick
The joystick is another device used during the campus and provided in the starter kit. It is a simple example of an analog peripheral that uses two potentiometers. They vary the resistance in the circuit and indicate the position of the joystick relative to the X and Y axes.
![Title](https://user-images.githubusercontent.com/71086591/179380109-0a1d56e6-37a3-437c-9fa6-48fc44a613a6.png)

#### OLED Display
The project incorporates an OLED (Organic Light-Emitting Diode) display for visual output. Communication with the display is performed using the I2C (Inter-Integrated Circuit) protocol. The project utilizes the I2C interface of the STM32 microcontroller to send commands and data to the OLED display, enabling the display of text, graphics, and other visual elements.

#### PWM (Pulse Width Modulation)
PWM is a technique used for controlling the power and brightness of external peripherals. The project utilizes PWM to control the buzzer's sound and LED's brightness. The STM32 microcontroller provides timers (TIM) that can generate PWM signals. By configuring the timer's period and duty cycle, the project achieves desired sound and brightness levels.
![Title](https://user-images.githubusercontent.com/71086591/179380154-c53b758d-2b2a-44ec-a489-8554f97b342b.jpg)

#### Encoder
An encoder is an electronic device that converts angular positions into digital electrical signals. The project utilizes an encoder as a dimmer to control the intensity of the LED. By rotating the encoder, the user can increase or decrease the LED's brightness.

### Test Cases
The project includes a set of test cases to verify and validate the functionalities and components of the board. Each test case covers specific features and can be executed individually. The test cases are identified by the following format: TS-REQXX-YY, where XX represents the requirement number, and YY represents the test case number.

The test cases cover functionalities such as controlling the RGB LED's color and behavior, obtaining joystick position coordinates, displaying states on the OLED display, playing acoustic signals using the buzzer, and implementing a color wheel demo.

## Requirements
- ChibiStudio 20.4

## Results
The TEST-SUITE_ChibiOS project provides a comprehensive testing platform for exploring and validating various functionalities and components of embedded systems. By leveraging the ChibiOS real-time operating system and the STM32 microcontroller's capabilities, the project demonstrates how to control LEDs, read analog inputs, communicate via serial interface, and interact with external peripherals like OLED display and buzzer. The project's test cases serve as valuable examples for developers working on embedded systems and provide a solid foundation for building more complex applications.
![IMG_1603](https://user-images.githubusercontent.com/71086591/179379953-c6e6047a-6d97-4c85-95be-1590b0e73fb4.jpeg)

## Contributions
- [Leonardo Catello](https://github.com/Leonard2310) 
- [Diego Bert](https://github.com/ISDIES)
- [Simone Mario D'Onghia](https://github.com/Limon93)

## License
This project is licensed under the [GNU General Public License v3.0]. Refer to the LICENSE file for more information.

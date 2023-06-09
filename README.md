

# TEST-SUITE_ChibiOS
This project, called "TEST-SUITE," is a C project developed using ChibiStudio (Eclipse + ChibiOS) and implemented on an STM32 Nucleo-64 (NUCLEO-F401RE) board. It aims to test various functionalities and components of the board, such as LEDs, joystick, OLED display, buzzer, and more. The project leverages the ChibiOS real-time operating system and utilizes C programming language.

## Development Environment
The development environment used for programming and controlling the board is ChibiStudio, a collection of programs based on Eclipse specifically designed to support ChibiOS. ChibiOS is a highly compact and efficient real-time operating system for embedded applications. The tools used in ChibiStudio include Eclipse IDE, GNU GCC ARM Compiler & Tools, Open OCD Tool & scripts, and ChibiOS libraries.

ChibiStudio provides two types of perspectives: C/C++ development for project creation, management, and code writing, and Debug for testing and fixing functional errors.

The project's structure consists of various folders, including the Debug folder for execution configuration, and configuration headers such as `chconf.h`, `halconf.h`, and `mcuconf.h`. The Makefile contains a script used by the compiler for code building, and the `main.c` file serves as the entry point of the application.

## Development Board
The development board used in this project is an STM32 Nucleo-64 (NUCLEO-F401RE). It is a 32-bit microcontroller board based on the ARM Cortex-M architecture. The board provides GPIO (General Purpose Input Output) pins that allow communication with various peripherals.

![STM32 Nucleo-64](https://user-images.githubusercontent.com/71086591/179379789-9ab43abd-b920-42ec-8091-00f09c9c4434.jpg)

## Components and Functionalities

### GPIO (General Purpose Input Output)
The board's GPIO pins are organized in groups of 16 elements and can be configured as input or output independently. Each group is called a "Port" and is identified by a letter (GPIOA, GPIOB, GPIOC, etc.). Each pin is identified by the combination of the letter P, the port identifier (A, B, C, ...), and the pin identifier (0, 1, 2, ..., 15). GPIO pins can be programmed in various modes such as input, output, analog, or alternate mode.

### RGB LED
The project utilizes an RGB LED, which consists of three different colored LEDs (red, green, and blue) combined into a single component. By controlling the input pins of the LED, it is possible to turn on or off each color independently, creating various color combinations.

![RGB LED](https://user-images.githubusercontent.com/71086591/179380009-5ee7dafe-1c7c-4f2e-9451-a0145f3cacb1.png)

### ChibiOS PAL Driver
The ChibiOS PAL Driver (Port Abstraction Layer) is a driver provided by ChibiOS that simplifies the interaction with GPIO pins. It offers various functions for configuring and controlling individual pins, groups of pins, entire ports, and lines. The PAL driver provides an abstraction of the hardware structure, making it easier to develop code. Proper initialization of each pad is necessary before usage, and the configuration is performed using functions such as `palSetPadMode()`, `palSetGroupMode()`, and `palSetPortMode()`.

![ChibiOS PAL Driver](

https://user-images.githubusercontent.com/71086591/179380095-cfbc0096-bf27-4557-a4a7-8c4e9edf250e.png)

### USART (Universal Asynchronous Receiver-Transmitter)
The project utilizes the USART interface for serial communication with external devices. The ChibiOS Serial Driver is implemented to facilitate communication through the USART interface. It provides functions for initializing the USART peripheral, sending and receiving data, and configuring parameters such as baud rate, data length, parity, and stop bits.

### Shell Interface
The project includes a Shell interface that allows asynchronous command-based interaction through the USART interface. Users can send commands to the board via a serial terminal, and the board executes the corresponding actions. The Shell interface provides a convenient way to test and control different functionalities of the board.

### Joystick
The STM32 Nucleo-64 board includes a built-in joystick, which provides analog input for capturing the position relative to the X and Y axes. The project utilizes the ADC (Analog-to-Digital Converter) of the STM32 microcontroller to read the analog input and determine the joystick's position. The joystick can be moved in four directions: up, down, left, and right.

### OLED Display
The project incorporates an OLED (Organic Light-Emitting Diode) display for visual output. Communication with the display is performed using the I2C (Inter-Integrated Circuit) protocol. The project utilizes the I2C interface of the STM32 microcontroller to send commands and data to the OLED display, enabling the display of text, graphics, and other visual elements.

### PWM (Pulse Width Modulation)
PWM is a technique used for controlling the power and brightness of external peripherals. The project utilizes PWM to control the buzzer's sound and LED's brightness. The STM32 microcontroller provides timers (TIM) that can generate PWM signals. By configuring the timer's period and duty cycle, the project achieves desired sound and brightness levels.

### Encoder
An encoder is an electronic device that converts angular positions into digital electrical signals. The project utilizes an encoder as a dimmer to control the intensity of the LED. By rotating the encoder, the user can increase or decrease the LED's brightness.

## Test Cases
The project includes a set of test cases to verify and validate the functionalities and components of the board. Each test case covers specific features and can be executed individually. The test cases are identified by the following format: TS-REQXX-YY, where XX represents the requirement number, and YY represents the test case number.

The test cases cover functionalities such as controlling the RGB LED's color and behavior, obtaining joystick position coordinates, displaying states on the OLED display, playing acoustic signals using the buzzer, and implementing a color wheel demo.

## Conclusion
The TEST-SUITE_ChibiOS project provides a comprehensive testing platform for exploring and validating various functionalities and components of embedded systems. By leveraging the ChibiOS real-time operating system and the STM32 microcontroller's capabilities, the project demonstrates how to control LEDs, read analog inputs, communicate via serial interface, and interact with external peripherals like OLED display and buzzer. The project's test cases serve as valuable examples for developers working on embedded systems and provide a solid foundation for building more complex applications.

![TEST-SUITE_ChibiOS](https://user-images.githubusercontent.com/71086591/179380337-2a7aa405-df6a-4a5d-86e5-0e16e6bc27e5.png)

Feel free to explore the project and experiment with different functionalities!

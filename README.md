# xConnect Cluster Software Repository

This repository contains the software for car clusters compatible with the MCB 2.2 sold by xConnect and Arduino UNO-like microcontrollers. The files are provided in the `.ino` format, which is compatible with the Arduino IDE. To compile the software, you will need to use the Arduino IDE.

## License

This project is licensed under the GNU GPL 3.0 license. For more information, please see the [LICENSE](LICENSE) file.

## Required hardware

**Option 1:**
- MCB 2.1 or 2.2 board

**Option 2:**
-   Arduino UNO
-   A CAN BUS shield, we recommend the [CAN-BUS Shield V2.0](https://wiki.seeedstudio.com/CAN-BUS_Shield_V2.0/)

**For both options you will need:**

- A 12V 2.5A power supply (2.5A is the lowest amperage that you should use)
- Jumper wires (dupont)
- A cluster that you want to use and that is supported in this repo

## Getting Started

To get started with the xConnect Cluster Software, follow these steps:

1. **Download and install the Arduino IDE** from the [official website](https://www.arduino.cc/en/software)
2. **Download and install the CAN BUS library** from [here](https://github.com/coryjfowler/MCP_CAN_lib), a tutorial on how to do this can be found [here](https://www.arduino.cc/en/Guide/Libraries#toc4)
3. **Install the ESP32 library** (only for MCB boards) by going to File -> Preferences -> Additional Boards Manager URLs and pasting the following link: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json. Then you need to go to the Boards Manager and install `esp32 by Espressif Systems`.
4. **Clone or download this repository** to your local machine.
5. **Open the `.ino` file** for your specific cluster software in the Arduino IDE.
6. **Set up the compiler and port** by going to Tools -> Board -> ESP32 Dev Module or Arduino UNO, and then Tools -> Port -> COMX (where X is the port number)
7. **Compile and upload the code** to your MCB 2.2 or Arduino UNO-like microcontroller using the Arduino IDE.

## Support

If you encounter any issues while using the xConnect Cluster Software, please create an issue on this repository.

For priority support, my supporters can message me at: support@xconnect.cc

## Contributing

We welcome contributions from the community! If you would like to contribute to the xConnect Cluster Software, please follow these guidelines:

1. **Fork the repository**.
2. **Create a new branch** for your feature or bug fix.
3. **Make your changes** and test them thoroughly.
4. **Submit a pull request** with a clear description of your changes.

We will review your pull request and merge it if it meets our standards.

## Acknowledgments

We would like to thank the Arduino community for their support and contributions to the open-source hardware and software ecosystem.

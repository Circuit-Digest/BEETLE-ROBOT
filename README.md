# Wall Climbing Robot (ESP32)

A Wi-Fi controlled wall-climbing robot built on the ESP32. The robot uses a BLDC motor (driven through an ESC) to power a suction fan for wall adhesion, and two DC motors (driven via an MX1508 driver) for movement. Control is done through a lightweight built-in web interface — no app required.

## Features

- 📶 Self-hosted web control panel served directly from the ESP32
- 🌀 BLDC suction fan control with ON/OFF toggle and variable speed slider (0–100%)
- 🕹️ Directional movement control (Forward / Backward / Left / Right / Stop)
- ⚙️ ESC signal generated using the ESP32 `ledc` PWM peripheral (50 Hz, 16-bit resolution)
- 📱 Responsive UI — usable from a phone or desktop browser

## Hardware Requirements

| Component                     | Notes                                  |
|--------------------------------|-----------------------------------------|
| ESP32 development board        | Any variant with `ledc` PWM support     |
| Brushless (BLDC) motor + ESC   | For the suction fan                     |
| MX1508 dual H-bridge driver    | Drives two DC gear motors for movement  |
| 2x DC gear motors               | Robot drive wheels/tracks               |
| Suitable power supply/battery  | Sized for ESC + motor draw              |

### Pinout

| Function        | ESP32 Pin |
|------------------|-----------|
| ESC signal       | GPIO 4    |
| Left motor IN1   | GPIO 16   |
| Left motor IN2   | GPIO 17   |
| Right motor IN1  | GPIO 18   |
| Right motor IN2  | GPIO 19   |

> Adjust pin numbers in the sketch if your wiring differs.

## Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software) (or PlatformIO)
- [ESP32 board package](https://github.com/espressif/arduino-esp32) installed in the Arduino IDE
- Built-in libraries only: `WiFi.h`, `WebServer.h`

## Setup

1. Clone this repository.
2. Open the sketch in the Arduino IDE.
3. Update your Wi-Fi credentials:
   ```cpp
   const char* ssid = "your_ssid";
   const char* password = "your-password";
   ```
4. Select your ESP32 board and correct COM port.
5. Upload the sketch.
6. Open the Serial Monitor at `115200` baud to view the assigned IP address once connected.

## Usage

1. Power on the robot. On boot, the ESC is armed automatically (~5 second delay at minimum throttle — **keep the robot clear of obstacles during this step**).
2. Connect your phone/laptop to the same Wi-Fi network as the ESP32.
3. Navigate to the IP address printed in the Serial Monitor (e.g. `http://192.168.1.42`).
4. Use the web UI to:
   - Turn the suction fan **ON/OFF**
   - Adjust suction speed with the slider
   - Drive the robot using the directional buttons

## Safety Notes

- Always arm the ESC with the propeller/fan clear of people and objects.
- Double-check ESC and motor wiring polarity before powering on.
- Use a power supply rated for the combined current draw of the ESC and drive motors.
- This project controls physical hardware — test movement and suction functions independently before combined operation.

## Roadmap Ideas

- [ ] Add authentication to the web interface
- [ ] Add live telemetry (battery voltage, suction current)
- [ ] Replace polling slider with WebSocket-based control for lower latency
- [ ] Add mobile-friendly joystick control

## License

MIT — feel free to use, modify, and share.

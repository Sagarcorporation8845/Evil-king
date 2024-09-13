# Evil-king
Evil-King

Evil-King is a comprehensive WiFi security project that combines two powerful techniques: Evil-Twin Access Point attacks and continuous Deauthentication attacks. Designed for educational and testing purposes, this project allows you to perform simultaneous WiFi network attacks to capture passwords and test network security.
Features

    Evil-Twin Access Point: Creates a rogue WiFi network to capture user credentials from devices attempting to connect.
    Continuous Deauthentication Attack: Disrupts the target WiFi access point to force devices to reconnect, allowing for password capture attempts.
    Simultaneous Attacks: Run both Evil-Twin and Deauthentication attacks concurrently or independently.
    Interactive Web Interface: Start and manage attacks using a user-friendly web interface.
    Network Range Visualization: Displays nearby WiFi networks with color-coded signal strength.

Prerequisites

    ESP8266 Module
    Arduino IDE
    Required Libraries (included in the repository)

Installation

    Clone the Repository:

    bash

    git clone https://github.com/yourusername/Evil-King.git

    Install Dependencies: Open the Arduino IDE and install the necessary libraries from the Library Manager.

    Upload the Code: Connect your ESP8266 module to your computer and upload the code using the Arduino IDE.

    Configure Settings: Update the configuration settings in the code to match your network environment and attack preferences.

Usage

    Start Attacks: Access the web interface hosted by the ESP8266 module. Use the buttons to start or stop Deauthentication and Evil-Twin attacks.

    Monitor Network Activity: View the range of WiFi networks and their signal strength on the web interface.

    Authenticate with Target Network: Submit the password for the target network. The system will attempt to authenticate and stop the Deauthentication attack if successful.

Important Notes

    Ethical Use: This project is intended for educational purposes and authorized testing only. Unauthorized use of this tool is illegal and unethical.
    Compatibility: Ensure your ESP8266 module is compatible with the code provided.

Contributing

Contributions are welcome! If you have any improvements or suggestions, please create a pull request or open an issue.
License

This project is licensed under the MIT License. See the LICENSE file for details.
Contact

For any questions or support, please contact [your email address] or open an issue on the repository.

# Evil-king

Evil-King is a comprehensive WiFi security project that combines two powerful techniques: Evil-Twin Access Point attacks and continuous Deauthentication attacks. Designed for educational and testing purposes, this project allows you to perform simultaneous WiFi network attacks to capture passwords and test network security. It Also Verify the Password which enter by the target users.
Features

    Evil-Twin Access Point: Creates a rogue WiFi network to capture user credentials from devices attempting to connect.
    Continuous Deauthentication Attack: Disrupts the target WiFi access point to force devices to reconnect, allowing for password capture attempts.
    Simultaneous Attacks: Run both Evil-Twin and Deauthentication attacks concurrently or independently.
    Interactive Web Interface: Start and manage attacks using a user-friendly web interface.
    Network Range Visualization: Displays nearby WiFi networks with color-coded signal strength.
    
Youtube Channel Of Sagar Corporation

([https://youtube.com/@sagarcorporation](https://www.youtube.com/@sagarcorporation)) 

Portal UI:-
![image](https://github.com/user-attachments/assets/82520c89-5348-4616-bd07-9afa7c1ed95d)

Phishing Page:-
![2](https://github.com/user-attachments/assets/51453c39-8316-4951-85bf-1c1263423545)

Password Verify :-
![3](https://github.com/user-attachments/assets/dc86c299-6d99-476f-8a4a-d0e871df9300)

Wrong Password Redirected To Again Phishing Page :- 
![4](https://github.com/user-attachments/assets/5849b010-764a-45cb-a64a-277c3997c50c)

Password Got Successfully !!!
![5](https://github.com/user-attachments/assets/553ff4a6-3055-426f-bd8a-00a8b636e4ba)


Prerequisites

    ESP8266 Module
    Arduino IDE
    Required Libraries (included in the repository)

Installation
Clone the Repository:

    https://github.com/Sagarcorporation8845/Evil-king.git
Install Dependencies: Open the Arduino IDE and install the necessary libraries from the Library Manager.

Upload the Code: Connect your ESP8266 module to your computer and upload the code using the Arduino IDE.
Configure Settings: Update the configuration settings in the code to match your network environment and attack preferences.


Usage

    Step 1 : Power on device it creates One wifi access point with name Evil-king and password as Pass@123
    step 2 : After Successfully Connected to Wifi Evil-king , keep connection without internet and go to your browser
             and open 192.168.0.1 ip address
    step 3 : Now you can see the admin portal is open where it scan for available wifi networks.
    step 4 : Now Select the wifi which you want to perform this attack , after selecting the wifi click on start Deauth button
             which start to disconnect all devices from that wifi network.
    step 5 : Now click on Evil Twin button which create a replica (fake) open wifi network with target wifi name which confuse users
             when any user is connected to fake access point it redirect user to our phishing page where it ask user to wifi requires 
             an update so enter wifi password, if user submit wrong password it verify the password if password is correct the fake access point
             is closed and origional Evil-king Access point started again , if user enter wrong password it redirected to phishing page again.
    step 6 : Now connect to Evil-King Wifi and agin Go to Portal Ip address 192.168.0.1 where first stop Deauth attack And you can See the
             Target Wifi's Name and Password is Showing.
             

Important Notes

    Ethical Use: This project is intended for educational purposes and authorized testing only. Unauthorized use of this tool is illegal and unethical.
    Compatibility: Ensure your ESP8266 module is compatible with the code provided.

Contributing

       SAGAR BHONG
       
Contributions are welcome! If you have any improvements or suggestions, please create a pull request or open an issue.
License

This project is licensed under the MIT License. See the LICENSE file for details.
Contact

For any questions or support, please contact (https://github.com/Sagarcorporation8845/Evil-king.git) or open an issue on the repository.

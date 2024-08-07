# LCD Messaging
Connects the LCD display to SCE Messaging API. Send messages to a chatroom on SCE's main website and have it display on a 16x2 LCD screen.


<img width="400" alt="image" src="https://github.com/user-attachments/assets/5d098c70-cbcd-4063-9df6-6ea4bed5f42e">


## Prerequisite 
- ESP32 Dev Kit
- 16x2 LCD Screen
- Arduino IDE

## Getting Started
1. Make sure you have Arduino IDE installed, for more information, you can find it [here](https://support.arduino.cc/hc/en-us/articles/360019833020-Download-and-install-Arduino-IDE).
2. Clone the repository, open the listen.ino file in Arduino IDE, and now it should look something like this:

<img width="1440" alt="image" src="https://github.com/user-attachments/assets/560185e1-3335-44e0-8aec-50b49cd1dc27">




3. On lines 14 and 15, enter your Wi-Fi Credentials. Replace ssid and password with your Wi-Fi name and password.

<img width="612" alt="image" src="https://github.com/user-attachments/assets/d88d4bbb-b5a8-4caa-921f-daa6a10672cd">



4. Next, go to the [SCE website](https://sce.sjsu.edu/) and login. Go to 'Profile' tab, generate an API key, and copy it.

<img width="1436" alt="image234" src="https://github.com/user-attachments/assets/0c11f715-6562-4e37-a4fc-8d11ef2db894">



5. Go back to Arduino IDE and line 16, you can copy and paste the apiKey here you can specify any room ID you want for the id query parameter
<img width="734" alt="image" src="https://github.com/user-attachments/assets/99552326-7fda-4cee-84c1-39b37936c439">



6. Now, you should go to the top bar and click Sketch -> Upload and wait for it to upload and compile.

<img width="1440" alt="image" src="https://github.com/user-attachments/assets/864deedb-c1df-49bc-a131-7e08646ad2dc">

7. Go to the 'Admin' tab and click on 'Messaging' on the sidebar. You will see the 'Messaging' page here.


<img width="1440" alt="image" src="https://github.com/user-attachments/assets/d6683473-f3e9-4ef2-8d00-79945ae2e41d">










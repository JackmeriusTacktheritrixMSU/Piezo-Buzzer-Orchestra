# Piezo-Buzzer-Orchestra
<img width="799" height="591" alt="image" src="https://github.com/user-attachments/assets/0ceca32c-1842-41e1-a92e-ae857cfb17e4" />

I wanted to fill a room with sound from every angle with multiple singularly addressable piezo buzzer assemblies.
Each buzzer assembly should be a small little self contained pocket with adhesive backing so you can stick it anywhere. The assemblies will be able to buzz together through espnow, or buzz on their own at intervals through a separate mode. A master device or "Controller" will allow you to choose between buzz together and buzz at intervals 
<img width="954" height="732" alt="image" src="https://github.com/user-attachments/assets/fd2fdc6a-fa98-4c79-9191-c22f4a87b753" />


Ideally, the devices will each be portable, simple to use and program, and last a long time.

**Wiring** 
The wiring hinges on 4 modules, listed again in the bill of materials

*For the Master Module**
1x Esp32C3 
which will have any two digital pins wired to one side of any two buttons, along with the other two button sides going to 3.3v

2x Buttons
Wiring described above

1x 3 AAA Battery Holder
Ground wired to ground, positive wired to 5v which does take voltage on my board but check yours first

Thats it!

*For the Child Modules*

1x ESP32C3
Ground and a digital pin lead to each piezo buzzer

1x Piezo Buzzer
Wiring described above

1x 3 AAA Battery Holder
Ground to ground, positive to 5v you know the drill 
<img width="2123" height="1785" alt="circuit_image (2)" src="https://github.com/user-attachments/assets/ba931016-3ce6-4737-8889-eca9ad514af9" />

**Printed Parts**

*For The Child Modules* 
The devices have a 3D printed Caddy that they will sit on.  The caddy just takes normal m2 screws, no heatserts just screw into the plastic the trashy way. 
<img width="759" height="551" alt="image" src="https://github.com/user-attachments/assets/47813b41-c92c-40e0-95f2-21277b2bee99" />

*For The Master Module* 
The device has a two shell design, with perfboard in-between as a shell. a 2mm layer of perfboard on which the buttons and esp32 c3 sit, and then a second shell on top with again screws.
<img width="836" height="611" alt="image" src="https://github.com/user-attachments/assets/f5982c2f-b365-48d8-a1e4-fddb2ccd68f5" />


**Programming**

The system is  a pretty simple, cut and dry espnow connection- with the second button broadcasting to all slave/child modules to go into a lower power state and forever work on an interval.
Its a lot adapted from multiple pieces of example code, including the examples included with the library, so credit goes to adafruit. Some of it is original, and if you have any questions just email me at conerc22@proton.me




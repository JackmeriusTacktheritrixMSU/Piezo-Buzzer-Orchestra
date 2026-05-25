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


**BOM**
| Product Name | Product Link | Product Cost | Quantity | Already Had? |
| --- | --- | --- | --- | --- |
| Piezo Buzzer | [Link](https://www.amazon.com/QWORK-Electronic-piezoelectric-Physical-Continuous/dp/B0CQRJJXC8) | N/A | 20 | Yes |
| ESP32 C3 | [Link](https://www.amazon.com/10PCS-Supermini-ESP32-C3-Development-Board/dp/B0F888JQ91/ref=sr_1_3?crid=2E3NZ3B6FP2UD&dib=eyJ2IjoiMSJ9.tgczZJsDG1VBFTs_wJQFV4OtBuV_4Ndhxr5RFfktMe_FjuqpOr1x4vYL6cd6C93RIweUrDH5ojclRDq2yWONUh5f522HVE4eSdPuphsauCWblE0Hy7YwkEJdkKdSn2B3gEtVNn8vosb58DJbzf7hVMlvxWruaV2lW78sC6wTT4jxcHKYcg6QmaXcX1ic_Nz_1V2GajNhXY8FN2eKL36bOyk2iKVYOZBFuynG2uiHkMA.HBVEtSNaI8MiM47X710K3ogx3Pcrqdl44GhRUnpHMwE&dib_tag=se&keywords=esp32%2Bc3%2B10&qid=1779739539&sprefix=esp32%2Bc3%2B10%2Caps%2C167&sr=8-3&th=1) | N/A | 21 | Yes |
| AAA Battery 3 Holder | [Link](https://www.amazon.com/DIANN-Battery-Applicable-Electronic-Appliance/dp/B0CHNL5TB9/ref=sr_1_3?crid=39P1CNF79S6SO&dib=eyJ2IjoiMSJ9.3hd-i86-uGQtGCvwrRuTrs-ApvLikLIWIpEV--p5Yn3HdHYCGr4wBezDUNPR71l7qdV6cf9k1j2aDTO6ZjTJXvCHGGyggGZ64vluLnrEUQ4OllcoVtfaWybbLGXyclPRderlPqAOvrbd0sRveLzoxFnlPewF-55uflcQBovAbVWyvVRnHanKxqJqcs8_aQCQoS8FYRaoBbQWEOc7Dg3_bMsOnRCogrVSEEQ-8orRqDw.oTsNAxoPH45KdBYz2wgWeOsLLiQ2NLYemeptvyxSDks&dib_tag=se&keywords=aaa%2Bbatteries%2B3%2Bholder&qid=1779739579&sprefix=aaa%2Bbatteries%2B3%2Bhold%2Caps%2C158&sr=8-3&th=1) | N/A | 21 | Yes |
| Buttons | [Link](https://www.amazon.com/YETAIDA-125pcs-Tactile-Momentary-Assortment/dp/B07QV8JS27/ref=sr_1_10_sspa?crid=1Y48H84XIF1IB&dib=eyJ2IjoiMSJ9.y3V8lVL6h3Z7C4KKpoBqyJ5b0ySNLJveYsucURfyNa984GPQGwvio90y4KQqia46099YW3FnZhjCA1N6dvw9sr5XyVWDYfwH9wL6Iy7MllE9djrLNV_mSZFldGeXFd1NO7xND_KlzLqb238PKmVc2n-SaxOVUq40kGkBTF8Gpe9WepsjXV3jSqshI4RBRjXTipWCApBLezgrtxO84CUNwgthLfX1UJyk2_TUIydak0A.qyfVYAcQH0hA4S2I6XaLtcH5oyRzFashBFgEjgnCqFA&dib_tag=se&keywords=Push+buttons&qid=1779739619&sprefix=push+button%2Caps%2C156&sr=8-10-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9tdGY&psc=1) | $10 | 2 | No |
| Project Totals |  | $10 | 64 |  |


**Zine**
[piezo buzzer orchestra zine.pdf](https://github.com/user-attachments/files/28234114/piezo.buzzer.orchestra.zine.pdf)



# Electric Heater
This is an electric heater control system, where the temperature of water tank is controlled by heating/cooling systems.
This project is implemented using PIC16F877A microcontroller, and run on PICGenios simulator.

## Layout
<ul>
  <li>Temperature display on DIS3 and DIS4</li>
  <li>ON/OFF button on RB0</li>
  <li>UP button on RB4</li>
  <li>DOWN button on RB5</li>
  <li>LED on RB1</li>
</ul>

![image](https://user-images.githubusercontent.com/59218287/118491191-02932900-b71f-11eb-9e52-4b6c36031706.png)

By default, the two 7-segment display units show the current water temperature<br>
In setting mode, the two units show the reference temperature and blink every 1 second<br>
After 5 seconds without button events, the units resume to showing the current water temperature

## Events and Interrupts
![image](https://user-images.githubusercontent.com/59218287/118491140-f7d89400-b71e-11eb-8b97-9591f202989a.png)
![image](https://user-images.githubusercontent.com/59218287/118491161-fd35de80-b71e-11eb-9325-85dbbc14b005.png)

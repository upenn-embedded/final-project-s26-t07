[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/-Acvnhrq)
# Final Project

**Team Number:** 7

**Team Name:** Team 7

|  Team Member Name  | Email Address       |
|--------------------|---------------------|
| Lori Brown         | blor@seas           |
| Amy Luo            | amyluo@seas         |
| Jeremy Chung       | ddchung@seas        |

**GitHub Repository URL:** https://github.com/upenn-embedded/final-project-s26-t07

**GitHub Pages Website URL:** [for final submission]*

## Final Project Proposal

### 1. Abstract  
We are building an autonomous assembly-line mocktail mixer which allows users to select a drink option. We plan to use an OLED screen and buttons for drink selection and notifications. Solenoids and pumps used in conjunction with load cells for each bottle will dispense the liquid and notify the user if a bottle needs to be refilled. The cup (detected using a load cell) will move through a series of stages corresponding to each bottle.

### 2. Motivation  
Mocktail and tea prices have been going up in the past few years. It is no longer appealing to buy an 8 dollar peach oolong tea, for example, which is very easy to make. We propose an autonomous drink-maker to solve this problem and save ourselves money. We want a device that we can use at home with minimal maintenance and cost.

### 3. System Block Diagram
![](files/system.png)   
### 4. Design Sketches  
![](files/design.png)  
The critical design features include the linear slide (to move the drink forward) and the solenoid-pump system to dispense the drink. We may need to 3D print a mount for each bottle. We can laser-cut a wooden display interface for drink selection and notification (to place the OLED and buttons in). We will also need to construct an outer frame for the device, which can be done using wood (since the bottles may be too heavy for cardboard).

### 5. Software Requirements Specification (SRS)

**5.1 Definitions, Abbreviations**

**5.2 Functionality**

| ID     | Description                                                                                                                                                                                                              |
| ------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| SRS-01 | The system shall measure bottle load cell values at least once every 1 second to determine whether sufficient liquid remains for dispensing.                                                                                                                 |
| SRS-02 | The system shall allow the user to select from available drink options using the button interface and display the selected option on the OLED screen within 1 second of input.                                                                                                                                           |
| SRS-03 | The system shall verify that a cup is present at the starting position using the load cell before beginning the drink-making sequence. |
| SRS-04 | At each dispensing station, the system shall activate the corresponding solenoid and pump for the programmed dispense duration associated with the selected drink recipe.                                                                                 |
| SRS-05 | The system shall stop the drink-making process and display an error message if the cup is not detected during any stage of operation. |
| SRS-06 | After the selected drink has been completed, the system shall display a completion message to the user within 2 seconds.                                                                                 |

### 6. Hardware Requirements Specification (HRS)

**6.1 Definitions, Abbreviations**


**6.2 Functionality**

| ID     | Description                                                                                                                        |
| ------ | ---------------------------------------------------------------------------------------------------------------------------------- |
| HRS-01 | An OLED display shall be used to present drink options, status updates, and refill notifications to the user. The display shall have a minimum resolution of 128 × 64 pixels. |
| HRS-02 |A linear motion mechanism shall be used to move the cup between dispensing stations. The mechanism shall provide enough travel distance to position the cup beneath each ingredient outlet.                                                           |
| HRS-03 | A motor shall be used to drive the linear slide and shall provide sufficient torque to move a filled cup reliably through all dispensing stages.      |
| HRS-04 | A solenoid valve and pump assembly shall be used for each ingredient bottle to control liquid dispensing.                       |
| HRS-05 |The outer frame shall be constructed from a rigid material capable of supporting the bottles, pumps, and motion system during operation.                       |

### 7. Bill of Materials (BOM)
[BOM](https://docs.google.com/spreadsheets/d/1Mw4CX4s19iIcAV7RWT3Ym583nlKp-pcjlx95vIQbqWI/edit?usp=sharing)    
  

The RoboBartender system will be centered around an ATmega328PB microcontroller, which serves as the primary controller for the device and runs bare-metal, register-level C firmware. The microcontroller manages all core system operations, including sensor acquisition, actuator control, and communication with the user interface. It was selected because it provides sufficient GPIO pins, multiple hardware timers for PWM generation, and communication peripherals such as SPI and I²C that enable reliable interaction with other components. To measure liquid quantities and detect ingredient availability, the system uses 5 kg strain-gauge load cells placed beneath the cup platform and each ingredient bottle. These sensors measure weight changes, allowing the system to detect cup presence, monitor how much liquid has been dispensed, and determine when bottles are empty. Because load cells output very small millivolt-level signals, HX711 load cell amplifier modules are used to amplify and digitize the signals so they can be read accurately by the microcontroller.

To dispense ingredients, the system uses 12 V peristaltic pumps, which allow controlled liquid flow while keeping the liquid isolated from mechanical components to maintain cleanliness. These pumps are driven using N-channel logic-level MOSFETs, which act as electronic switches so the microcontroller can control higher-current loads safely. A servo motor moves the cup platform between ingredient stations, and its position is controlled using PWM signals generated by the ATmega328PB’s hardware timers. The system provides user interaction through an SSD1306 I²C OLED display and push-button inputs, allowing users to select drink recipes and receive system status messages such as "Place Cup" or "Refill Bottle." Power is supplied by a 12 V regulated power supply, which drives the pumps and other high-power components, while a buck converter steps the voltage down to 5 V for the microcontroller, sensors, and display. Additional passive components such as resistors, capacitors, and flyback diodes are included to stabilize the circuit, filter noise, and protect components from voltage spikes generated by inductive loads like pumps and motors.

### 8. Final Demo Goals
The device will be set on a flat surface for demonstration. A drink will be selected by an audience member, and the device will create the drink. We will show how the device reacts to a drink requiring an amount of liquid from an empty bottle. 

### 9. Sprint Planning

| Milestone  | Functionality Achieved | Distribution of Work |
| ---------- | ---------------------- | -------------------- |
| Sprint #1  |  HRS-02-05             | All team members will work together to design the outer structure. Jeremy will manufacture it. Lori will design + construct the bottle-operating structure. Amy will design + construct the linear slide structure.         |
| Sprint #2  |  SRS-03, SRS-04, SRS-06            |         Lori + Amy will work on SRS-04, SRS-06. Jeremy will work on SRS-03.             |
| MVP Demo   |   All other specifications                     |     Amy and Jeremy work on error handling. Lori works on OLED screen and responsiveness.                 |
| Final Demo |     Stretch features                   |      All team members will work together on stretch features.                |

**This is the end of the Project Proposal section. The remaining sections will be filled out based on the milestone schedule.**

## Sprint Review #1

### Last week's progress
Last week we finalized the BOM and ordered the parts necessary for our project. We also clarified the responsibilties for each of the team members. 

### Current state of project
We have finalized the electrical schematic and the setup for our power modulator and touchscreen LED. We are in the process of CAD-designing our turntable.   

### Next week's plan
Prototype mechanical design and setup software framework (GFX library, controls, etc.) and begin integration. 

## Sprint Review #2

### Last week's progress
We received a lot of our parts (but are still waiting on motors). We updated our schematics and CADs, and are assembling our parts.
### Current state of project
The parts that we have are integrated into the circuit. We are working on the GFX library.
### Next week's plan
Lasercut the structure, acquire bottles for testing, receive + integrate the rest of the parts.
## MVP Demo

### Show a system block diagram & explain the hardware implementation.
![](files/system_diagram_mvp.png)  

The system consists of a 12V battery supplying a diaphragm pump directly and a 5V buck converter powering the ATmega328PB, touchscreen LCD (SPI), RFID module (SPI), and flow sensor (GPIO interrupt), with the pump driven through a MOSFET. The touchscreen LCD provides both user input and visual output, while the RFID module allows new recipes to be loaded, and the flow sensor provides real-time feedback to the microcontroller for precise volume control. Hardware implementation uses SPI for communication with the LCD and RFID (shared bus with separate chip select lines), a MOSFET for safe high-current pump control, and a closed-loop feedback system where the ATmega turns the pump on/off based on flow sensor pulses.


### Explain your firmware implementation, including application logic and critical drivers you've written.

The firmware is structured around a simple state machine that handles user interaction and dispensing logic, with states such as idle, selection, dispensing, and completion. The application reads touch input from the LCD and RFID data to determine the selected recipe, then activates the pump via a GPIO-controlled MOSFET while counting pulses from the flow sensor using an interrupt to precisely measure dispensed volume and stop at the target. Critical drivers include SPI communication for the touchscreen LCD and RFID module, an interrupt-based flow sensor driver for accurate pulse counting, and GPIO control for the pump and any motor drivers.

### Have you achieved some or all of your Software Requirements Specification (SRS)?
SRS-01: We have made slight adjustments to this specification, as instead of load cells, we are using fluid sensors to determine whehter sufficient liquid remains for dispensing. We have a working fluid sensor that can detect the volume of liquid passing through pulse calculations, and we plan on scaling this to 4 tube systems.

SRS-02/06: We are in the process of finishing up the Graphics library for the OLED screen, so we have not completed the drink selection interface yet. 

SRS-03/05: We are in the process of finishing our cup detection mechanism. 

SRS-04: We have a pump and fluid sensor system integrated together, and we plan on scaling to 4 tube systems. 

### Have you achieved some or all of your Hardware Requirements Specification (HRS)?
HRS-01: We are in the process of finishing up the Graphics library for the OLED screen, so we have not completed the display yet, however, we have integrated the hardware and circuity for the OLED screen into our system already. 

HRS-02: We are now using a rotational motion mechanism, a lazy Susan of sorts, to move the cup between dispensing stations. This is complete, apart from a couple screws and standoffs for stability. 

HRS-03: We have finished making the rotating platform and integrated the motor. 

HRS-04: We have finished the pump and fluid sensor assembly for 1 bottle, and we will scale to 4 this week. 

HRS-05: We have finished the outer frame. 

### What is the riskiest part remaining of your project?

The riskiest part remaining of our project is probably the controls in terms of dispensing and pump activation to the specific volume, as well as the I2C communication between the drink selection on the OLED screen to the dispenser. 

### How do you plan to de-risk this?

The priority is to divide the remaining software requirements into modular subsystems so that integration is simplified, and interdependencies are minimized. For instance, using a different clock for the motor and the fluid sensor.   

### What questions or help do you need from the teaching team?
We anticipate needing some guidance in terms of some of our stretch goals within the scope of the week we have left for the project, specifically the RFID integration, so if we foresee limitations, we plan on reaching out to Andrea to communicate them and ask about advice. 


## Final Report

Don't forget to make the GitHub pages public website!
If you’ve never made a GitHub pages website before, you can follow this webpage (though, substitute your final project repository for the GitHub username one in the quickstart guide):  [https://docs.github.com/en/pages/quickstart](https://docs.github.com/en/pages/quickstart)

### 1. Video

### 2. Images

### 3. Results

#### 3.1 Software Requirements Specification (SRS) Results

| ID     | Description                                                                                               | Validation Outcome                                                                          |
| ------ | --------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- |
| SRS-01 | The IMU 3-axis acceleration will be measured with 16-bit depth every 100 milliseconds +/-10 milliseconds. | Confirmed, logged output from the MCU is saved to "validation" folder in GitHub repository. |

#### 3.2 Hardware Requirements Specification (HRS) Results

| ID     | Description                                                                                                                        | Validation Outcome                                                                                                      |
| ------ | ---------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| HRS-01 | A distance sensor shall be used for obstacle detection. The sensor shall detect obstacles at a maximum distance of at least 10 cm. | Confirmed, sensed obstacles up to 15cm. Video in "validation" folder, shows tape measure and logged output to terminal. |
|        |                                                                                                                                    |                                                                                                                         |

### 4. Conclusion


## References


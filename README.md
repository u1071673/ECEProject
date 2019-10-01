# ECEProject
ECE Capstone Project

![alt protoype](https://proxy.duckduckgo.com/iu/?u=https%3A%2F%2Ftse3.mm.bing.net%2Fth%3Fid%3DOIP.GHSA6KYGV2oZg64Pt78JoAHaE8%26pid%3D15.1&f=1 "Prototype")

#### Table of Contents
|             #             | Section                                   | Short Description                                                               |
|:-------------------------:|:------------------------------------------|:-------------------------------------------------------------|
| [1](#1-mission-statement) | [Mission Statement](#1-mission-statement) | Our team's Mission statement                                 |
|   [2](#3-team-members)    | [Team Members](#2-team-members)           | Team members for this project.                               |
| [3](#3-bill-of-materials) | [Bill of Materials](#3-bill-of-materials) | Bill of Materials for the project                            |
| [4](#4-technical-documentation) | [Technical Documentation](#4-technical-documentation) | Datasheets and documentation               |
|   [5](#5-meeting-logs)    | [Meeting Logs](#5-meeting-logs)           | Record of all meeting logs as well as the meeting agenda.    |


## 1. Mission Statement
Our mission is to provide arcade gaming interfaces that are unique, immersive and en-joyable to our users.  These interfaces will provide a more realistic and exciting experiencethrough  unique  physical  interaction.   There  are  few  organizations  who  implement  similartechnologies.  Unlike these organizations, we aim to make an affordable arcade product witha unique immersive experience

## 2. Team Members
* Dezeray Kowalski
* Jamision Bauer
* John Young
* Seth Kingston

## 3. Bill of Materials

https://docs.google.com/spreadsheets/d/1E5jHg0KErL1eJAIMd6SxGYYcoagG-iWO-heWENuRLdU/edit?usp=sharing

## 4. Technical Documentation

#### Motors

https://www.pololu.com/product/1208
https://www.pololu.com/product/1474

#### Motor Control Boards

https://www.pololu.com/product/2134
https://www.pololu.com/product/3096

#### STM32

https://github.com/u1071673/ECEProject/blob/master/Disc_User_Man.pdf
https://github.com/u1071673/ECEProject/blob/master/Chip_Data.pdf
https://github.com/u1071673/ECEProject/blob/master/Core_Man.pdf
https://github.com/u1071673/ECEProject/blob/master/Perph_Ref_Man.pdf

#### Gyro

https://github.com/u1071673/ECEProject/blob/master/GYRO%20l3gd20h.pdf
https://www.digikey.com/catalog/en/partgroup/bno055-9-dof-absolute-orientation-imu-fusion-breakout-board/56944

#### Force Sense Resistors

https://www.mouser.com/ProductDetail/Ohmite/FSR03CE?qs=sGAEpiMZZMve4%2FbfQkoj%252BPOtsLdIyBHHN11TusasqYM%3D

## VERIFIED GPIO PINS (FOR GENERAL PURPOSE OUTPUT MODE)

All of these pins have been verified to work on the STM32 F0 128-KBYTE 32-BIT MCU + 3-AXIS MEM SENSORS Discovery board.


| WORKING PIN (ALSO USED FOR) |
|:----|
| PA0 (Button) (ADC Force Sensor) |
| PA1 (ADC Force Sensor [X]) |
| PA4 (PWM TIMER 14 CH1) (ADC Force Sensor [X])|
| PA5 (ADC Force Sensor [X]) |
| PA8 (Encoder Input TIMER 1 CH1) |
| PA9 (Encoder Input TIMER 1 CH2) |
| PA10 |
| PA15 |
| PB2 |
| PB3 |
| PB4 |
| PB5 [X]| 
| PB6 [X]|
| PB7 [X]|
| PB8 [X]|
| PB9 (PWM TIMER 17 CH1) |
| PB10 |
| PB11 (I2C2_SDA) |
| PB12 |
| PB13 (I2C2_SCL) |
| PB14 (I2C2 SLAVE ADDRESS) |
| PB15 |
| PC0 (SPI/I2C2 MODE SELECT PIN) |
| PC3 (ADC Force Sensor [X]) |
| PC4 (USART3_TX [X]) |
| PC5 (USART3_RX [X]) |
| PC6 (Red LED; Encoder Input TIMER 3 CH1) |
| PC7 (Blue LED; Encoder Input TIMER 3 CH2) |
| PC8 (Orange LED) |
| PC9 (Green LED) |
| PC10 |
| PC12 |
| PC13 |
| PC14 |
| PC15 |
| PF0 |
| PF1 |
| PD2 |

## STAY AWAY FROM THESE PINS! THEY'VE BEEN VERIFIED TO NOT WORK.

All of these pins have been verified to NOT WORK on the STM32 F0 128-KBYTE 32-BIT MCU + 3-AXIS MEM SENSORS Discovery board.

| NOT WORKING |
|:----|
| PA2 |
| PA3 |
| PA6 |
| PA7 |
| PA11 |
| PA12 |
| PA13 |
| PA14 |
| PB0 |
| PB1 |
| PC1 |
| PC2 |

## 5. Meeting Logs

#### Meeting Log for 2/21/19 Thursday

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [X] Dezeray Kowalski
* [X] Jamison Bauer
* [X] John Young
* [X] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
1. Everyone understands how meeting logs work.
2. Implement abstract and mission statement feedback.
3. Assign Tasks.

<!-- What to read before this meeting -->
###### Please Read: N/A
<!-- What to bring before this meeting -->
###### Please Bring: N/A
<!-- Any additional resources to bring before this meeting -->
###### Resources Required: N/A

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda            | Purpose                                                             | Responsible | Mins |
|:------------------|:--------------------------------------------------------------------|:------------|:-----|
| Meeting logs      | Teach how they work.                                                | John Young  | 5    |
| Abstract feedback | Implement feedback into our project proposal                        | Everyone    | 5    |
| Mission Statement | Implement feedback from mission statement into our project proposal | Everyone    | 5    |
| Project Proposal  | Understand what is required by the professor                        | Everyone    | 15   |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility                    | Due Date | Description                                                    |
|:------------|:----------------------------------|:---------|:---------------------------------------------------------------|
| Jamison     | Abstract Implementations          | 2/26/19  | Implement abstract feedback from TA                            |
| Jamison     | Linux Build                       | 3/5/19   | Get a Linux build on his laptop for the open source flight sim |
| Dezeray     | Introduction Outline              | 2/26/19  | Come up with an introduction outline                           |
| Seth        | Mission Statement Implementations | 2/21/19  | Implement mission statement feedback                           |

#### Meeting Log for 2/27/19 Wednesday

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [x] Dezeray Kowalski
* [x] Jamision Bauer
* [x] John Young
* [x] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
1. Outline our presentation slides
2. Talk about presentation logistics

<!-- What to read before this meeting -->
###### Please Read:
<!-- What to bring before this meeting -->
###### Please Bring:
<!-- Any additional resources to bring before this meeting -->
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda               | Purpose                                  | Responsible | Mins |
|:---------------------|:-----------------------------------------|:------------|:-----|
| Discuss Slide Topics | Outline the slides and assign topics out | Everyone    | 30   |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility                 | Due Date       | Description                                                    |
|:------------|:-------------------------------|:---------------|:---------------------------------------------------------------|
|             |                                |                |                                                                |
| Jamison     | Complete assigned slides draft | 3/3/19 evening | Finish your draft of your slides so we can finish up on Monday |
| John        | Complete assigned slides draft | 3/3/19 evening | Finish your draft of your slides so we can finish up on Monday |
| Dezeray     | Complete assigned slides draft | 3/3/19 evening | Finish your draft of your slides so we can finish up on Monday |
| Seth        | Complete assigned slides draft | 3/3/19 evening | Finish your draft of your slides so we can finish up on Monday |

#### Meeting Log for 2/28/19 Thursday

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [x] Dezeray Kowalski
* [x] Jamision Bauer
* [x] John Young
* [x] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
1. Finalize initial milestones.
2. Get everyone on the same page for all slides.
3. Finalize initial individual slides.


<!-- What to read before this meeting -->
###### Please Read: N/A
<!-- What to bring before this meeting -->
###### Please Bring: N/A
<!-- Any additional resources to bring before this meeting -->
###### Resources Required: N/A

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda            | Purpose                                                 | Responsible | Mins |
|:------------------|:--------------------------------------------------------|:------------|:-----|
| Milestones        | Layout Timeline                                         | Jamison     | 10   |
| Individual Slides | Get everyone on the same page for all slides            | John        | 5    |
| Initial Slides    | To get a rough outline we can populate over the weekend | Everyone    | 10   |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description                                                            |
|:------------|:---------------|:---------|:-----------------------------------------------------------------------|
| Everyone    | Do your slides | 3/3/10   | Get your slides done by sunday night so we can have rehearse on monday |


#### Meeting Log for 3/4/19 Monday

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [x] Dezeray Kowalski
* [x] Jamision Bauer
* [x] John Young
* [ ] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
1. Finalize the presentation slides.
2. Practice presenting.
3. Discuss design ideas.

<!-- What to read before this meeting -->
###### Please Read:
<!-- What to bring before this meeting -->
###### Please Bring:
1. Your completed slides.
<!-- Any additional resources to bring before this meeting -->
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
|  Design Ideas | Clarify the design for the physical structure of our project. | John | 20  |
|  Review Slides | Go through entire slide show and make necessary edits. | Jamison | 20  |
|  Practice Presentation | Practice individual sections then practice it all together. | John | 20  |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
| Entire Team | Make necessary changes to presentation. | 3/5  | Some presentation dialog needs to be worked on and practiced. |

#### Meeting Log for 4/16/2019 Tuesday

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [ ] Dezeray Kowalski
* [X] Jamision Bauer
* [X] John Young
* [ ] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
Soldering motor Control Boards 
Wiring 
STM32 writing code
<!-- What to read before this meeting -->
###### Please Read:
<!-- What to bring before this meeting -->
###### Please Bring:
<!-- Any additional resources to bring before this meeting -->
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
|    Code    |     STM32 writing code    |      John       |  2 hrs   |
|    Hardware setup    |     Wiring  /  Soldering motor Control Boards    |      Jamison       |  2 hrs   |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
|      John       |        STM32 writing code        |          |             |

#### Meeting Log for 4/18/2019 Thursday

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [ ] Dezeray Kowalski
* [X] Jamision Bauer
* [X] John Young
* [ ] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
Motor Control
writing stm32 
wiring
<!-- What to read before this meeting -->
###### Please Read:
<!-- What to bring before this meeting -->
###### Please Bring:
<!-- Any additional resources to bring before this meeting -->
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
|    Code    |     STM32 writing code / Motor Control   |      John       |  2 hrs   |
|    Hardware setup    |     Wiring  / Motor Control   |      Jamison       |  2 hrs   |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
|             |                |          |             |

#### Meeting Log for  Monday 4/22/2019

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [ ] Dezeray Kowalski
* [X] Jamision Bauer
* [X] John Young
* [X] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
<!-- What to read before this meeting -->
###### Please Read:
<!-- What to bring before this meeting -->
###### Please Bring:
<!-- Any additional resources to bring before this meeting -->
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
|    Prepare project    |      Prepare Project for Presentation      |    Everyone      |   6 hrs   |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
|             |                |          |             |

#### Meeting Log for 4/23/2019 Tuesday

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [X] Dezeray Kowalski
* [X] Jamision Bauer
* [X] John Young
* [X] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives

<!-- What to read before this meeting -->
###### Please Read:
<!-- What to bring before this meeting -->
###### Please Bring:
<!-- Any additional resources to bring before this meeting -->
Project Hardware and Software
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
|  Presentation      |    Present to Brunvand   |    Everyone  |   2 hrs   |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
|       Everyone      |      Present to Brunvand          |  4/23/2019  |    Present project to Brunvand    |

#### Meeting Log for 4/27/2019 Saturdary 

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [ ] Dezeray Kowalski
* [X] Jamision Bauer
* [X] John Young
* [X] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives

<!-- What to read before this meeting -->
###### Please Read:
<!-- What to bring before this meeting -->
###### Please Bring:
<!-- Any additional resources to bring before this meeting -->
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
|        |         |             |      |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
|             |                |          |             |

#### Meeting Log for 4/29/2019 Monday

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [X] Dezeray Kowalski
* [X] Jamision Bauer
* [ ] John Young
* [X] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
1. Get Presentation Ready for Wednesday May 1, 2019
<!-- What to read before this meeting -->
###### Please Read:
<!-- What to bring before this meeting -->
###### Please Bring:
laptops: to work on presentation
<!-- Any additional resources to bring before this meeting -->
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
|    Presentation  |     Prepare for final on Wednesday 1    |      Everyone       |      |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
|      Jamison       |   Presentation Slides    |    4/30/2019   |    Motor Control and Structure Slides    |
|      John       |   Presentation Slides    |    4/30/2019   |  Flight Gear, STM32, and Sensor Slides    |
|      Dezeray       |   Presentation Slides    |    4/30/2019   | TimeLine and Final Demo Slides   |
|      Seth       |   Presentation Slides    |    4/30/2019   | Project Background and first demo Slides   |
|      Everyone       |   update Website    |    4/31/2019   | update website on the project.   |

#### Meeting Log for 5/18/2019 Saturday

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [X] Dezeray Kowalski
* [X] Jamision Bauer
* [X] John Young
* [X] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
Assign out start of Summer Assignments
<!-- What to read before this meeting -->
###### Please Read:
<!-- What to bring before this meeting -->
###### Please Bring:
laptops: yourself
<!-- Any additional resources to bring before this meeting -->
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
|    Responisibilities  |  Assign them out  |      Everyone       |      |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
|      Jamison       |   Wiring Structure and Hardware    |   5/18/2019  |        |
|      John       |   Flight Gear Position Algorithm    |    5/18/2019   |        |
|      Dezeray       |   IMU/Gyro ordering + data output   | 5/18/2019  |         |
|      Seth       |   FSR's ordering + communicating w/ STM    |   5/18/2019   |  Order up FSR03 from datasheet use lab on it   |
|      Everyone       |   PAY UP   |   5/18/2019  |      |

#### Meeting Log for 8/27/2019 Tuesday

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [X] Dezeray Kowalski
* [X] Jamision Bauer
* [X] John Young
* [X] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
- Assign action items. 
- Reprint melted parts. 
- Bring sensors. 
- Observe the damage of melted parts.
- Reschedule meeting with Prof Stevens.
- Set plan of action.
<!-- What to read before this meeting -->
###### Please Read:
<!-- What to know before this meeting -->
###### Please Bring:
- Structural Pieces
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
|   Meet with Professor Stevens    |   |      |       |
|   Put Structure Together    |   Make sure everything still works    |   whole group   |   x   |


###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
|    Jamison, Dezeray     |   Reprint Parts   |   8/29/2019   |  Reprint melted parts   |
|   Seth    |  Bring Sensors   |   8/28/2019   |   Bring force sensors   |
|   Dezeray |   Bring IMU   |   8/28/2019   |       |
|   John    |  Get Better   |   8/28/2019   |   Heal up   |


#### Meeting Log for 9/3/2019 Tuesday

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [X] Dezeray Kowalski
* [X] Jamision Bauer
* [X] John Young
* [X] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
Meet with Ken
Start Printing Parts
Add Assignments to Project Kanban on Github

<!-- What to read before this meeting -->
###### Please Read:
<!-- What to know before this meeting -->
###### Please Bring:
<!-- Any additional resources to bring for this meeting -->
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
|        |         |             |      |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
|   Seth    |    Bring Force Sensors    |      9/4/2018    |   Bring Force Sensors   |

#### Meeting Log for 9/5/2019 Thursday

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [X] Dezeray Kowalski
* [work at home] Jamision Bauer
* [work at home] John Young
* [X] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
Work on solo assigned work.

<!-- What to read before this meeting -->
###### Please Read:
Your tasks in the 'Project' tab.

<!-- What to know before this meeting -->
###### Please Bring:
<!-- Any additional resources to bring for this meeting -->
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
|   Continue Reprinting Parts     |    restart failed print     |       Dez & Seth      |   5   |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
|     Dezeray        |        IMU library       |    9/12/19      |      Figure out how we can use the provided Adafruit library       |


#### Meeting Log for 09/10/2019 Tuesday

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [x] Dezeray Kowalski
* [x] Jamision Bauer
* [x] John Young
* [x] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
Assemble structure with newly printed parts.
Test force sensing resistors.

<!-- What to read before this meeting -->
###### Please Read:
<!-- What to know before this meeting -->
###### Please Bring:
Any pieces needed for the structure, and FSRs.
<!-- Any additional resources to bring for this meeting -->
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
| Talk about new design for pressue on FSRs | Decide a way to best put pressure on the FSRs for accurate modeling of flight chair | John | 15 min |
| Assemble structure | Make sure things still work & identify any persisting issues | Jamison, John, Dezeray | 20 min |
| Testing of FSRs | See what kind of resistance output we get with different pressures, what to expect | Seth | 10 min |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
| Seth | Code for FSRs | TBD | Figure out reading of pressure from FSRs |
| John | Design new piece for putting pressure on FSRs | 09/17/2019 | |

#### Meeting Log for 09/12/2019 Thursday

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [x] Dezeray Kowalski
* [ ] Jamision Bauer
* [ ] John Young
* [x] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
Continue individual work.
Possibly print new pressure piece.
<!-- What to read before this meeting -->
###### Please Read:
<!-- What to know before this meeting -->
###### Please Bring:
<!-- Any additional resources to bring for this meeting -->
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
| Print new piece | Print the new pressue piece that John desinged | Dezeray | 15 min |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
|             |                |          |             |


#### Meeting Log for 09/17/19 Tuesday

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [Work at Home] Dezeray Kowalski
* [x] Jamision Bauer
* [x] John Young
* [x] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
Continue code & structure work. 

<!-- What to read before this meeting -->
###### Please Read:
<!-- What to know before this meeting -->
###### Please Bring:
<!-- Any additional resources to bring for this meeting -->
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
| Wiring | Get wiring working in the structure | Jamison |      |
| Force Sensors | Work on code | Seth, John |   |
| IMU | Work on getting some reading/communication with libraries | Dezeray

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
|             |                |          |             |


#### Meeting Log for 09/19/19 Thursday

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [x] Dezeray Kowalski
* [ ] Jamision Bauer
* [ ] John Young
* [x] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
Continue Tuesdays work.
<!-- What to read before this meeting -->
###### Please Read:
<!-- What to know before this meeting -->
###### Please Bring:
<!-- Any additional resources to bring for this meeting -->
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
|        |         |             |      |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
|             |                |          |             |



#### Meeting Log for 10/1/19 Tuesday
###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [x] Dezeray Kowalski
* [x] Jamision Bauer
* [x] John Young
* [x] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
Continue work on wiring and code.
<!-- What to read before this meeting -->
###### Please Read:
<!-- What to know before this meeting -->
###### Please Bring:
<!-- Any additional resources to bring for this meeting -->
###### Resources Required:
IMU, FSRs
###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
| IMU Communication | Works with Arduino, figure out libraries with STM | Dezeray, John |      |
| FSR Code | Debug | Seth | |
| Wiring | Put structure together | Jamison | |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
|             |                |          |             |



#### Meeting Log for [Date] [Day of the Week]

###### Attendees:
<!--
* [X] Present
* [ ] Not Present
-->
* [ ] Dezeray Kowalski
* [ ] Jamision Bauer
* [ ] John Young
* [ ] Seth Kingston

<!-- List of objectives to acomplish at meeting -->
###### Meeting Objectives
<!-- What to read before this meeting -->
###### Please Read:
<!-- What to know before this meeting -->
###### Please Bring:
<!-- Any additional resources to bring for this meeting -->
###### Resources Required:

###### Meeting Agenda
<!-- | Agenda Item to go over | What's Purpose/Reason to go over it | Who's responsible for it | How long do we want to spend on it | -->
| Agenda | Purpose | Responsible | Mins |
|:-------|:--------|:------------|:-----|
|        |         |             |      |

###### Action Items
<!-- | Who's responsible for the action item discussed. | When is it due. | Details about the action item | -->
| Responsible | Responsibility | Due Date | Description |
|:------------|:---------------|:---------|:------------|
|             |                |          |             |





PROJECT NAME
============

**University of Pennsylvania, ESE 519: Real Time and Embedded Systems**

* Team Member: Placebo
* [Blog](https://devpost.com/software/project-name) (TODO: Link to devpost submission)

### VIDEO
[![](images/IMG_9743.JPG)]https://www.youtube.com/watch?v=HlDqCCFK9PE

![]
Function explanations:
| Buttons | 
White button is for decreasing distractor distance.
Black button is for increasing distractor distance.
Green button is for committing distractor distance. 
Red button is for immediately shutting off motor moving.
| Pressure sensor |
Wrapping in upper left soft pencil. When force exerting on pressure sensor, motor rotation speed would decrease. Larger the force is, slower would the sWhen force is released, motor rotate at normalized speed.

Inspiration
The bones of the human skull are joined together by cranial sutures. Normally the sutures gradually fuse within the first few years after birth. In infants where one or more of the sutures fuses too early, the growth of the skull is restricted, resulting in limited space for brain and abnormal head shape. In order to stop the unexpected early fuse, cranial distractors are widely used to pull the skull gradually (about 1mm each day) as Figure.1 shows. Parents would use patient screwdriver to rotate the rod of distracters to distract the skull. Distraction phase is 2 turns of 360 degrees for a total of 1mm distraction per day. The distractor is covered under skin with the head of the rod going through the skin and sticking out for operation. 

What it does
A magnetic-driven motorized cranial vault distractor embedded system is designed to cope with these problems. The magnets and distractors are enclosed completely under the skin to reduce the infection. A outside band with stepper motor, microcontroller and sensor will help precisely control the rod rotation. The system has a rapid rotation mode and a slow working mode, which will complete the rotation over a period of time to reduce babys pain. In addition, bluetooth is used to give user an easy way to control distractors.

How I built it
(i) Stepper motor (28BYJ-48 ULN2003 5V stepper motor) (ii) Corresponding motor drive (ULM2003 driver board) (iii) Batteries (Duracell Procell 9 Volt Batteries) (iv) Corresponding battery holders (v) Bluetooth module (HC05) (vi) Microcontroller (Arduino micro) (vii) 3D-printed plastic joints (viii) 6mm diameter Nb-Fe-B magnets () (ix) Bluetooth electric device (Samsung phone)

Challenges I ran into
Accomplishments that I'm proud of
Our whole system can be divided into two sub-systems, an under-skin subsystem and an outside subsystem. The under-skin subsystem contains a distractor and a disk at the end of the rod with magnets mounting on it. The outside subsystem is a head ring fitting a stepper motor to create torque to spin, a disk with magnets to transfer torque, a power supply with power conversion and management module to provide stable and clean power, a sensor to measure the rotation of the system, a Bluetooth module to communicate with device, a microcontroller and all the other electric components for driving and control. The system is required to provide enough torque and a high accuracy. Our first iteration is using a direct connection between inside and outside system and enclosing the motor under the skin for rotation. Then we revised our design to remove the physical connection and came up with the idea of magnetic-driven system. For distracting bones from swivel, an adequate amount of torque need to be supplied from detached arm. The torque need to be large enough to combat with the bones. One of the challenges is to transfer enough torque and indication signals between inside and outside. Magnetic force need to be specially calculated to meet the requirements. Magnets distribution is carefully designed not to draw much attraction force between inside-outside magnets to prevent from skin damage. All components must function well in human body, which means a lot of constraints on choosing the sensor and great effort towards minimizing the size. The stepper motor is used for precise rotation control. Stepper motor is known as an open loop device. To ensure the system is stable and precise, we close the loop by placing a hall effect sensor near outside disk to detect the rotation of magnets. The sensor acts as a wheel encoder to count the number of magnets that pass through. Bluetooth module communicating with microcontroller ensures our system could interface with any device with a standard Bluetooth module. The user can use their device for receiving commands and sending rotation information. We placed five LEDs into our outside skin system to indicate the progress of rotation. There are two modes of operations in our system, the fast rotation mode and slow rotation mode. The previous one has a normal motor rotation speed based on research. It allows user to finish 1mm distraction in less than a minute. The latter slow mode rotation is accomplished by entering the corresponding time period user would like to operate for finishing one 360 degree turn. Commands are sending from bluetooth electric device. Research information shows slower mode of distraction would mitigate the pain of patients. This operation would finish the distraction at a duration of time as user requested.

What's next for Motorized Cranial Vault Distractor
One of the challenge for this project is the size of inside system. Later research can adopt smaller magnets with higher magnetic density, or maybe refined magnets distribution design. Outside system would be optimized in size as well by using printed circuit board. Smaller components can be used, like micro stepper motor. Also, since the stepper motor is open-loop components, other types of motors with closed-loop functionalities can be utilized in further implementation for complex control.
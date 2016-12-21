
#include <AccelStepper.h>
#include <SoftwareSerial.h>// import the serial library
#include <stdlib.h>
#include "string.h"

#define HALFSTEP 8

  /***********motor control*************/
// Motor pin definitions
#define motorPin1  6     // IN1 on the ULN2003 driver 1
#define motorPin2  7     // IN2 on the ULN2003 driver 1
#define motorPin3  8     // IN3 on the ULN2003 driver 1
#define motorPin4  9     // IN4 on the ULN2003 driver 1
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

  /*************bluetooth***************/
SoftwareSerial Bluetooth(11, 12); // RX, TX
char distance[100]; // the data given from Computer
char duration[100]; // the data given from Computer
char BluetoothData[100];

int count = 0;
long int time_interupt;
long int old_time;
char* state;
char _ready[] = {'r','e','a','d','y','\0'};
char _stop[] = {'s','t','o','p','\0'};
int recordPos = 0;
char hour[5];
char minute[5];

  /*************LED*******************/
 // 5 LEDs
const int ledPin5 =  A4;      // the number of the LED pin
const int ledPin4 =  A3;      // the number of the LED pin
const int ledPin3 =  A2;      // the number of the LED pin
const int ledPin2 =  A1;      // the number of the LED pin
const int ledPin1 =  A0;      // the number of the LED pin
float distanceRotated;
float distanceTotal;
double required_distance;
double required_time;
int moveStep;
char start_flag = '0';
unsigned long true_time;
int outsideTick = 0;
int insideTick = 0;
int diskPos = 0;

void setup() {
	  // put your setup code here, to run once:
	  // pin5 PD4 ICP1
          
	  pinMode(5, INPUT);
	  TCCR1B |= 1 << 7;   //Noise
	  TCCR1B |= 1 << ICES1; // zero - falling edge   one - rising edge
	  TCNT1 = 0x00; //clear timer
	  TIFR1 |= 1 << ICF1 ; // clear input capture flag
          

	  //Serial.begin(9600);

	  pinMode(ledPin1, OUTPUT);
	  pinMode(ledPin2, OUTPUT);
	  pinMode(ledPin3, OUTPUT);
	  pinMode(ledPin4, OUTPUT);
	  pinMode(ledPin5, OUTPUT);
	  digitalWrite(ledPin1, HIGH);
	  digitalWrite(ledPin2, HIGH);
	  digitalWrite(ledPin3, HIGH);
	  digitalWrite(ledPin4, HIGH);
	  digitalWrite(ledPin5, HIGH);

	  Serial.begin(115200);

	  delay(2000);
	  Bluetooth.begin(9600);
	  Bluetooth.println("Starting up... ...");
	  //Bluetooth.end();
	  delay(1000);
	 // Bluetooth.begin(9600);//
	  Bluetooth.println("Check the Battery... ...");
	  

	  ADCSRA |= _BV(ADEN);
	  ADMUX	&=	0xf0;
	  ADMUX	|=	0;//PF0 
	  ADCSRA |= _BV(ADSC);//starts the conversion

	  while ( (ADCSRA & _BV(ADSC)) );
	  float Vref = 5.0*ADC/1024;
	  if(Vref >= 4.5){
	  	Bluetooth.println("The Battery is OK... ...");
	  }
	  else{
	  	//while(1){
	  		Bluetooth.println("Please relpace a new Battery... ...");
	  	//}
	  	
	  }

	  delay(1000);
	  Bluetooth.println("Check the LED... ...");
	  delay(500);
	  digitalWrite(ledPin1, HIGH);
	  delay(500);
	  digitalWrite(ledPin1, LOW);
	  digitalWrite(ledPin2, HIGH);
	  delay(500);
	  digitalWrite(ledPin2, LOW);
	  digitalWrite(ledPin3, HIGH);
	  delay(500);
	  digitalWrite(ledPin3, LOW);
	  digitalWrite(ledPin4, HIGH);
	  delay(500);
	  digitalWrite(ledPin4, LOW);
	  digitalWrite(ledPin5, HIGH);
	  delay(500);
	  digitalWrite(ledPin5, LOW); 
	  delay(1000);

	  Bluetooth.println("READY");
	  delay(1000);
	  Bluetooth.end();


	  TIMSK1 |= 1 << ICIE1; //enable input capture interrupt
	  TCCR1B |= 0x01; //start timer
	  sei();
}

void loop() {

	
	switch(start_flag){
		
		case '0':{
			Bluetooth.begin(9600);
   			Bluetooth.println("Ready now, Press <1> quick working, <2> slow working ,<other> waiting ");
   			while(!Bluetooth.available());
   			while (Bluetooth.available()){
   				start_flag = Bluetooth.read();
   			}
   			Bluetooth.end();
   			break;
		}
		case '1':{
			Bluetooth.begin(9600);
   			Bluetooth.println("Starting quick working...");
   			Bluetooth.end();

			/*************bluetooth***************/
			// read control value first from bluetooth

			
			required_distance = read_distance();
			/*Serial.println("Required_distance");
			Serial.println(required_distance);
			Serial.println("required_time");
			Serial.println(required_time);*/
            Bluetooth.println("Distance Set");                      
			/***********motor control*************/
			stepper1.setCurrentPosition(0);
			stepper1.setAcceleration(50.0);
			stepper1.setMaxSpeed(1000.0);
			moveStep = 4096 * required_distance * 2 ;
			stepper1.move(-moveStep);
			float  inputSpeed = -200;
			//Serial.println("inputSpeed");
			//Serial.println(inputSpeed);
			stepper1.setSpeed(inputSpeed);
			/************motor***************/
			Bluetooth.println("motor running");
			Bluetooth.println("in quick mode");
		    //run till hit the point
		    while (stepper1.distanceToGo() != 0 && strcmp(state,_ready) == 0 ) {
			    stepper1.runSpeed();
			    led_light();
			    //read sensor
			    // if more than 260 degree
			    if(stepper1.currentPosition() - recordPos >= 3000) { 
                //record the position of motor interrupted last time and if after that the motor rotated 240 degrees without no new interrupt, stop
			    	state = _stop;
			    	start_flag = 'e';
			        break;
	    		}
  			}
  			if(start_flag = '1'){
	  			Bluetooth.begin(9600);
	   			Bluetooth.println("Work Done... ...");
	   			Bluetooth.end();
	   			start_flag = '0';
  			}
  			break;
  		}
  		case '2':{
			Bluetooth.begin(9600);
   			Bluetooth.println("Starting slow working... ...");
   			Bluetooth.end();

			/*************bluetooth***************/
			// read control value first from bluetooth

			//required_time = read_time();
			required_distance = read_distance();
            read_time();
			/*Serial.println("Required_distance");
			Serial.println(required_distance);
			Serial.println("required_time");
			Serial.println(required_time);*/
			Bluetooth.println("Distance Set"); 
			/***********motor control*************/
			stepper1.setCurrentPosition(0);
			stepper1.setAcceleration(50.0);
			stepper1.setMaxSpeed(1000.0);
			moveStep = 4096 * required_distance * 2 ;//
			stepper1.move(-moveStep);
			float  inputSpeed = -200.0;
			//Serial.println("inputSpeed");
			//Serial.println(inputSpeed);
			stepper1.setSpeed(inputSpeed);
			Bluetooth.println("motor running");
   			Bluetooth.println("in slow mode");
			/************motor***************/
		    //run till hit the point
		    while (stepper1.distanceToGo() != 0 && strcmp(state,_ready) == 0 ) {
			    stepper1.runSpeed();
			    delay(true_time/moveStep);
			    //Serial.println("Between Run and led_light");
			    led_light();
			    //read sensor
			    // if more than 260 degree
			    if(stepper1.currentPosition() - recordPos >= 3000) { //record the position of motor interrupted last time and if after that the motor rotated 240 degrees without no new interrupt, stop
			    	state = _stop;
			    	start_flag = 'e';
			        break;
	    		}
  			}
  			if(start_flag = '2'){
	  			Bluetooth.begin(9600);
	   			Bluetooth.println("Work Done... ...");
	   			Bluetooth.end();
	   			start_flag = '0';
  			}
  			break;
  		}
  		case 'e':{
  			Bluetooth.begin(9600);
   			Bluetooth.println("ERROR ... ERROR ...");
   			Bluetooth.println("Please reset... ...");
   			Bluetooth.end();
   			//start_flag = '0';
   			break;

  		}
  		default:{
			Bluetooth.begin(9600);
   			Bluetooth.println("Waiting for working... ...");
   			Bluetooth.end();
   			start_flag = '0';
   			break;
		}
	}

}


ISR(TIMER1_CAPT_vect){

  	recordPos = stepper1.currentPosition();

}


void led_light(){
  long int motorPos = -stepper1.currentPosition();
  if ( motorPos < 0.2 * moveStep){
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
    digitalWrite(ledPin5, LOW);
  }
  else if ( (motorPos>=0.2 * moveStep )&& (motorPos < 0.4 * moveStep)){
    digitalWrite(ledPin1, HIGH);
  }
  else if ((motorPos>=0.4 * moveStep )&& (motorPos < 0.6 * moveStep)){
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
  }
  else if ((motorPos>=0.6 * moveStep )&& (motorPos < 0.8 * moveStep)){
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);   
  }
  else if ((motorPos>=0.2 * moveStep )&& (motorPos < 1.0 * moveStep)){
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin4, HIGH);
  }
  else {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin4, HIGH);
    digitalWrite(ledPin5, HIGH);
  }
}



double read_distance(){
   // read distance
    Bluetooth.begin(9600);
    Bluetooth.println("Setting rotation distance(in mm)");
    int index = 0;
    while(!Bluetooth.available());
    while (Bluetooth.available()){
          	state = _ready;
    	int temp_distance = Bluetooth.read();
   		memset(BluetoothData,0,sizeof(distance));
   		distance[index] = temp_distance;
    	index ++;
    }
   double input_distance = atof(distance);
   //Serial.println("input_distance");
   //Serial.println(input_distance);
   Bluetooth.end();
   return input_distance;
}


void read_time(){
    // read time
    Bluetooth.begin(9600);
    Bluetooth.println("Setting the whole work time :");
    Bluetooth.println("Please input the Hours(0-24)");
    int   index = 0;
    while(!Bluetooth.available());
    //memset(hour,0,sizeof(hour));
    while (Bluetooth.available()){

   		int temp_hour = Bluetooth.read();
   		hour[index] = temp_hour;
      	index ++;
    }
    Bluetooth.end();
    delay(10);
    Bluetooth.begin(9600);
    Bluetooth.println("Please input the Minutes(1-60)");  
    int   index1 = 0;
    while(!Bluetooth.available());
    //memset(minute,0,sizeof(minute));
    while (Bluetooth.available()){
   		int temp_minute = Bluetooth.read();
   		minute[index1] = temp_minute;
      	index1 ++;
    }
    char *hour1 = hour;
    char *minute1 = minute;

    float true_hour = atof(hour1);
    float true_minute = atof(minute1);
    true_time = 60*60*true_hour*1000 + true_minute*60*1000;
    Bluetooth.end();
}





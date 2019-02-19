/*
   YF-S201 Hall Effect Water Flow Meter / Sensor
   http://www.hobbytronics.co.uk/yf-s201-water-flow-meter
   
   Read Water Flow Meter and output reading in litres/hour
*/
#include <Bridge.h>

unsigned char flowmeter = 7;  // Flow Meter Pin number

const int average_num = 15;
volatile int  flow_frequency;  // Measures flow meter pulses
unsigned long  l_hour;          // Calculated litres/hour                      
unsigned long currentTime;
unsigned long cloopTime;
unsigned int  counter = 0;
unsigned long  FLOW = 0;

void flow (){                  // Interruot function
   flow_frequency++;
} 

void setup(){ 
   pinMode(13,OUTPUT);       
   pinMode(flowmeter, INPUT);
   attachInterrupt(digitalPinToInterrupt(flowmeter), flow, RISING); // Setup Interrupt 
   sei();                            // Enable interrupts  
   currentTime = millis();
   cloopTime = currentTime;

   Serial.begin(9600); 
   Bridge.begin();   // D0 and D1 are used by the Bridge library.
   counter = 0;
} 

void loop (){
    char D13[2];
    char valueStr[23];
   
    Bridge.get("Reg_done",  D13, 2);
    digitalWrite(13, atoi(D13));       

    currentTime = millis();
   // Every second, calculate and print litres/hour
    if(currentTime >= (cloopTime + 1000)){     
        cloopTime = currentTime;              // Updates cloopTime
        // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min. (Results in +/- 3% range)
        l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flow rate in L/hour 
        flow_frequency = 0;                   // Reset Counter
        Serial.print(l_hour, DEC);            // Print litres/hour
        Serial.println(" L/hour");
        FLOW += l_hour;
        counter ++;

        if (counter >= average_num){
            dtostrf(FLOW/float(average_num), 5, 3, valueStr);
            Bridge.put("Waterflow", valueStr);
            Serial.print(valueStr);            // Print litres/hour
            Serial.println(" average L/hour");            
            FLOW = 0;
            counter = 0;
        }
    }
}

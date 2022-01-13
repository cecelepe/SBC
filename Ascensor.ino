
///////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// Variables LEDS //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

const int ledPin0 = 4;
const int ledPin1 = 0;
const int ledPin2 = 2;
//CO2
const int ledPin3 = 17; //verde 
const int ledPin4 = 5; //rojo
//celula carga
const int ledPin5 = 16;

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////      Variables Sensirion     //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "esp_timer.h"
#include <Wire.h>
#include "SCD30.h"
#include "Sensirion_GadgetBle_Lib.h"

static int64_t lastMmntTime = 0;
static int startCheckingAfterUs = 1900000;

GadgetBle gadgetBle = GadgetBle(GadgetBle::DataType::T_RH_CO2_ALT);

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////      Variables Celula de carga     ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include <SPI.h>
#include <Wire.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////      Variables 3 sensores Ultrasonido     /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
const int trigPin0 = 32;
const int echoPin0 = 33;

const int trigPin1 = 25;
const int echoPin1 = 26;

const int trigPin2 = 27;
const int echoPin2 = 14;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define PISO0 0
#define PISO1 1
#define PISO2 2

int pisoAscensor = PISO1;
long duration0;
long duration1;
long duration2;
float distanceCm0;
float distanceCm1;
float distanceCm2;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Fin Variables  /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////




void setup() {
   Serial.begin(115200);


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////Setup LEDS////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
     // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledPin0, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  //CO2
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  //celula de carga 
  pinMode(ledPin5, OUTPUT);
  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////    SETUP Sensirion   //////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////
 
  delay(100);
  // Initialize the GadgetBle Library
  gadgetBle.begin();
  Serial.print("Sensirion GadgetBle Lib initialized with deviceId = ");
  Serial.println(gadgetBle.getDeviceIdString());

  // Initialize the SCD30 driver
  Wire.begin();
  scd30.initialize();
  scd30.setAutoSelfCalibration(1);

  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////    SETUP celula de carga   ////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////
 
  //Serial.begin(115200);
  //pin numero 34

  //////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////// SETUP 3 sensores Ultrasonido //////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////
 
  // Starts the serial communication
  pinMode(trigPin0, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin0, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);



}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Fin Setup  /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {

  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////    Loop Sensirion   ///////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////

 float result[3] = {0};

  if (esp_timer_get_time() - lastMmntTime >= startCheckingAfterUs) {

    if (scd30.isAvailable()) {
      scd30.getCarbonDioxideConcentration(result);

      gadgetBle.writeCO2(result[0]);
      gadgetBle.writeTemperature(result[1]);
      gadgetBle.writeHumidity(result[2]);

      gadgetBle.commit();
      lastMmntTime = esp_timer_get_time();

      // Provide the sensor values for Tools -> Serial Monitor or Serial Plotter
      Serial.print("CO2[ppm]:");
      Serial.print(result[0]);
      Serial.print("\t");
      Serial.print("Temperature[℃]:");
      Serial.print(result[1]);
      Serial.print("\t");
      Serial.print("Humidity[%]:");
      Serial.println(result[2]);
      //C02 
      if ((result[0])<1000){
        digitalWrite(ledPin4, LOW);
       digitalWrite(ledPin3, HIGH); //verde 
       
      }
       else  {
        digitalWrite(ledPin3, LOW);
        digitalWrite(ledPin4, HIGH); //rojo
       }
    }
  }

  gadgetBle.handleEvents();
  delay(3);

  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////    Loop celula de carga  //////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////

  analogSetWidth(9);
  analogSetAttenuation(ADC_11db);
  analogSetPinAttenuation(34, ADC_11db);
  Serial.println(analogRead(34));
  if (analogRead(34)>400){
    digitalWrite(ledPin5, HIGH);
  }
  else {
    digitalWrite(ledPin5, LOW);  }

  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////    Loop 3 sensores Ultrasonido   //////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////
 
//ULTRASONIDO INICIO 0
  // Clears the trigPin
  digitalWrite(trigPin0, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin0, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin0, LOW);
 
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration0 = pulseIn(echoPin0, HIGH);
 
  // Calculate the distance
  distanceCm0 = duration0 * SOUND_SPEED/2;
 
  // Prints the distance in the Serial Monitor
  Serial.print("Distance 0 (cm): ");
  Serial.println(distanceCm0);
  if(distanceCm0<100){
    Serial.println("No cerrar");
    digitalWrite(ledPin0, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(500); //medio segundo 
  }
  else {
   Serial.println("Cerrando");
   digitalWrite(ledPin0, LOW);   // turn the LED on (HIGH is the voltage level)
 // delay(500); 
  }
// delay(500);
//ULTRASONIDO FIN


//ULTRASONIDO INICIO 1
  // Clears the trigPin
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
 
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration1 = pulseIn(echoPin1, HIGH);
 
  // Calculate the distance
  distanceCm1 = duration1 * SOUND_SPEED/2;
 
  // Prints the distance in the Serial Monitor
  Serial.print("Distance 1 (cm): ");
  Serial.println(distanceCm1);
  if(distanceCm1<100){
    Serial.println("No cerrar");
    digitalWrite(ledPin1, HIGH);   // turn the LED on (HIGH is the voltage level)

  }
  else {
   Serial.println("Cerrando");
   digitalWrite(ledPin1, LOW);   // turn the LED on (HIGH is the voltage level)
  }
 
 delay(500);
//ULTRASONIDO FIN

//ULTRASONIDO INICIO 2
  // Clears the trigPin
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
 
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration2 = pulseIn(echoPin2, HIGH);
 
  // Calculate the distance
  distanceCm2 = duration2 * SOUND_SPEED/2;
 
  // Prints the distance in the Serial Monitor
  Serial.print("Distance 2 (cm): ");
  Serial.println(distanceCm2);
  if(distanceCm2<100){
    Serial.println("No cerrar");
    digitalWrite(ledPin2, HIGH);   // turn the LED on (HIGH is the voltage level)

  }
  else {
   Serial.println("Cerrando");
   digitalWrite(ledPin2, LOW);   // turn the LED on (HIGH is the voltage level)
  }
 
//delay(500);
//ULTRASONIDO FIN

}



  /////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////  Fin del loop   /////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////

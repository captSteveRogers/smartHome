//Blynk App essential Libraries
#define BLYNK_PRINT Serial
#include<ESP8266WiFi.h>
#include<BlynkSimpleEsp8266.h>

//Servo motor config
#include<Servo.h>
Servo servo;
BLYNK_WRITE(V2){
  servo.write(param.asInt());
}

//Libraries and utilities for DHT 11
#include<DHT.h>            //Library for DHT 11 temp and humidity sensor. Works perfectly with 1.2.3
#define dhtPin 13           //13 stands for D7
#define DHTTYPE DHT11       //DHT 11
DHT dht(dhtPin, DHTTYPE);
BlynkTimer timer;

//Blynk app and Wifi configuration
char auth[] = "315028c2ad07466d829ddecf8dbaada4";
char ssid[] = "TonyStark";
char pass[] = "ashu0810";

//Ultrasonic sensor HC-SR04 config
int trigPin = D5;           //pin which throws the ping
int echoPin = D6;           //pin which catches the reflected ping

// Function handling the input of PIR sensor.
float distance(){
    digitalWrite(trigPin,LOW);                                //intital low signal for the pulse
    delayMicroseconds(2000);                                  //For the signal to settle
    digitalWrite(trigPin,HIGH);                               //___---___ for the pulse
    delayMicroseconds(20);                                    //time in high state
    digitalWrite(trigPin,LOW);                                //Signal to go low, pulse generated
    delayMicroseconds(10);                                    //Signal to settle
    float speedOfSound=332./25000. ;                          //speed of sound converted to inches/microsec

    float pingTime=pulseIn(echoPin,HIGH);                     //the time for the ping to return

    float targetDist = speedOfSound * pingTime ;
    Serial.print("The target distance is: ");
    Serial.println(targetDist);

    return targetDist;
    //Blynk.notify("Motion Detected");
  
}

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
 
  if (isnan(h) || isnan(t)) {                                     //checks whether the acquired value is a number or not
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  
}
//
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);             //Start the serial monitor
  Blynk.begin(auth, ssid, pass);  //Start the bynk server
  dht.begin();                    //Start the DHT11 sensor
  servo.attach(D8);               //attach the servo to D8
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  timer.setInterval(1000L, sendSensor);     //Sending data to Blynk app at an interval of 1 s
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
  float dist = distance();
  if(dist < 30.){
    Blynk.notify("Warning : Intruder detected");
    //Blynk.email("shantam.ashu0810@gmail.com", "Intruder Alert!!!", "Someone approaching");
  }
  delay(1000);
   
}

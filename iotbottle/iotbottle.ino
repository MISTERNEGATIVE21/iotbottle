
#include <ConsentiumThings.h>
#include <NewPing.h>
const int switchPin = 5;
const int LEDPIN = 12;
const int lm35Pin = 36;
#define TRIGGER_PIN  32  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     33 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters).
#define BUZZER_PIN   25  // Arduino pin tied to the buzzer.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
unsigned long start_time = 0; // variable to store the start time of the measurement
unsigned long last_distance_time = 0; // variable to store the time when the distance was last measured
unsigned int last_distance = 0; // variable to store the last measured distance


ConsentiumThings board;   // create ConsentiumThing object


const char *ssid = "Utsavwifi"; // add WiFi SSID
const char *pass = "12345678"; // add WiFi password
const long interval = 5; // take 5 seconds of delay 
const char *key = "9JMDGMONTM4VAMXBN7WSQW";       // Write api key

void setup(){
  board.begin();   // init. IoT boad

  board.initWiFi(ssid, pass);  // begin WiFi connection
   pinMode(LEDPIN,OUTPUT);

 pinMode(switchPin ,INPUT_PULLUP);
   pinMode(BUZZER_PIN, OUTPUT); // Set buzzer pin as output

}

void loop(){
	float t= lm35();
	float h= depth();
  switchcode();

    float sensor_val[] = {t, h};  // sensor data array
    String info_buff[] = {"Temperature", "Height"}; // sensor info. array
  
    int sensor_num = sizeof(sensor_val)/sizeof(sensor_val[0]); // number of sensors connected 
  
    board.sendREST(key, sensor_num, info_buff, sensor_val, LOW_PRE, interval); // send over REST with delay with desired prescision
}

float lm35(){

  float voltage = analogRead(lm35Pin) * (3.3 / 4096.0);
  float temperature = (voltage - 0.5) * 100.0;

  Serial.print("Temperature: ");

  Serial.println(" C");
  return temperature;
}

void switchcode() {
 

  // digitalRead function stores the Push button state 
// in variable push_button_state
int switchPin_state = digitalRead( switchPin);
  Serial.println(" SwitchPin");
// if pressed LED will turn on otherwise remain off 
if (  switchPin_state == 0 )
{ 
digitalWrite(LEDPIN, HIGH); 
}
else 
{
digitalWrite(LEDPIN, LOW); 
}
}

float depth() {
  unsigned int distance = sonar.ping_cm(); // Send ping, get distance in cm and print result (0 = outside set distance range).
  
  if (distance == 0) {
    Serial.println("Out of range");
  } else {
    Serial.print(distance);
    Serial.println(" cm");
  }

  // check if the distance has changed
  if (distance != last_distance) {
    // reset the last distance time
    last_distance_time = millis();
    // store the new distance
    last_distance = distance;
  }

  // check if the distance has been the same for 2 minutes
  if (millis() - last_distance_time >= 2 * 60 * 1000 && last_distance != 0) {
    // play the buzzer
    digitalWrite(BUZZER_PIN, HIGH);
    // wait for 1 second
    delay(1000);
    // turn off the buzzer
    digitalWrite(BUZZER_PIN, LOW);
    // reset the last distance time
    last_distance_time = millis();
  }

  // delay for 100 milliseconds before taking another measurement
  delay(100);
  return distance ;
}
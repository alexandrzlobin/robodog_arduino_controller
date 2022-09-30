#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32.h>

ros::NodeHandle nh;

std_msgs::Float32 butt_msg;
ros::Publisher pub_batt("battery", &butt_msg);

float Vout = 0.00;
float Vin = 0.00;
float R1 = 100000.00; // resistance of R1 (100K) 
float R2 = 10000.00; // resistance of R2 (10K) 
int value = 0;
int voltageInput = 0;
float voltage;
long last_time = 0;
const int battery_time = 5000;
void setup(){
  Serial.begin(115200);
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.advertise(pub_batt);
}

void loop(){
  if ((millis() - last_time) > battery_time) {
    float perc = calcVoltage();
    publishBattery(perc);
    Serial.println(perc);
    last_time = millis();
  }
  nh.spinOnce();
}

float calcVoltage(){
  value = analogRead(voltageInput);
  Vin = (value * 5.0)/1023;
  Vin = Vout / (R2/(R1+R2));
  return map(Vin, 0, 33.6, 0, 100);
}

void publishBattery(float perc){
  butt_msg.data = perc;
  pub_batt.publish( &butt_msg );
}


#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <ros.h>
#include <geometry_msgs/Twist.h>

// Pin variables for motors.
const int right_pwm_pin = 5;
const int right_dir_pin1 = 7;
const int right_dir_pin2 = 8;
const int left_pwm_pin = 6;
const int left_dir_pin1 = 9;
const int left_dir_pin2 = 10;
const bool left_fwd = true;
const bool right_fwd = true;

// Default_speed.
const int default_vel = 100;
int state_vel = default_vel;
const int max_vel = 255;
bool right=false;
bool left=false;
ros::NodeHandle  nh;
void MoveStop()
{
   digitalWrite(right_dir_pin1, LOW);
  digitalWrite(right_dir_pin2, LOW); 
  digitalWrite(left_dir_pin1, LOW);
  digitalWrite(left_dir_pin2, LOW);
  analogWrite(right_pwm_pin, 0);
  analogWrite(left_pwm_pin, 0);
}

void MoveDifferential(float x, float z_rot) {

  float right_vel = 0;
  float left_vel = 0;
  bool right_f,right_b,left_f,left_b;
  if (x>0 && z_rot == 0.0) {
    right_f=true;
    right_b=false;
    left_f=true;
    left_b=false;
    right_vel = max_vel * x;
    left_vel = max_vel * x;
    nh.loginfo("Forward");
  }
  else if (z_rot > 0.0) {
        right_f=true;
    right_b=false;
    left_f=true;
    left_b=false;
   left_vel = max_vel * x;
    right_vel = max_vel * x - 2.0 * max_vel * z_rot * x;
    nh.loginfo("forward and Left");
  }
  else if (z_rot < 0.0) {
        right_f=true;
    right_b=false;
    left_f=true;
    left_b=false;
    right_vel = max_vel * x;
    left_vel = max_vel * x - 2 * max_vel * abs(z_rot) * x;
    nh.loginfo("forward and Right");
  }
  else {
    right_f=false;
    right_b=false;
    left_f=false;
    left_b=false;
    right_vel = 0.0;
    left_vel = 0.0;
    nh.loginfo("Stop");
  }

  if (x == 0 && z_rot != 0) {
   right_f=(z_rot > 0) ? true : false;
   right_b=(z_rot > 0) ? false : true;
   left_f=(z_rot > 0) ? false : true;
   left_b=(z_rot > 0) ? true : false;
   right_vel = z_rot * max_vel;
   left_vel  = z_rot * max_vel;
   if(right_f) nh.loginfo("Left");
   else nh.loginfo("Right");
  }
  
  digitalWrite(right_dir_pin1, (right_f) ? HIGH : LOW);
  digitalWrite(right_dir_pin2, (right_b) ? HIGH : LOW); 
  digitalWrite(left_dir_pin1, (left_f) ? HIGH : LOW);
  digitalWrite(left_dir_pin2, (left_b) ? HIGH : LOW);
  analogWrite(right_pwm_pin, abs((int) right_vel));
  analogWrite(left_pwm_pin, abs((int) left_vel));
}

void cmd_vel_cb(const geometry_msgs::Twist & msg) {
  // Read the message. Act accordingly.
  // We only care about the linear x, and the rotational z.
  const float x = msg.linear.x;
  const float z_rotation = msg.angular.z;
  MoveDifferential(x,z_rotation);
}


ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", cmd_vel_cb);

void setup() {

  pinMode(right_pwm_pin, OUTPUT);    // sets the digital pin 13 as output
  pinMode(right_dir_pin1, OUTPUT);
  pinMode(right_dir_pin2, OUTPUT);
  pinMode(left_pwm_pin, OUTPUT);
  pinMode(left_dir_pin1, OUTPUT);
  pinMode(left_dir_pin2, OUTPUT);
  // Set initial values for directions. Set both to forward.
  pinMode(13, OUTPUT);
  // Send forward command.
  MoveStop();

  nh.initNode();
  nh.subscribe(sub);



}

void loop() {
  nh.spinOnce();
  delay(1);
}

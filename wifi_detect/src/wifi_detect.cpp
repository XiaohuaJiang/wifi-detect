#include "ros/ros.h"
#include "std_msgs/String.h"
#include "wifi_detect/Signal.h"
#include <sstream>
#include "stdlib.h"
#include <fstream>
#include <iostream>
#include <math.h>
using namespace std;
/* allowed format: -.4; +.4; -1.2; 2.5;35.;+35.  
     */  
     float atod(string str) {  
          
        bool negative = false;  
        //get the value before the "."  
        float valueBeforeDot = 0.0d;  
        //get the value after the ".";  
        float valueAfterDot = 0.0d;  
        bool pointAppear = false;  
        int count = 0;  
          
        for (int i = 0; i < str.length(); i++) {  
            //check whether the first character is "+" or "-"  
            if (i == 0 && (str.at(0) == '-' || str.at(0) == '+')) {  
                if (str.at(0) == '-') {  
                    negative = true;  
                    continue;  
                }  
            }  
            //check whether the character is "." and appears for   
            //the first time and appears at the correct position.  
            if (pointAppear == false && str.at(i) == '.') {  
                pointAppear = true;  
                continue;  
            }   
            if (str.at(i) >= '0' && '9' >= str.at(i)) {  
                if (pointAppear == false) {  
                    valueBeforeDot = valueBeforeDot * 10 + (str.at(i) - '0');  
                } else {  
                    valueAfterDot = valueAfterDot * 10 + (str.at(i) - '0');  
                    count++;  
                }  
            } 
        }  
        valueBeforeDot = valueBeforeDot + valueAfterDot /(10^count);  
        return negative == true ? valueBeforeDot * -1 : valueBeforeDot;               
    }   
/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line. For programmatic
   * remappings you can use a different version of init() which takes remappings
   * directly, but for most command-line programs, passing argc and argv is the easiest
   * way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "wifi_detect");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */
  ros::Publisher chatter_pub = n.advertise<wifi_detect::Signal>("wifi_signal", 1000);

  ros::Rate loop_rate(10);

  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  int count=0;
  while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
    std_msgs::String msg;
    wifi_detect::Signal signal;
    std::stringstream ss;
    string wifi_signal[37];
    float quality,level;
    ifstream file;
    file.open("/proc/net/wireless",ios::in);
    for(int i=0;i<37;i++)
    {file>>wifi_signal[i];}
    quality=atod(wifi_signal[29]);
    level=atod(wifi_signal[30]);
    signal.quality=quality;
    signal.level=level;	  
    ss <<"Link Quality is "<<quality<<" Signal level is  "<<level<<" dmB"<<count;
    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
    msg.data = ss.str();

   // ROS_INFO("%s", msg.data.c_str());
    chatter_pub.publish(signal);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
   
  }


  return 0;
}

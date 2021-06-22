#include "ros/ros.h"
#include "std_msgs/String.h"  
#include "std_msgs/UInt32.h"
#include <sstream>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <stdint.h>
#include <strings.h>
#include <arpa/inet.h>
#include"iostream"

//void delay(void);

#define SET_DESTINATION     "192.168.1.123"   //must be the same as the local ip , if you dont sprcify it will use the avalbel ip 
#define SET_MY_IP_ADDRESS   "192.168.1.106"
#define destination_port 6000
#define Set_point_port   8000

uint8_t set_point_buffer [1000];

int SET_socket;
struct sockaddr_in SET_dest; 
struct sockaddr_in SET_My_IP;
socklen_t ccsize=sizeof(SET_dest);


extern void Set_Point_Callback(const std_msgs::UInt32::ConstPtr& msg);
extern void Init_Eth_Socket_set_point(void);
extern void start_connection_set_point(void);
extern void send(uint8_t * buffer , uint16_t size);
extern void delay_2(void);
/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void Set_Point_Callback(const std_msgs::UInt32::ConstPtr& msg)
{
  //ROS_INFO("I heard: [%s]", msg->data.c_str());
  ROS_INFO("I heard: [%x]", msg->data);
  uint8_t  buffer[4];
  memcpy(buffer ,&(msg->data),4);
  send( buffer , 4);
}

int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "setpoint");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;
  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
  ros::Subscriber set_point = n.subscribe("set_point", 1000, Set_Point_Callback);
   
  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  //ros::Rate loop_rate(5);
  //std_msgs::UInt32 msg;
  start_connection_set_point();
  ros::spin();

  return 0;
}

void send(uint8_t * buffer , uint16_t size)
{
send(SET_socket,buffer,size,0);
}
void start_connection_set_point(void)
 {
   Init_Eth_Socket_set_point();
   send(set_point_buffer , 2);
   send(set_point_buffer , 2);
   delay_2();
   send(set_point_buffer , 2);
   delay_2();
   send(set_point_buffer , 2);
 }
void Init_Eth_Socket_set_point(void)
{
 SET_socket=socket(AF_INET, SOCK_DGRAM, 0); //socket created
 bzero(&SET_My_IP, sizeof(SET_My_IP));
 SET_My_IP.sin_family=AF_INET;
 SET_My_IP.sin_port=htons(Set_point_port);

 SET_dest.sin_family=AF_INET;
 SET_dest.sin_port=htons(destination_port);

 inet_aton(SET_MY_IP_ADDRESS , &SET_My_IP.sin_addr);
 inet_aton(SET_DESTINATION , &SET_dest.sin_addr);

 bind(SET_socket, (struct sockaddr*)&SET_My_IP, sizeof(SET_My_IP));
 int connect_socket=connect(SET_socket,(struct sockaddr*)&SET_dest,sizeof(SET_dest));
}
void delay_2(void)
{
 int c, d;
   
   for (c = 1; c <= 32767; c++)
   {
       for (d = 1; d <= 32767; d++)
       {

       }
   }
}
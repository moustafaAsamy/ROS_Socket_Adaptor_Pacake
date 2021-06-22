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

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
using namespace std;
uint8_t flag =0;
extern void Init_Eth_Socket(void);
// extern void delay(void);
extern void send(uint8_t * buffer , uint16_t size);
extern void recive(uint8_t * buffer , uint16_t size);

extern uint8_t OUT_BUFFER[1024];
extern uint8_t IN_BUFFER[1024];
extern uint8_t OUT_BUFFER[1024];
extern uint8_t IN_BUFFER[1024];
extern void start_connection(void);
void delay(void);

#define DESTINATION     "192.168.1.123"   //must be the same as the local ip , if you dont sprcify it will use the avalbel ip 
#define MY_IP_ADDRESS   "192.168.1.106"

uint8_t OUT_BUFFER[1024]={0};
uint8_t IN_BUFFER[1024]={0};

int network_socket;
struct sockaddr_in dest; 
struct sockaddr_in My_IP;
socklen_t csize=sizeof(dest);


//ros::Rate delay(1);
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
  ros::init(argc, argv, "udp_socket_adaptor");

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
  //ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);

  ros::Publisher sensor_reading = n.advertise<std_msgs::UInt32>("sensor_reading", 1000);

  //ros::Rate loop_rate(10);

 // ros::Rate delay(1);


  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  int count = 0;
  start_connection();
  ros::Rate loop_rate(5);
  while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
    //std_msgs::String msg;

    std_msgs::UInt32 msg;

    // std::stringstream ss;
     //ss << "hello world gg " << count;
     
     //string word_ ="0";
    //msg.data = count;

    //ROS_INFO("%c", msg.data); //to print on screen 
    ROS_INFO( " %x " ,msg.data); //to print on screen 
    //ROS_INFO( " %x  %x  %x  %x "IN_BUFFER[0],IN_BUFFER[1],IN_BUFFER[2],IN_BUFFER[3]);
    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
    //recive(IN_BUFFER ,  1);
    send(OUT_BUFFER , 2);
    msg.data=     ( (IN_BUFFER[0]) | (IN_BUFFER[1]<<8)| (IN_BUFFER[2]<<16) |(IN_BUFFER[3]<<24));
    cout<<msg.data;
    sensor_reading.publish(msg);
    recive(IN_BUFFER ,  4);

    ros::spinOnce();

    loop_rate.sleep();
    //++count;
  }
 

  return 0;
}

 void start_connection(void)
 {
   Init_Eth_Socket();
  //  delay();
  //  send(OUT_BUFFER , 2);
  //  delay();
  //  send(OUT_BUFFER , 2);
  //   delay();
   send(OUT_BUFFER , 2);
  //  delay();
   send(OUT_BUFFER , 2);
  //   delay();
   send(OUT_BUFFER , 2);
  //  delay();
   send(OUT_BUFFER , 2);

  //  while(!flag)
  //  {
  //  recive(IN_BUFFER ,  1);
  //  if (IN_BUFFER[0] == 88)
  //  {
  //    flag =1;
  //  }
  //  }
  // recive(IN_BUFFER ,  1);
  // recive(IN_BUFFER ,  1);
 }
void Init_Eth_Socket(void)
{
 network_socket=socket(AF_INET, SOCK_DGRAM, 0); //socket created
 bzero(&My_IP, sizeof(My_IP));
 My_IP.sin_family=AF_INET;
 My_IP.sin_port=htons(9807);

 dest.sin_family=AF_INET;
 dest.sin_port=htons(9007);

 inet_aton(MY_IP_ADDRESS , &My_IP.sin_addr);
 inet_aton(DESTINATION , &dest.sin_addr);

 bind(network_socket, (struct sockaddr*)&My_IP, sizeof(My_IP));
 int connect_socket=connect(network_socket,(struct sockaddr*)&dest,sizeof(dest));
}

void delay(void)
{
 int c, d;
   
   for (c = 1; c <= 32767; c++)
   {
       for (d = 1; d <= 32767; d++)
       {

       }
   }
}

void send(uint8_t * buffer , uint16_t size)
{
send(network_socket,buffer,size,0);
}

void recive(uint8_t * buffer , uint16_t size)
{
int n=recvfrom(network_socket,buffer,size,0,(struct sockaddr*)&dest, &csize);
}

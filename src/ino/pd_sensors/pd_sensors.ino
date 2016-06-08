/*
 * pd_sensors.ino - John Burnett + Sage Jenson (c) 2016
 * 
 * Based on CNMAT's UDPSendBundle.ino
 */

#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>    
#include <OSCBundle.h>

EthernetUDP Udp;

//the Arduino's IP
IPAddress ip(127, 0, 0, 1);
//destination IP
IPAddress outIp(127, 0, 0, 1);
const unsigned int outPort = 12345;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };


void setup() {
  Ethernet.begin(mac,ip);
  Udp.begin(8888);
}


void loop(){
    OSCBundle bndl;

    bndl.add("/sensors/temperature").add((int32_t)analogRead(0)); // read from analog 0
    bndl.add("/sensores/piezo").add((int32_t)analogRead(1)); // read from analog 1

    Udp.beginPacket(outIp, outPort);
    bndl.send(Udp);
    Udp.endPacket();
    bndl.empty();

    delay(100);
}

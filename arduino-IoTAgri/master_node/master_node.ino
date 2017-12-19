/*
 Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 
 Update 2014 - TMRh20
 */

/**
 * Simplest possible example of using RF24Network,
 *
 * RECEIVER NODE
 * Listens for messages from the transmitter and prints them out.
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>


RF24 radio(9,10);                // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);      // Network uses that radio
const uint16_t this_node = 00;    // Address of our node in Octal format ( 04,031, etc)


struct payload_t {                 // Structure of our payload
  int data;
  int value;
};

void setup(void)
{
  Serial.begin(57600);
  Serial.println("RF24Network/examples/helloworld_rx/");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 88, /*node address*/ this_node);
}

void loop(void){
  
  network.update();                  // Check the network regularly

  if (Serial.available()) {
    sendData(Serial.readString());
  }

  while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader header;        // If so, grab it and print it out
    payload_t payload;
    network.read(header,&payload,sizeof(payload));
    
    Serial.print(payload.data);
    Serial.print("=");
    Serial.print(payload.value);
    Serial.println("");
  }
}

void sendData(String n) {
  int data = n[0]-'0';
  int value = n[1]-'0';
//  Serial.println(n[0]);
//  Serial.println(n[1]);
  payload_t send_payload = { 9, value };
  RF24NetworkHeader send_header(/*to node*/ (uint16_t)data);
  bool ok = network.write(send_header,&send_payload,sizeof(send_payload));
  if (ok)
    Serial.println("999=ok");
  else
    Serial.println("999=failed");
}


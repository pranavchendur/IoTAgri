/*
 Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 
 Update 2014 - TMRh20
 */

/**
 * Simplest possible example of using RF24Network 
 *
 * TRANSMITTER NODE
 * Every 2 seconds, send a payload to the receiver node.
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(9,10);                    // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);          // Network uses that radio

int sm=0;

const uint16_t this_node = 02;        // Address of our node in Octal format
const uint16_t master_node = 00;       // Address of the other node in Octal format

const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already


struct payload_t {                 // Structure of our payload
  int data;
  int value;
};

void setup(void)
{
  Serial.begin(57600);
  Serial.println("RF24Network/examples/helloworld_tx/");

  pinMode(4,OUTPUT);
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 88, /*node address*/ this_node);

}

void loop() {
  
  network.update();                          // Check the network regularly

  while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader recv_header;        // If so, grab it and print it out
    payload_t recv_payload;
    network.read(recv_header,&recv_payload,sizeof(recv_payload));
    
    Serial.print(recv_payload.data);
    Serial.print("=");
    Serial.print(recv_payload.value);
    Serial.println("");

    if (recv_payload.data==9) {
      if (recv_payload.value==1)
        digitalWrite(4,HIGH);
      else if (recv_payload.value==0)
        digitalWrite(4,LOW);
    }
  }

  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - last_sent >= interval  )
  {
    last_sent = now;

    sm = analogRead(A0);
    sm = map(sm,0,1024,100,0);

    Serial.print("Soil Moisture: ");
    Serial.print(sm);
    Serial.println(" units ");

    RF24NetworkHeader header(/*to node*/ master_node);

    Serial.print("Sending...");
    payload_t payload = { 201, sm };
    
    bool ok = network.write(header,&payload,sizeof(payload));
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
  }
}




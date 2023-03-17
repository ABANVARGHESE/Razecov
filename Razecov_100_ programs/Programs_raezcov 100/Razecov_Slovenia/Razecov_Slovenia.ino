// wifi control
#include <WiFi.h>
#include "esp_wifi.h"
const uint16_t port = 4042;
const char * host = "192.168.0.101";
WiFiClient client;

// relay control
const int top_light = 15;
const int bottom_light = 27;
const int sensor1 = 4;

// switch control
const int top_switch = 23;
const int bottom_switch = 22;

// motor control
const int driverPUL = 26;    // PUL- pin
const int driverDIR = 25;    // DIR- pin

// flags and initializers
boolean height_flag = false;
boolean clockwise = false;
boolean anticlockwise = false;
int num = 0;
int seconds;
int runner;
unsigned long trigger;

//// Set your Static IP address
//IPAddress local_IP(192, 168, 0, 80);
//// Set your Gateway IP address
//IPAddress gateway(192, 168, 0, 50);
//
//IPAddress subnet(255, 255, 255, 0);

void setup(){

    Serial.begin(115200);
    
    pinMode(top_light, OUTPUT);
    digitalWrite(top_light, HIGH);
    
    pinMode(bottom_light, OUTPUT);
    digitalWrite(bottom_light, HIGH);
        
    pinMode(driverPUL, OUTPUT);
    digitalWrite(driverPUL,HIGH);
    
    pinMode(driverDIR, OUTPUT);
    digitalWrite(driverDIR,HIGH);
    
    pinMode(sensor1, INPUT_PULLUP);
    pinMode(top_switch, INPUT_PULLUP);
    pinMode(bottom_switch, INPUT_PULLUP);

//    // Configures static IP address
//    if (!WiFi.config(local_IP, gateway, subnet)) {
//     Serial.println("STA Failed to configure");
//    }

    WiFi.begin("TP-Link_Extender", "");

    while ((WiFi.status() != WL_CONNECTED))
      {
        Serial.print(".");
        delay(100);        
      }

    Serial.println();
    Serial.print("IP_Address: ");
    Serial.println(WiFi.localIP());

    Serial.println();
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());

    Serial.println();
    Serial.print("Subnet: ");
    Serial.println(WiFi.subnetMask());

    Serial.println("Succesfully Connected!!!");
    Serial.println(host);

    while(1){
        
      while(!(client.connected())){
       connection();
      }
      while((client.connected())){
       heigth_listener(); 
      }

   }  
    
}

void connection(){
  
  while(1){

    if(client.connect(host, port)){
     return;
    }
        
  }
   
}

void heigth_listener(){
    
  while(1){
    
      while(client.connected() < 1){
       connection();
      }
      
    while(client.available() > 0){
      
     String line = client.readStringUntil('\r');
     Serial.println(line); 
     if(line == "-3"){
      anticlockwise = true;      
     }
     else if(line == "-2"){      
      clockwise = true; 
     }

     else if(line == "-1"){
      clockwise = false;
      anticlockwise = false;
     }
     
     else if(line == "5"){
      int light = digitalRead(bottom_switch);
      if((light == 0)){
       height_flag = true;  
      }
      
      else{
       height_flag = false; 
      }       
      timer_listener();
     }
      
    }
    if(clockwise == true){
      digitalWrite(driverDIR,HIGH);
      delayMicroseconds(2.5);
      digitalWrite(driverPUL,HIGH);
      delayMicroseconds(75);
      digitalWrite(driverPUL,LOW);
      delayMicroseconds(75);    
      int bottom = digitalRead(bottom_switch);

       while((bottom == 0)){
        digitalWrite(driverPUL, LOW);
        client.write("b");
        clockwise = false; 
        break;
       }
        
    }
    else{
     digitalWrite(driverPUL, LOW); 
    }
    if(anticlockwise == true){
     digitalWrite(driverDIR,LOW);
     delayMicroseconds(2.5);
     digitalWrite(driverPUL,HIGH);
     delayMicroseconds(75);
     digitalWrite(driverPUL,LOW);
     delayMicroseconds(75);
     int top = digitalRead(top_switch);      

      while((top == 0)){
       digitalWrite(driverPUL, LOW); 
       client.write("t");
       anticlockwise = false;
       break;        
      }
        
    }
    else{
     digitalWrite(driverPUL, LOW); 
      }

   }  
   
}

void timer_listener(){

  while(1){
  
       while(client.connected() < 1){
        connection();
        num = 0;
        heigth_listener();
       } 
          
       while (client.available() > 0){
        num *= 10;
        num += (client.read() - '0');   
       }
       if((num == 1)||(num == 2)||(num == 3)||(num == 4)||(num == 5)||(num == 6)||(num == 7)||(num == 8)||(num == 9)||(num == 10)||(num == 15)||(num == 20)||(num == 25)||(num == 30)||(num == 35)||(num == 40)||(num == 45)||(num == 50)||(num == 55)||(num == 60)){
        Serial.println(num);
        seconds = (240 * num);
        trigger = 0;
        runner = 0;
        delay_loop(seconds, trigger);
         }
       else if((num == 65)){
        Serial.println(num);
        num = 0;
        heigth_listener();  
        }
    
    }
  
}


void delay_loop(int counts, unsigned long checkers){

 while(1){

    while(client.connected() < 1){
     connection();
     num = 0;
     heigth_listener();
    }
     
    delay(1);        
    if(runner < 30000){
     runner++;
    }
    if(runner > 20000){ 
     time_loop(counts, checkers);
    }
       
  }
   
}


void time_loop(int count, unsigned long checker){
  
 while(1){

  for(int a = count-1; a >= 0; a--){
      
   for(int b = 0; b < 250; b++){

          if(height_flag == false){   
          digitalWrite(top_light, LOW);
          digitalWrite(bottom_light, LOW);
          } 
          else{
          digitalWrite(bottom_light, LOW); 
          }
         while((client.available() > 0)) {
          
          String line = client.readStringUntil('\r');
          
          if(line == "s"){
            
            digitalWrite(top_light, HIGH);
            digitalWrite(bottom_light, HIGH);
            num = 0;
              
             while(1){
    
              int bottom = digitalRead(bottom_switch);
              
              if((bottom == 0)){
                digitalWrite(driverPUL, HIGH);
                client.write("d");
                client.write("b");
                heigth_listener(); 
              } 
               
              digitalWrite(driverDIR,HIGH);
              delayMicroseconds(2.5);
              digitalWrite(driverPUL,HIGH);
              delayMicroseconds(75);
              digitalWrite(driverPUL,LOW);
              delayMicroseconds(75);    
                
            }
            
          }
          if(line == "h"){ 
           digitalWrite(top_light, HIGH);
           digitalWrite(bottom_light, HIGH);
           num = 0;
           int sub = a;
           halt_loop(sub, checker);
          }
               
        }
        
    delay(1);

     if(checker <= 40000){
       checker++;
     }

     if(checker >= 20000){
    
         int val = digitalRead(sensor1);

         if(val  == 0) {

            client.write("2");             
            digitalWrite(top_light, HIGH);
            digitalWrite(bottom_light, HIGH);
            num = 0;       
            timer_listener();
                     
        }
  
      }
    
    }
     
  }
    checker = 0;
    count = 0;
    
    digitalWrite(top_light, HIGH);
    digitalWrite(bottom_light, HIGH);   
    finish_loop();

 }
 
} 

void finish_loop(){

 while(1){

   while(client.connected() < 1){
       connection();
       num = 0;
       heigth_listener();
   }
    
   while((client.available() > 0)) {
    
        String line = client.readStringUntil('\r');
        
       if(line == "y"){
        num = 0; 
        timer_listener();
       }
       if(line == "n"){
        
         num = 0;
          
         while(1){

          int bottom = digitalRead(bottom_switch);
          
          if((bottom == 0)){
            digitalWrite(driverPUL, HIGH); 
            client.write("d");
            client.write("b");
            heigth_listener(); 
          } 
           
          digitalWrite(driverDIR,HIGH);
          delayMicroseconds(2.5);
          digitalWrite(driverPUL,HIGH);
          delayMicroseconds(75);
          digitalWrite(driverPUL,LOW);
          delayMicroseconds(75);      
            
         }
          
       }  
  
     }

   }
    
 }

void halt_loop(int loop_count, unsigned long loop_checker){

 while(1){

  while(client.connected() < 1){
     digitalWrite(top_light, HIGH);
     digitalWrite(bottom_light, HIGH); 
     connection();
     num = 0;
     heigth_listener();
  }
  
  while(client.available() > 0) {

     String line = client.readStringUntil('\r');
   if(line == "p"){
       loop_checker = 0; 
       time_loop(loop_count, loop_checker);
   }
   if(line == "s"){
       digitalWrite(top_light, HIGH);
       digitalWrite(bottom_light, HIGH);
       num = 0;
              
             while(1){
    
              int bottom = digitalRead(bottom_switch);
              
              if((bottom == 0)){
                digitalWrite(driverPUL, HIGH);
                client.write("d");
                client.write("b"); 
                heigth_listener(); 
              } 
               
              digitalWrite(driverDIR,HIGH);
              delayMicroseconds(2.5);
              digitalWrite(driverPUL,HIGH);
              delayMicroseconds(75);
              digitalWrite(driverPUL,LOW);
              delayMicroseconds(75);      
                
            }
             
         }     
     
      }
    
   }
  
}

void loop(){

}

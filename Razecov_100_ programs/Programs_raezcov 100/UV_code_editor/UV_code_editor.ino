// wifi control
#include <WiFi.h>
#include "esp_wifi.h"
const uint16_t port = 4042;
//const char * host;
//const char* ssid     = "RazeCov_Semi 0";
//const char* password = "razecov0";
WiFiClient client;
String incomingByte;
//IPAddress ip;

// relay control
const int top_light = 15;
const int bottom_light = 27;
const int motor_pwr = 5;
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

// timers
unsigned long counter1 = 0;

int conn = 0;

void setup(){

    Serial.begin(115200);
    
    pinMode(top_light, OUTPUT);
    digitalWrite(top_light, HIGH);
    
    pinMode(bottom_light, OUTPUT);
    digitalWrite(bottom_light, HIGH);
    
    pinMode(motor_pwr, OUTPUT);
    digitalWrite(motor_pwr, HIGH);
    
    pinMode(driverPUL, OUTPUT);
    digitalWrite(driverPUL,HIGH);
    
    pinMode(driverDIR, OUTPUT);
    digitalWrite(driverDIR,HIGH);
    
    pinMode(sensor1, INPUT_PULLUP);
    pinMode(top_switch, INPUT_PULLUP);
    pinMode(bottom_switch, INPUT_PULLUP);

//    WiFi.softAP(ssid, password);
    WiFi.begin("razecov_Slv", "razecov123");

    while ((WiFi.status() != WL_CONNECTED))
      {
        Serial.print(".");
        delay(100);        
      }

    Serial.println();
    Serial.print("IP_Address: ");
    Serial.println(WiFi.localIP());  

    Serial.println("Succesfully Connected!!!");   

    while(1){
        
      while(!(client.connected())){
       connection();
      }
      while((client.connected())){
//         Serial.println("TCP Connected!!!");
       heigth_listener(); 
      }

   }  
    
}

void connection() {
  
  while(1){

//      wifi_sta_list_t wifi_sta_list;
//      tcpip_adapter_sta_list_t adapter_sta_list;
//     
//      memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
//      memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
//     
//      esp_wifi_ap_get_sta_list(&wifi_sta_list);
//      tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);
//     
//      for (int i = 0; i < adapter_sta_list.num; i++) {
//     
//        tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
//        host = ip4addr_ntoa(&(station.ip));
//        conn = 1;
//        if (client.connect(host, port)) {
//         return;
//        }
//            
//      }
//
//   delay(500);

 while(client.connected() < 1){

  incomingByte = Serial.readStringUntil('\r');
  Serial.println(incomingByte);
  if(client.connect(incomingByte.c_str(), port)){
   return;  
  }
  
 }
      
   }
}

void heigth_listener(){

  byte tap1 = 0;
  byte tap2 = 0;
//  unsigned long beep1 = 0;
//  unsigned long beep2 = 0;
    
  while(1){
    
      while(client.connected() < 1){
       connection();
      }
    
    while(client.available() > 0){
      String line = client.readStringUntil('\r');    
      Serial.println(line);
      
     if(line == "-3"){
         anticlockwise = true;
         digitalWrite(motor_pwr, LOW);
         tap1++;       
     }
     else if(line == "-2"){      
         clockwise = true;
         digitalWrite(motor_pwr, LOW);
         tap2++; 
     }

     else if(line == "-1"){
      clockwise = false;
      anticlockwise = false;
      tap1++;
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

//    if(tap1 == 2){
//      beep1++;
//      if(beep1>30000){
//       tap1 = 0;
//       beep1 = 0; 
//      }
//    }
//
//    if(tap1 == 4){
//      tap1 = 0;
//      Serial.println("pulsing up....");
//      while(1){
//  
//        int top = digitalRead(top_switch);
//        
//        if((top == 0)){
//          digitalWrite(motor_pwr, HIGH);
//          digitalWrite(driverPUL, HIGH);
//          client.write("d");
//          client.write("t"); 
//          heigth_listener(); 
//        } 
//         
//        digitalWrite(driverDIR,LOW);
//        delayMicroseconds(2.5);
//        digitalWrite(driverPUL,HIGH);
//        delayMicroseconds(300);
//        digitalWrite(driverPUL,LOW);
//        delayMicroseconds(300);
//
//        while(client.available() > 0){
//          String line = client.readStringUntil('\r');    
//          Serial.println(line);
//          if(line == "-3"){      
//           digitalWrite(driverPUL, HIGH);
//           num = 0;
//           heigth_listener(); 
//          }
//                
//        }
//          
//      }
//      
//    }
//
//    if(tap2 == 2){
//      beep2++;
//      if(beep2>30000){
//       tap2 = 0;
//       beep2 = 0; 
//      }
//    }
//
//    if(tap2 == 4){
//      tap2 = 0;
//      Serial.println("pulsing down....");
//      while(1){
//  
//        int bottom = digitalRead(bottom_switch);
//        
//        if((bottom == 0)){
//          digitalWrite(motor_pwr, HIGH);
//          digitalWrite(driverPUL, HIGH);
//          client.write("d");
//          client.write("b"); 
//          heigth_listener(); 
//        } 
//         
//        digitalWrite(driverDIR,HIGH);
//        delayMicroseconds(2.5);
//        digitalWrite(driverPUL,HIGH);
//        delayMicroseconds(300);
//        digitalWrite(driverPUL,LOW);
//        delayMicroseconds(300);
//
//        while(client.available() > 0){
//          String line = client.readStringUntil('\r');    
//          Serial.println(line);
//          if(line == "-2"){      
//           digitalWrite(driverPUL, HIGH);
//           num = 0;
//           heigth_listener(); 
//          }
//                
//        }
//          
//      }
//      
//    }
    
    if(clockwise == true){
      digitalWrite(driverDIR,HIGH);
      delayMicroseconds(2.5);
      digitalWrite(driverPUL,HIGH);
      delayMicroseconds(75);
      digitalWrite(driverPUL,LOW);
      delayMicroseconds(75);     
      int bottom = digitalRead(bottom_switch);

       if((bottom == 0)){
        digitalWrite(motor_pwr, HIGH);
        client.write("b"); 
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

      if((top == 0)){
       digitalWrite(motor_pwr, HIGH);
       client.write("t"); 
      }  
    }
    else{
     digitalWrite(driverPUL, LOW); 
      }

   }  
   
}

void timer_listener(){

  digitalWrite(motor_pwr, HIGH);

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
    if(runner > 1){ 
     time_loop(counts, checkers);
    }
       
  }
   
}

void time_loop(int count, unsigned long checker){
  
 while(1){

  for(int a = count-1; a >= 0; a--){

//      wifi_sta_list_t wifi_sta_list;
//      tcpip_adapter_sta_list_t adapter_sta_list;
//     
//      memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
//      memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
//     
//      esp_wifi_ap_get_sta_list(&wifi_sta_list);
//      tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);
//     
//      for (int i = 0; i < adapter_sta_list.num; i++) {
//     
//        tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
//        host = ip4addr_ntoa(&(station.ip));
//        conn = 1;
//
//      }
//
//      if(counter1 <= 10){
//       counter1++; 
//      }
//    
//      if(counter1 == 10){
//       conn = 0;
//       counter1++; 
//      }
//
//      if(counter1 > 10){
//       counter1++; 
//      }
//
//      if(counter1 == 25){
//       counter1 = 0;
//       if(conn == 0){
//        Serial.println("connection lost");
//        ESP.restart();
//       }
//      }
//
//    Serial.println(conn);

   
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
          Serial.println(line);
          
          if(line == "s"){
            
            digitalWrite(top_light, HIGH);
            digitalWrite(bottom_light, HIGH);
            num = 0;
//            heigth_listener(); 
            digitalWrite(motor_pwr, LOW);
            Serial.println("resetting height.....");
              
             while(1){
    
              int bottom = digitalRead(bottom_switch);
              
              if((bottom == 0)){
                digitalWrite(motor_pwr, HIGH);
                digitalWrite(driverPUL, HIGH);
                client.write("d");
                client.write("b"); 
                heigth_listener(); 
              } 
               
              digitalWrite(driverDIR,HIGH);
              delayMicroseconds(2.5);
              digitalWrite(driverPUL,HIGH);
              delayMicroseconds(300);
              digitalWrite(driverPUL,LOW);
              delayMicroseconds(300);     
                
            }
            
          }
          if(line == "h"){ 
           digitalWrite(top_light, HIGH);
           digitalWrite(bottom_light, HIGH);
           num = 0;
           int sub = a;
           Serial.println(checker);
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
            Serial.println("motion...!");                   
            client.write("2"); 
            digitalWrite(top_light, HIGH);
            digitalWrite(bottom_light, HIGH);
            num = 0;       
            timer_listener();
                     
        }
  
      }
    
    }
     
  }
    counter1 = 0;
    checker = 0;
    count = 0;
    
    digitalWrite(top_light, HIGH);
    digitalWrite(bottom_light, HIGH);   
    finish_loop();

 }
 
} 

void finish_loop(){

 digitalWrite(motor_pwr, HIGH);

 while(1){

   while(client.connected() < 1){
       connection();
       num = 0;
       heigth_listener();
      }
   while((client.available() > 0)) {
        String line = client.readStringUntil('\r');
        Serial.println(line);
        if(line == "y"){
         num = 0; 
         timer_listener();
          }
       if(line == "n"){
        
         num = 0;
//         heigth_listener(); 
         digitalWrite(motor_pwr, LOW);
         Serial.println("resetting height.....");
          
         while(1){

          int bottom = digitalRead(bottom_switch);
          
          if((bottom == 0)){
            digitalWrite(motor_pwr, HIGH);
            digitalWrite(driverPUL, HIGH); 
            client.write("d");
            client.write("b");
            heigth_listener(); 
          } 
           
          digitalWrite(driverDIR,HIGH);
          delayMicroseconds(2.5);
          digitalWrite(driverPUL,HIGH);
          delayMicroseconds(300);
          digitalWrite(driverPUL,LOW);
          delayMicroseconds(300);     
            
         }
          
       }  
  
     }

   }
    
 }

void halt_loop(int loop_count, unsigned long loop_checker){

 digitalWrite(motor_pwr, HIGH);
 
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
     Serial.println(line);
   if(line == "p"){
       loop_checker = 0; 
       time_loop(loop_count, loop_checker);
   }
   if(line == "s"){
       digitalWrite(top_light, HIGH);
       digitalWrite(bottom_light, HIGH);
       num = 0;
//       heigth_listener(); 
       digitalWrite(motor_pwr, LOW);
       Serial.println("resetting height.....");
              
             while(1){
    
              int bottom = digitalRead(bottom_switch);
              
              if((bottom == 0)){
                digitalWrite(motor_pwr, HIGH);
                digitalWrite(driverPUL, HIGH);
                client.write("d");
                client.write("b"); 
                heigth_listener(); 
              } 
               
              digitalWrite(driverDIR,HIGH);
              delayMicroseconds(2.5);
              digitalWrite(driverPUL,HIGH);
              delayMicroseconds(300);
              digitalWrite(driverPUL,LOW);
              delayMicroseconds(300);     
                
            }
             
         }     
     
      }
    
   }
  
}

void loop(){

}

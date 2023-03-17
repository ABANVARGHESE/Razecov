// wifi control
#include <WiFi.h>
#include "esp_wifi.h"
const uint16_t port = 4042;
const char * host;
const char* ssid     = "RazeCov_Semi 94";
const char* password = "razecov94";
WiFiClient client;

// relay control
const int top_light = 15;
const int bottom_light = 27;
const int sensor1 = 4;

// flags and initializers
int num = 0;
int seconds;
unsigned long trigger;

// timers
unsigned long timer1;

void setup(){
    
    pinMode(top_light, OUTPUT);
    digitalWrite(top_light, HIGH);
    
    pinMode(bottom_light, OUTPUT);
    digitalWrite(bottom_light, HIGH);
    
    pinMode(sensor1, INPUT_PULLUP);

    WiFi.softAP(ssid, password);

    while(1){
        
      while(!(client.connected())){
       connection();
      }
      while((client.connected())){
       heigth_listener(); 
      }

   }  
    
}

void connection() {

 timer1 = millis();
  
  while(1){
    
    if (millis() - timer1 >= 500UL){
      wifi_sta_list_t wifi_sta_list;
      tcpip_adapter_sta_list_t adapter_sta_list;
     
      memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
      memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
     
      esp_wifi_ap_get_sta_list(&wifi_sta_list);
      tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);
     
      for (int i = 0; i < adapter_sta_list.num; i++) {
     
        tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
        host = ip4addr_ntoa(&(station.ip));
        if (client.connect(host, port)) {
         return;
        }    
      }
     
      timer1 = millis(); 
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
      
      if(line == "5"){      
       timer_listener();
      }
      
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
        seconds = (240 * num);
        trigger = 0;
        time_loop(seconds, trigger);
       }
       else if((num == 65)){
        num = 0;
        heigth_listener();  
       }
    
    }
  
}


void time_loop(int count, unsigned long checker){
  
 while(1){

  for(int a = count-1; a >= 0; a--){
    
   for(int b = 0; b < 250; b++){
  
         digitalWrite(top_light, LOW);
         digitalWrite(bottom_light, LOW);
  
         while((client.available() > 0)) {
          
          String line = client.readStringUntil('\r');
          
          if(line == "s"){            
           digitalWrite(top_light, HIGH);
           digitalWrite(bottom_light, HIGH);
           num = 0;
           heigth_listener();            
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
        heigth_listener();          
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
    heigth_listener();
             
   }     
     
  }
    
 }
  
}

void loop(){

}

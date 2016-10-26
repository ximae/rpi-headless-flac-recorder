#include <wiringPi.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>



PI_THREAD (record) {

   time_t rawtime;
   struct tm *info;

   char filename[80];
   char command[100];

   time( &rawtime );

   info = localtime( &rawtime );
   strftime(filename, 80, "%d_%m_%Y__%H_%M_%S", info);

  strcpy( command, "arecord -D hw:1,0 -f cd " );
  
  strcat(command , filename);
  
  strcat(command , ".wav");

  system(command);

 } 

  

int main (void) {

  wiringPiSetup () ;
  pinMode (0, OUTPUT) ;
  pinMode (1, OUTPUT) ;
  pinMode (2, OUTPUT) ;
  pinMode (3, OUTPUT) ;
  pinMode (4, INPUT) ;

  digitalWrite (0, HIGH) ;
  
  int state = 0 ;
  int press = 0;
  int prev = 0;

for (;;)  {

 if (digitalRead(4) == HIGH) {
    press = 1; // pressed
    delay(200);
    }
 else
  press = 0; // not pressed
 
 // at every button press, toggle the state
 if ((press == 1) && (press != prev))
  state = !state;
 
 // store prev value
 prev = press;

 if (state == 1) {
   
       piThreadCreate (record) ;

       digitalWrite (0,  LOW) ;    
       while (state == 1) {
          digitalWrite (1, HIGH) ; delay (300) ;
          digitalWrite (1,  LOW) ; 
          digitalWrite (2, HIGH) ; delay (300) ;
          digitalWrite (2,  LOW) ; 
          digitalWrite (3, HIGH) ; delay (300) ;
          digitalWrite (3,  LOW) ; 
          if (digitalRead(4) == HIGH)
             state = 0;
       
          }
      }

 else {
        digitalWrite (0, HIGH) ;
        digitalWrite (1,  LOW) ;
        digitalWrite (2,  LOW) ;
        digitalWrite (3,  LOW) ;
        system ("pkill arecord");
       }

  }
  return 0 ;
}





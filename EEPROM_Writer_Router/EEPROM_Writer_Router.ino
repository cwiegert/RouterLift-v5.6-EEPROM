/**************************************************************************************************
 * Test program to figure out if we can use EEPROM to store variables for setup and managmement
 * Would like to have a tool that allows us to set EEPROM for each stepper motor, so we don't have to 
 * use the SD cared to store config settings.   EEPROM, once written, does not reset until it is over written or erased by 
 * writing to EEPROM again.   It may be a good way to set configuration on automation tools, and would use this program to set them up
 * 
 * once they are set up, EEPROM.get and EEPROM.read can be used to set the config necessary during run time, or doing lookups
 * 
 * mega 2560 has 4096 bytes
 * uno  has 
 * nano has 1024 bytes
 * 
 * set the boardMemory variable to the appropriate size , and the memory will be cleared then written 
 * May need to modify code for the right program and initializing - but the concpet is here, 
 * 
 * reading memory is based on sizeof(type)... that will advance the reading point
 * can write to various points of EEPROM by using put, instead of write.   Can create structures and write entire structure
 *    instead of writing variable by variable
 *    use last read address += sizeof(variable type) to advance to te next read
 *    
 *    thinking of using this for initializing a bunch of setup on a nano, and not having to use the SD card for memory management
 **********************************************************************************/


#include <EEPROM.h>
#include "SdFat.h"
#include "RouterEEPROMGlobals.h"
  
   struct inchCacl {
          int    index;
          char   inches[6];
          float  decimal;
          float  steps;
          char   label[30];
        }  preSetLookup; 
  


 int  boardMemory = 4096;     // This is for a MEGA
 //int boardMemory = 1024;

//Serial.begin (115200);
//delay (1000);
//EEPROM.get (0, directionPin);
//Serial.print (" outside setup ==> ");
//Serial.println (directionPin);

 void clearEEProm ( int boardSize)
  {
    int lastx = 0;
   Serial.println("clearing the EEPROM");
    for (int x = 0; x< boardSize; x++)
      {
        if (x == lastx + 80)
          {
            Serial.println(".");
   //         Serial.println("Clearing EEPROM.");
            lastx = x;
          }
        else
          Serial.print(".");
        EEPROM.write(x, 255);
      }
     Serial.println("");
     Serial.println("EEPROM Cleared");
  }

        
int loadHeightsFromFile(int move_go) {

        
          int     i = 0;
          SdFile  fSeek;
          char    fileLine[60] = {'\0'};
          char    divider[2] = {',', '\0'};
          char    *token;
          char    checker;
          int     limit;
          int     bStop=1;

 
          if (!fSeek.open("heights.txt"))
            {
              //ErrorDialog( 108 );   
              return 0;
            }
          fSeek.rewind(); 
          checker = fSeek.read();
          if (move_go == 1)
            while (checker != '^')            // find the end of the header section, start populating the structure with preset configs
              checker = fSeek.read();
          else if (move_go == 0)
            while (checker != char(160) )  // find the break in heights, then start reading special bit definitions 
                checker = fSeek.read(); 
          fSeek.read();
          if (move_go == 1)
            limit = 4;
          else if ( move_go == LOW)
            limit = 5;
          Serial.print("Writing heights to the EEPROM.");
          while (fSeek.available())
            {
              fSeek.fgets(fileLine, sizeof(fileLine));
              if (fileLine[0] == '*')
                {
                  fSeek.close();
                  Serial.println("");
                  Serial.println("Finished writing Heights to EEPROM");
                  return;
                }
              token = strtok(fileLine, divider);   
              for (i=1; i<= limit; i++)
                {
                  token = strtok(NULL, divider);
                  switch (i)
                    {
                      case 1:
                        preSetLookup.index = atoi(token);
                        break;
                      case 2:
                        strcpy(preSetLookup.inches, token);  
                        break;
                      case 3:
                        preSetLookup.decimal = atof(token);
                        break;
                      case 4:
                        preSetLookup.steps = atof(token);
                        break;
                      case 5:
                        strcpy (preSetLookup.label, token);
                        break;
                    }
                  }
                EEPROM.put(eeAddress, preSetLookup);
                Serial.print(".");
                eeAddress += sizeof(preSetLookup);
 
   /*           if (move_go == HIGH)
                { 
                  if (preSetLookup.index == preSetNum)      // found the right line from the file, move to end of file
                    fSeek.seekEnd();
                }
              else if (move_go == LOW)
                {
                  if ( preSetLookup.index == index)         // found the right custom bit in the heights file, move to the end of the file
                  fSeek.seekEnd();
                }*/
              
                  
            }
          fSeek.close();
          Serial.println("");
          Serial.println("Finished writing Heights to EEPROM");
          return eeAddress;
        }

void setup() {
 Serial.begin(250000);
  delay (1000);

   pinMode(SD_WRITE, OUTPUT);       // define the SD card writing pin
        
   if (!sdCard.begin(SD_WRITE))
      Serial.println("Could not open the SD card");
  

 
/*   add section here to take input from the Serial interface and dedide if the memory should be cleared*/

  
 /*if (EEPROM[boardMemory - 1] == 255)
    {
      Serial.println("going to write a bunch of data to the EEPROM, the filled byte indicates it hasn't been written");
      EEPROM.put (eeAddress, directionPin);
      eeAddress += sizeof(byte);
      EEPROM.put(eeAddress, fenceDirPin);
      eeAddress += sizeof(byte);
      EEPROM.put(eeAddress, stepPin);
      eeAddress += sizeof(byte);
      EEPROM.put(eeAddress, fenceStepPin);
      eeAddress += sizeof(byte);
      EEPROM.put(eeAddress, enablePin);
      eeAddress += sizeof(byte);
      EEPROM.put(eeAddress, fenceEnablePin);
      eeAddress += sizeof(byte);
      EEPROM.put(eeAddress, initSpeed);
      eeAddress += sizeof(float);
      EEPROM.put(eeAddress, maxMotorSpeed);
      eeAddress += sizeof(long);
      EEPROM.put(eeAddress, workingMotorSpeed);
      eeAddress += sizeof(float);
      EEPROM.put(eeAddress, stepsPerRevolution);
      eeAddress += sizeof(int);
      EEPROM.put(eeAddress, microPerStep);
      eeAddress += sizeof(int); 
      EEPROM.put (eeAddress, distPerStep);
      eeAddress += sizeof(float);     
      EEPROM.put(eeAddress, pulseWidthMicros);
      eeAddress += sizeof(float);
      EEPROM.put(eeAddress, millisBetweenSteps);
      EEPROM.put (4095, 1);
    }
      initSpeed = 0;
      EEPROM.get(6, initSpeed);
      Serial.print("read initSpeed from EEPROM ==> ");
      Serial.println(initSpeed);
      eeAddress = 6 + sizeof(initSpeed);
      maxMotorSpeed = 0;
      EEPROM.get(eeAddress, maxMotorSpeed);
      
      Serial.print ("read maxMotorSpeed from EEPROM ==> ");
      Serial.println (maxMotorSpeed);
      eeAddress += sizeof(maxMotorSpeed);
      workingMotorSpeed = 0;
      EEPROM.get(eeAddress, workingMotorSpeed);
      Serial.print ("read workingMotorSpeed from EEPROM ==> ");
      Serial.println(workingMotorSpeed);
      Serial.print ("value of end state of EEPROM ==> ");
      Serial.println(EEPROM.read(4095));

   */

}

void loop() {
  // put your main code here, to run repeatedly:
  char   serGet;
  int     nTemp = 0;
  Serial.println("");
  Serial.println ("enter an <X> if you want to clear the EEPROM");
  Serial.println("enter an <S> if you want to load the settings to EEPROM");
  Serial.println("enter an <I> if you want to get the settings from EEPROM");
  Serial.println("enter an <H> if you what the Heights file loaded");
  Serial.println("enter an <R> if you want to read the first heights values from EEPROM");
  Serial.println("    enter an <N> if you want to read the next Heights values");
  Serial.println("enter <B> if you want to load special bits from the SD");
  Serial.println ("enter <C> if you want size of structure");
  Serial.println("enter <L> if you want to see what's in EEPROM");

  while (Serial.available()==0);
  serGet = Serial.read();
  
 switch (serGet)
   {
      case 'X':
        {
          clearEEProm(boardMemory);
          break;
        }
      case 'S':
        {
            eeAddress = 0;
            Serial.println("");
            Serial.println("going to write a bunch of data to the EEPROM, the filled byte indicates it hasn't been written");
            EEPROM.put (eeAddress, directionPin);
            eeAddress += sizeof(directionPin);
            EEPROM.put(eeAddress, fenceDirPin);
            eeAddress += sizeof(fenceDirPin);
            EEPROM.put(eeAddress, stepPin);
            eeAddress += sizeof(stepPin);
            EEPROM.put(eeAddress, fenceStepPin);
            eeAddress += sizeof(fenceStepPin);
            EEPROM.put(eeAddress, enablePin);
            eeAddress += sizeof(enablePin);
            EEPROM.put(eeAddress, fenceEnablePin);
            eeAddress += sizeof(fenceEnablePin);
            EEPROM.put(eeAddress, initSpeed);
            eeAddress += sizeof(initSpeed);
            EEPROM.put(eeAddress, stepsPerRevolution);
            eeAddress += sizeof(stepsPerRevolution);
            EEPROM.put(eeAddress, microPerStep);
            eeAddress += sizeof(microPerStep);  
            EEPROM.put(eeAddress, pulseWidthMicros);
            eeAddress += sizeof(pulseWidthMicros);
            EEPROM.put(eeAddress, millisBetweenSteps);
            eeAddress += sizeof (millisBetweenSteps);
            EEPROM.put(eeAddress, TOP_SWITCH);
            eeAddress += sizeof (TOP_SWITCH);
            EEPROM.put(eeAddress, BOTTOM_SWITCH);
            eeAddress += sizeof (BOTTOM_SWITCH);
            EEPROM.put(eeAddress, FRONT_SWITCH);
            eeAddress += sizeof (FRONT_SWITCH);
            EEPROM.put(eeAddress, BACK_SWITCH);
            eeAddress += sizeof (BACK_SWITCH);
            EEPROM.put(eeAddress,REZERO);
            eeAddress += sizeof(REZERO);
            EEPROM.put(eeAddress, maxAcceleration);
            eeAddress += sizeof(maxAcceleration);
            EEPROM.put (eeAddress, distPerStep);
            eeAddress += sizeof(distPerStep);   
            EEPROM.put(eeAddress, maxMotorSpeed);
            eeAddress += sizeof(maxMotorSpeed);
            EEPROM.put(eeAddress, workingMotorSpeed);
            eeAddress += sizeof(workingMotorSpeed);
            EEPROM.put (eeAddress, ver);
            eeAddress += sizeof(ver);
            nTemp = EEPROM[boardMemory - 1];
            if ((nTemp < eeAddress) || nTemp == 255)
              {
                EEPROM[boardMemory - 1] = eeAddress;
                
              } 
            EEPROM[boardMemory -3] = 100;
            break;
        }
      case 'I':
        {
          eeAddress = 0;
          EEPROM.get (eeAddress, directionPin);
          eeAddress += sizeof(directionPin);
          EEPROM.get(eeAddress, fenceDirPin);
          eeAddress += sizeof(fenceDirPin);
          EEPROM.get(eeAddress, stepPin);
          eeAddress += sizeof(stepPin);
          EEPROM.get(eeAddress, fenceStepPin);
          eeAddress += sizeof(fenceStepPin);
          EEPROM.get(eeAddress, enablePin);
          eeAddress += sizeof(enablePin);
          EEPROM.get(eeAddress, fenceEnablePin);
          eeAddress += sizeof(fenceEnablePin);
          EEPROM.get(eeAddress, initSpeed);
          eeAddress += sizeof(initSpeed);
          EEPROM.get(eeAddress, stepsPerRevolution);
          eeAddress += sizeof(stepsPerRevolution);
          EEPROM.get(eeAddress, microPerStep);
          eeAddress += sizeof(microPerStep);  
          EEPROM.get(eeAddress, pulseWidthMicros);
          eeAddress += sizeof(pulseWidthMicros);
          EEPROM.get(eeAddress, millisBetweenSteps);
          eeAddress += sizeof (millisBetweenSteps);
          EEPROM.get(eeAddress, TOP_SWITCH);
          eeAddress += sizeof (TOP_SWITCH);
          EEPROM.get(eeAddress, BOTTOM_SWITCH);
          eeAddress += sizeof (BOTTOM_SWITCH);
          EEPROM.get(eeAddress, FRONT_SWITCH);
          eeAddress += sizeof (FRONT_SWITCH);
          EEPROM.get(eeAddress, BACK_SWITCH);
          eeAddress += sizeof (BACK_SWITCH);
          REZERO = EEPROM[eeAddress];
          eeAddress += sizeof(REZERO);
          EEPROM.get(eeAddress, maxAcceleration);
          eeAddress += sizeof(maxAcceleration);
          EEPROM.get (eeAddress, distPerStep);
          eeAddress += sizeof(distPerStep);   
          EEPROM.get(eeAddress, maxMotorSpeed);
          eeAddress += sizeof(maxMotorSpeed);
          EEPROM.get(eeAddress, workingMotorSpeed);
          eeAddress += sizeof(workingMotorSpeed);
          EEPROM.get(eeAddress, ver);
          eeAddress+= sizeof(ver);
  
          Serial.println("_________________________________________________________________________________________________________________________");
          Serial.print ("directionPin ==> ");
          Serial.println(byte(directionPin));
          Serial.print ("fenceDirPin ==> ");
          Serial.println(fenceDirPin);
          Serial.print("stepPin ==> ");
          Serial.println(stepPin);
          Serial.print ("fenceStepPin ==> ");
          Serial.println(fenceStepPin);
          Serial.print ("enablePin ==> ");
          Serial.println(enablePin);
          Serial.print("fenceEnablePin ==> ");
          Serial.println(fenceEnablePin);
          Serial.print ("initSpeed ==> ");
          Serial.println (initSpeed);       
          Serial.print("stepsPerRevolution ==> ");
          Serial.println(stepsPerRevolution);
          Serial.print("microPerStep ==> ");
          Serial.println(microPerStep);       
          Serial.print("pulseWidthMicros ==> ");
          Serial.println(pulseWidthMicros);
          Serial.print ("millisBetweenSteps ==> ");
          Serial.println(millisBetweenSteps);
          Serial.print ("TOP_SWITCH ==> ");
          Serial.println(TOP_SWITCH);
          Serial.print("BOTTOM_SWITCH ==> ");
          Serial.println(BOTTOM_SWITCH);
          Serial.print("FRONT_SWITCH ==>");
          Serial.println (FRONT_SWITCH);
          Serial.print ("BACK_SWITCH ==> ");
          Serial.println(BACK_SWITCH);
          Serial.print("REZERO ==> ");
          Serial.println(REZERO);
          Serial.print ("maxAcceleartion ==> ");
          Serial.println(maxAcceleration);
          Serial.print("distPerStep ==> ");
          Serial.println(distPerStep,8);
          Serial.print("maxMotorSpeed == > ");
          Serial.println(maxMotorSpeed);
          Serial.print("workingMotorSpeed ==> ");
          Serial.println(workingMotorSpeed);
          Serial.print("version ==> ");
          Serial.println(ver);
          break;
        }
      
      case 'H':
        {
          eeAddress = EEPROM[boardMemory - 3];
          loadHeightsFromFile(1);
          EEPROM[4095] = eeAddress+16;
          Serial.print (" New start address of the EEPROM ==> ");
          Serial.println(eeAddress);
          break;
        }  
      case 'B' :
        {

          break;
        }
      case 'C':
        {
          Serial.print ("Size of the preset lookup structure ==> ");
          Serial.println(sizeof(preSetLookup));
          Serial.println("");
          break;
        }
      case 'L':
        for (int x = 0; x< boardMemory; x++)
         {
           Serial.print (x);
           Serial.print (" ==> ");
           Serial.println (EEPROM.read(x));
         }
       break;
      case 'R':
        {
         // eeAddress = sizeof(preSetLookup);
          eeAddress = 100;
          EEPROM.get(eeAddress,preSetLookup);
          Serial.println(preSetLookup.index);
          Serial.println(preSetLookup.inches);
          Serial.println(preSetLookup.decimal, 6);
          Serial.println(preSetLookup.steps, 6);
          Serial.println("");
    //      eeAddress += sizeof(preSetLookup);
          break;
        }
      case 'N':
        {
          eeAddress += sizeof(preSetLookup);
          EEPROM.get(eeAddress,preSetLookup);
          Serial.println(preSetLookup.index);
          Serial.println(preSetLookup.inches);
          Serial.println(preSetLookup.decimal, 6);
          Serial.println(preSetLookup.steps, 6);
          Serial.println("");
          break;
        }
  
    }
  
 
  

 
}

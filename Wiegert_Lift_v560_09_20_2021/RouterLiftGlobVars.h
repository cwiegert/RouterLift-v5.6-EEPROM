#define DEBUG 0
#define UP 0
#define DOWN 1
#define BACK  2             // for the fence
#define FORWARD 3          // for the fence
#define SD_WRITE 53
//#define DEBUG 1
        
#if DEBUG == 1
#define debug(x) Serial.print (x)
#define debugLn(x) Serial.println(x)
#else
#define debug(x)
#define debugLn(x)
#endif
#define FILE_WRITE (O_WRITE | O_READ | O_CREAT)   // redefine the file write function based on this thread   https://forum.arduino.cc/index.php?topic=616594.0


 
      
        /****************************************************************************
                 HIGH -- arduino constant for 1.   Used for turning the digital pins on and off.  Motor state, HIGH = ON
                 LOW  -- arduino constant for 0    Used for turning the digital pins on and off   Motor state, LOW = OFF
                          no need to define them here, they are already defined in libraries
        *********************************************************************************/
              
        int         TOP_SWITCH;
        int         BOTTOM_SWITCH;
        int         FRONT_SWITCH;
        int         BACK_SWITCH;
        int         REZERO;
        int         FENCE_ZERO;
                
        /*
                   Code for the stepper motor controller.   Need to set the appropriate pins for the digital write and signal
                    These wires go to the stepper controller, not the stepper directely 
                    directionPin and stepPin must be set prior to the AccelStepper object create.   Can not set them through a config file
        */
        
        const  PROGMEM byte   directionPin = 11;             // pin that will go to the D+ on the stepper controller  have to declare here, as the stepper is a global class
        const  PROGMEM byte   fenceDirPin = 28;              // pin that will go to the D+ on the fence stepper controller.   Have to declare here as the stepper is a global class
        const  PROGMEM byte   stepPin = 13;                  // pin that will wire to the P+ (pulse) on the stepper controlloer   have to declare here as the stepper is a global class
        const  PROGMEM byte   fenceStepPin = 30;             // pin that will wire to the P+ (pulse) on the Fence stepper controlloer   have to declare here as the stepper is a global class
        byte   enablePin;                                    //  pin that will wire to the E+ on the stepper controller.   Turns the controller on and off  LOW turns motor on, HIGH turns motor off
        byte   fenceEnablePin;                               //  pin that will wire to the E+ on the fence stepper controller.   Turns the controller on and off  LOW turns motor on, HIGH turns motor off
        int    initSpeed = 2000;                            //  sets the initial speed of the motor.   Don't think we need acceleration in the router, but will find out
        long   maxMotorSpeed;                               //  as defined - maximum speed motor will run.   Sets the 100% on the speed slider
        int    maxAcceleration = 6000;                      //  maximum number of steps for acceleration
        long   workingMotorSpeed;                           //  active working speed, set by the slider, and will be somewhere between 0 and 100%
        const  PROGMEM int    stepsPerRevolution = 1600;     //  number of steps required for 1 revolution of leadscrew
        const  PROGMEM int    microPerStep =   8;            //  number of microSteps.   Set on the TB600 controller by dip switch   Off | On | Off
        const  PROGMEM float  distPerStep = 0.00024606;      //  inches per step with calculated off <microPerStep> = 8
        const  PROGMEM int    pulseWidthMicros = 20;         //  miliseconds  -- delay for the steps.   the higher the number the slower the lead screw will turn
        const  PROGMEM int    millisBetweenSteps = 20;       //  milliseconds - or try 1000 for slower steps   delay between pulses sent to the controller
        long   curPos;                        //  return value from CurrentPosition() call on the stepper
        byte   DIRECTION;                     //  tests the direction the motor should be turning
        byte   HOME_MOTOR = 1;                //  Used with the swFence switch to tell which motor to use when zeroing and resetting
        byte   SET_MOTOR = 1;                 //  Used on the Settings Screen to tell which motor to use when resetting calibrations
        int    stepSize = 3;                 //  used in the moving of the router to set precision on how large the steps should be for the runToPosition() function
        const  PROGMEM char   CR = '\n';                     //  Carraige return constant, used for the writeDebug function to add a new line at the end of the function
        char   preSetTxt[8];                 //  text set in the pre-set combo box.   
        int    preSetNum;                    //  index of the pre-set values.   used when reading the Nextion variable which gets set when a value is selected
        long   lowLimit;                      //  step value when the lower limit switch is hit
        long   highLimit;                     //  step value when the upper limit switch is hit
        double  curPosInch;
        char    posInch[16] = {'\0'};         //  string to hold inch position for moving router
        char    curRoutPosInch[16] = {'\0'};  //  string to hold router height in inches.   Used on memory screen
        SdFat   sdCard;                       //  pointer to the SD card reader.   Used in storing config and memory files
        uint32_t   bGo = DOWN;                    //  used as a flag to test if the stop button has been pressed.
        char    storeFile[25] = {'\0'};       //  name of file where the memory will be stored
        const  PROGMEM char    fileName[20] = "LiftPinsConfig.cfg";    // Configuration file for the pins, router speed, memory file and version #
        int     eeAddress;
        char    ver[21] = {'\0'};
        int     boardMemory;

/***********************************************************************************
    Setting up an array of structures to hold the config to do a lookup for # of steps required to move a specific distance
    
    <lookup value>, <inch value>, <decimal equivalent>, <baseline steps>

    Baseline steps are based on a 200 steps per turn, on a 1.8 degree pitch, with a lead of 10mm Leed (10mm traveled per turn)
    and no microsteps
                      steps per turn * microsteps*4 (phases)                          inches
                      ---------------------------  = # of steps per mm to travel     --------  =  # of steps per inch travel
                          distance traveled                                         25.4 mm/inch

                          multiply the basics steps by 8, to get the 8 microsteps
************************************************************************************************/
    struct inchCacl {
          int    index;
          char   inches[6];
          float  decimal;
          float  steps;
          char   label[30];
        }  preSetLookup; 
        
        
/**********************
    My easy way to write to the Serial debugger screen
 **********************/
        
        void writeDebug (String toWrite, byte LF)
        {
          if (LF)
          {
            debugLn(toWrite);
          }
          else
          {
            debug(toWrite);
          }
        
        }
 
 /***********************************************************
  *         int   bounceMotorOffLimit (int whichLimit, bool bDirection)
  * 
  *         used to bounce the motor off the limit switch if it has hit the limit
  *              whichLimit ==> which limit switch are we bouncing from
  *              bDirection ==> which direction should the motor bounce
  *         returns True or false
  ************************************************************************/
     void bounceMotorOffLimit (int whichLimit, int bDirection, AccelStepper *whichMotor)
       {
        
         while ( digitalRead(whichLimit) )   //  bounce the screw off the limiter switch
           {
              if (bDirection == DOWN || bDirection == FORWARD)
                {
                  curPos = whichMotor->currentPosition() + 50;      //just the opposit as above
                  whichMotor->move(curPos);
                  whichMotor->setSpeed(workingMotorSpeed);
                }
              else
                {
                  curPos = whichMotor->currentPosition() - 50;      //just the opposite as above
                  whichMotor->move(curPos);
                  whichMotor->setSpeed(-workingMotorSpeed);      
                }
              while (whichMotor->currentPosition() != curPos && digitalRead(whichLimit))
                whichMotor->runSpeed();
          }
        }
/***************************************
    void Flushbuffer()
        when serial write to the Nextion, need to send final command
 ***********************************/
    void FlushBuffer() 
      {
          nexSerial.write(0xff);
          nexSerial.write(0xff);
          nexSerial.write(0xff);
      }

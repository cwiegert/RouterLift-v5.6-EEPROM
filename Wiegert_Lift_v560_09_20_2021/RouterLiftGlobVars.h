#define DEBUG 0
#define UP 0
#define DOWN 1
#define BACK  1             // for the fence
#define FORWARD 0          // for the fence
#define SD_WRITE 53
        

#if DEBUG == 1
#define debug(x) Serial.print (x)
#define debugLn(x) Serial.println(x)
#else
#define debug(x)
#define debugLn(x)
#endif

 
 /*************************************************************************************************************************************
                                    Wiegert Router Lift

                                    v.4.4.1_03_15_2020
                                    3_15_2020
                                    Cory D. Wiegert

                                    This is a router lift controller where an arduino mega is wired through a tb6600 connected to a nema23 stepper motor
                                    using a Nextion 7" intelligent HMI
                                    
                                    01/23/2020 -- CDW   important to note, the GetVal functions for all the Nextion objects will only work if utilizing
                                                        the new base class libraries  NexConfig.h, NexHardware.cpp and NexHardware.h downloaded from
                                                        https://forum.arduino.cc/index.php?topic=620821.0
                                                        
                                                        Through testing - any value over 18000 (at 8 mirosteps) starts overdriving the controller and setting the motors out of phase
                                                        speeds around 450-650 seem to phase switch the motors as well, and shuddering of the lead screw occurs.

                                    01/24/2020 -- CDW   IMPORTANT!!!!! Found out the hard way, if we remove buttons or slide things around in the Nextion IDE, make sure to check the object id's with the
                                                        NEX<object> definition section.   When the create/remove/move around happens in the IDE, it seems the object ID's are modified,
                                                        thereby causing the listening service on the arduino to mis-match the objects with the call backs.
                                                     
                                                        ALSO -- this may be a grounding problem.  If the Nextion is not grounded correctly, there are unexplainable behaviors
                                                        the same error is flagged when there is a grounding problem
                               
                                    02/12/2020 -- CDW   change the settings screen, putting the pin and variable configuration into the UI
                                              v2.7
                                    02/15/2020 -- CDW   Very good complete version finished.   Will call this version 3.0 and will deploy for burn-in test on teh router table.
                                             v 3.0      put to the router table on 2/17/2020, have the file for the Memory screen set to tokenize it based on the section
                                    02/21/2020 -- CDW   Error dialog implemented, fixed error logging to the HMI screen
                                             v 3.13     Deployed to the router lift on 02/22/2020 at 12/30 PM
                                    02/23/2020 -- CDW   added versioning, fixed a couple small bugs, redployed to router 
                                            v. 3.14_02_22_2020
                                    02/23/2020 -- CDW   modified how the pushcallbacks worked for the up and down buttons on settings and memory
                                            v. 3.15_02_23_2020
                                    02/23/2020 -- CDW   modified how all the saves, deletes and write to disk work on Memory screen.   Will add goto 0 button
                                                        on the memory screen as well.   button will lower router after completing mortise cuts
                                    02/24/2020 -- CDW   refactored some of the inefficient code, change some fo the callbacks to point to same functions
                                          v. 4.1_02_24_2020
                                    02/24/2020 -- CDW   refactored the write to file on the Memory screen.  Now, all lines are read to structure, and rewritten
                                                        no more losing content, or having strange characters bleed into the file.   No change to the UI in the Nextion
                                                        IDE
                                          v 4.1.1_02_24_2020
                                    02/26/2020 -- CDW   Refactored memory file config.   Allowed for an old file to be opened, and not over write the data in that file
                                          v. 4.2.1_02_26_2020
                                    02/29/2020  -- CDW  Refactored errors  to read the strings from a file.  Rewrote function ErrorDialog and all ErrorDialog commands
                                          v 4.3.1_02_29_2020
                                    03_07_2020 -- CDW   fixed error on memory screen.   change the check to -currentPosition() because we are storing positions above the tabel as 
                                                        positive #'s
                                          v.4.3.2_03_07_2020
                                    03_08_2020 -- CDW   modified the bits screen, have all the custom bit movement configured in HEIGHTS.cfg, and can configure the bits without
                                                        having to recompile.   Am using the LoadRouterfromMemory function to set the router off the bits screen
                                          v.4.4.0_03_08_2020
                                    03_15_2020 -- CDW   added a position field to the Memory screen so we are notified when the router has stopped moving.   Gives us a flag
                                                        that we have reached the height set in the depth field.
                                          v.4.4.1_03_15_2020
                                    09_19_2020  CDW -- line 1358 moved from 1351.   The top limit switch was not working when router was moving up.   Checking the buffer after 
                                                        a move insted of before the move.   Up function now exactly the same as the down function
                                          v. 5.5.0_0_03_2021
                                    01_03_2021  CDW --   starting the code on the power fence.   Adding ball screw and limit switches, controlling fence with HMI

*****************************************************************************************************************************************************************************/
      
        /****************************************************************************
                 HIGH -- arduino constant for 1.   Used for turning the digital pins on and off.  Motor state, HIGH = ON
                 LOW  -- arduino constant for 0    Used for turning the digital pins on and off   Motor state, LOW = OFF
                          no need to define them here, they are already defined in libraries
        *********************************************************************************/

        #define FILE_WRITE (O_WRITE | O_READ | O_CREAT)   // redefine the file write function based on this thread   https://forum.arduino.cc/index.php?topic=616594.0
        
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
        char   preSetTxt[8];
        int    preSetNum;
        long   lowLimit;                      //  step value when the lower limit switch is hit
        long   highLimit;                     //  step value when the upper limit switch is hit
        double  curPosInch;
        char    posInch[16] = {'\0'};         //  string to hold inch position for moving router
        char    curRoutPosInch[16] = {'\0'};  //  string to hold router height in inches.   Used on memory screen
        SdFat   sdCard;                       //  pointer to the SD card reader.   Used in storing config and memory files
        int    bGo = DOWN;                    //  used as a flag to test if the stop button has been pressed.
        char    storeFile[25] = {'\0'};       //  name of file where the memory will be stored
        const  PROGMEM char    fileName[20] = "LiftPinsConfig.cfg";    // Configuration file for the pins, router speed, memory file and version #
        int     eeAddress;
        char    ver[21] = {'\0'};
        int     boardMemory;

  const int    SD_WRITE = 53;                 //  CS pin of the SD card
       
        byte   directionPin = 11;             // pin that will go to the D+ on the stepper controller  have to declare here, as the stepper is a global class
        byte   fenceDirPin = 28;              // pin that will go to the D+ on the fence stepper controller.   Have to declare here as the stepper is a global class
        byte   stepPin = 13;                  // pin that will wire to the P+ (pulse) on the stepper controlloer   have to declare here as the stepper is a global class
        byte   fenceStepPin = 30;             // pin that will wire to the P+ (pulse) on the Fence stepper controlloer   have to declare here as the stepper is a global class
        byte   enablePin = 6;                 //  pin that will wire to the E+ on the stepper controller.   Turns the controller on and off  LOW turns motor on, HIGH turns motor off
        byte   fenceEnablePin = 26;           //  pin that will wire to the E+ on the fence stepper controller.   Turns the controller on and off  LOW turns motor on, HIGH turns motor off
        int    initSpeed = 2000;              //  sets the initial speed of the motor.   Don't think we need acceleration in the router, but will find out
        long   maxMotorSpeed = 20000;         //  as defined - maximum speed motor will run.   Sets the 100% on the speed slider
        int    maxAcceleration = 6000;        //  maximum number of steps for acceleration
        long   workingMotorSpeed = 3000;      //  active working speed, set by the slider, and will be somewhere between 0 and 100%
        int    stepsPerRevolution = 1600;     //  number of steps required for 1 revolution of leadscrew
        int    microPerStep =   8;            //  number of microSteps.   Set on the TB600 controller by dip switch   Off | On | Off
        float  distPerStep = 0.00024606;      //  inches per step with calculated off <microPerStep> = 8
        int    pulseWidthMicros = 20;         //  miliseconds  -- delay for the steps.   the higher the number the slower the lead screw will turn
        int    millisBetweenSteps = 20;       //  milliseconds - or try 1000 for slower steps   delay between pulses sent to the controller
        SdFat   sdCard;                        //  pointer to the SD card reader.   Used in storing config and memory files
        int     eeAddress =100;
        int     stepSize = 3;
        char    ver[21] = "v. 5.6.0_08_28_2022";


        int         TOP_SWITCH = 7;
        int         BOTTOM_SWITCH = 8;
        int         FRONT_SWITCH = 31;
        int         BACK_SWITCH = 29;
        int         REZERO = 2;
        int         FENCE_ZERO = 32;
                
        
        
      
        int    preSetNum;
        long   lowLimit;                      //  step value when the lower limit switch is hit
        long   highLimit;                     //  step value when the upper limit switch is hit
        double  curPosInch;
        char    posInch[16] = {'\0'};

      //  int    bGo = DOWN;                    //  used as a flag to test if the stop button has been pressed.
     //   char    storeFile[25] = {'\0'};       //  name of file where the memory will be stored
        char    fileName[20] = "LiftPinsConfig.cfg";    // Configuration file for the pins, router speed, memory file and version #

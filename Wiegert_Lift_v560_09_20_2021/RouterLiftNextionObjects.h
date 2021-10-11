 #include <Nextion.h>
        // Declare your Nextion objects - Example (page id = 0, component id = 1, component name = "b0"    Fully qualifying with the screen name
        //  creates ability to use in Serial3.write commands, without having to go through the libraries)
        
        NexPage     Home =           NexPage(0, 0, "Home");
        NexPage     Settings =       NexPage (1,0,"Settings");
        NexButton   bMoveDown =      NexButton(0, 1, "bMoveDown");
        NexButton   bMoveUp =        NexButton(0, 2, "bMoveUp");
        NexButton   bForward =       NexButton (0, 36, "bForward");
        NexButton   bBack =          NexButton (0, 35, "bBack");
        NexButton   bChangeBit =     NexButton(0, 3, "bChangeBit");             //  as it says, the change bit button on the HOME page (not settings)
        NexVariable vaSpeed =        NexVariable( 0, 5, "vaSpeed");       
        NexButton   bReZero =        NexButton ( 0, 6, "bReZero");             //  sets the current position of the lift as the 0 position on the stepper controller
        NexSlider   hMoveSpeed =     NexSlider (0, 8, "Home.hMoveSpeed");      //  the slider for the speed of the motor.
        NexNumber   nSetSpeed =      NexNumber (0, 9, "Home.nSetSpeed");       //  display value on the bottom of the set speed slider
        NexNumber   nSpd0 =          NexNumber (0, 10, "nSpd0");                //  scale fields on the slider.   Becuase we can not calculate decimals in the UI
        NexNumber   nSpd4 =          NexNumber (0, 11, "nSpd4");                //  we have to handle the click here in the arduino
        NexNumber   nSpd2 =          NexNumber (0, 12, "nSpd2");                //  scale fields on the slider.   Becuase we can not calculate decimals in the UI
        NexNumber   nSpd6 =          NexNumber (0, 13, "nSpd6");                //  we are going to use the value of the number field as a % of the max speed of the slider
        NexNumber   nSpd8 =          NexNumber (0, 14, "nSpd8");                //  with that % calculation, we can set the motor speed in steps / second
        NexNumber   nSpd10 =         NexNumber (0, 15, "nSpd10");               //  scale fields on the slider.   Becuase we can not calculate decimals in the UI
        NexDSButton swDirection =    NexDSButton(0, 17, "swDirection");
        NexTouch    cbPreSets =      NexTouch (0, 18, "cbPreSets");             //  Drop down list of standard movements.  ONce selected, the "go" button will move this value
        NexDSButton swHow =          NexDSButton (0, 19, "swHow");              //  instruction set on how to handle the auto distance move
        NexButton   bAutoMove =      NexButton (0, 20, "bAutoMove");            //  the "go" button which will move the router according to the preset check
        NexText     tHolder =        NexText (0, 22, "tHolder");                //  Text box to hold temporary values.   The variable object is not stable in Nextion
        NexText     tHoldCombo =     NexText (0, 23, "tHoldCombo");             //  Tex box to hold temporary values.   The variable object is not stable in Nextion
        NexButton   bToBottom =      NexButton(0, 24, "bToBottom");             //  as it says, the change bit button on the HOME page (not settings)
        NexRadio    rMax =           NexRadio (0, 26, "rMax");                  //  radio button to move at max speed when auto moving to top or bottom limit
        NexButton   bgotoZero =      NexButton (0, 31, "bgotoZero");            //  moves the lift to the new 0 position
        NexDSButton btPower =        NexDSButton (0, 33, "Home.btPower");       //  flips the bit on enablePin to turn the motor controller ON and OFF
        NexText     tPowerStat =     NexText ( 0, 34, "Home.tPowerStat");       //  text field to show the current state of the motor controller
        NexDSButton swFence =        NexDSButton(0, 39, "swFence");             //  Switch to set global for setting which motor to run   router/Fence
        NexButton   bDown =          NexButton (1, 1, "Settings.bDown");        //  move down button on Settings Page
        NexButton   bUp =            NexButton (1, 2, "Settings.bUp");          //  move up on the Settings page
        NexButton   bSetForward =    NexButton (1, 48, "Settings.bSetForward");
        NexButton   bSetBack =       NexButton (1, 47, "Settings.bSetBack");
        NexButton   bLiftBit =       NexButton (1, 3, "bLiftBit");              //  the change bit button on the Settings page.
        NexButton   bZero =          NexButton (1, 5, "bZero");                 //  rezero button on the Settings Page
        NexButton   swDirect =       NexButton (1, 7, "swDirect");             //  Direction button on the Settings Screen
        NexButton   bBottomOut =     NexButton (1, 8, "bBottomOut");            //  move to bottom on the settings page
        NexText     tUpLimit =       NexText (1, 9, "Settings.tUpLimit");       //  Text box to hold the uppoer Limit of the lead screw
        NexText     tLowLimit =      NexText (1, 10, "Settings.tLowLimit");     //  Text Box to hold the Lower limit of the lead screw
        NexButton   bCalib =         NexButton (1, 11, "bCalib");               //  button to move to top and bottom limits.   Mark those limits for later calculations
        NexDSButton btSetOff =       NexDSButton (1, 14, "Settings.btSetOff");  //  flips the bit on enablePin to turn the motor controller ON and OFF on Settings page
        NexText     tMaxSpeed =      NexText (1, 16, "tMaxSpeed");              //  field to configure the max lift speed, will reset the top of the slider
        NexText     tWorkSpeed =     NexText (1, 19, "tWorkSpeed");             //  field to configure working speed, will set the slider
        NexText     tStepSize =      NexText (1, 21, "tStepSize");              //  field to configure stepSize - will control speed 
        NexDSButton swWhich =        NexDSButton(1, 51, "swWhich");             //  toggle switch on the Settings screen to direct which motor is to be used for functions
        NexText     tFileName =      NexText(1, 37, "Settings.tFileName");      //  field to hold the SD card file name to read and write to
        NexText     tVersion =       NexText(1, 42, "Settings.tVersion");       //  field to hold the version of the application and code
        NexButton   bSetPins =       NexButton (1, 38, "bSetPins");             //  Button to download the pins to the arduino and restart
        NexButton   bSetMotor =      NexButton (1, 40, "bSetMotor");            //  Button to download new settings for max, working speeds and step size3
        NexButton   bSaveFile =      NexButton (1, 41, "bSaveFile");            //  resets the configuration file, changes the Memory file name in configPins.cfg set up file
        NexDSButton swFenceDir =     NexDSButton (1, 52, "swFenceDir");          //  switch to set which way to zero the fence.  if Forward, come from the back, if Back, start in front
        NexButton   bSDCard   =      NexButton(2, 58, "bSDCard");               //  button to reset the SD card reader if the card has been pulled and reinserted
        NexButton   bMemZero =       NexButton (2, 62, "bMemZer0");             //  Drops the router bit back to the 0 position, at max speed
        /**************************************************************
           Text fields for the Memory page presets.   These fields will be written to the SD card or EPROMM.  
        ***************************************************************/

        NexVariable vaIndex =       NexVariable (2, 59, "vaIndex");
        NexVariable vaDelIndex =    NexVariable (2, 60, "Memory.vaDelIndex");
        NexVariable vaSDIndex =     NexVariable (2, 61, "vaSDIndex");
        NexVariable vaFence =       NexVariable (0, 40, "Home.vaFence");
        NexText    tMem1 =          NexText (2, 8, "tMem1");
        NexText    tMem2 =          NexText (2, 2, "tMem2");
        NexText    tMem3 =          NexText (2, 4, "tMem3");
        NexText    tMem4 =          NexText (2, 5, "tMem4");
        NexText    tMem5 =          NexText (2, 6, "tMem5");
        NexText    tMem6 =          NexText (2, 3, "tMem6");
        NexText    tMem7 =          NexText (2, 7, "tMem7");
        
        NexText    tZero1 =         NexText(2, 30, "tZero1");
        NexText    tZero2 =         NexText(2, 37, "tZero2");
        NexText    tZero3 =         NexText(2, 32, "tZero3");
        NexText    tZero4 =         NexText(2, 33, "tZero4");
        NexText    tZero5 =         NexText(2, 34, "tZero5");
        NexText    tZero6 =         NexText(2, 35, "tZero6");
        NexText    tZero7 =         NexText(2, 36, "tZero7");
        
        NexText    tMem1Steps =     NexText(2, 38, "tMem1Steps");
        NexText    tMem2Steps =     NexText(2, 39, "tMem2Steps");
        NexText    tMem3Steps =     NexText(2, 40, "tMem3Steps");
        NexText    tMem4Steps =     NexText(2, 41, "tMem4Steps");
        NexText    tMem5Steps =     NexText(2, 42, "tMem5Steps");
        NexText    tMem6Steps =     NexText(2, 43, "tMem6Steps");
        NexText    tMem7Steps =     NexText(2, 44, "tMem7Steps");
        
        NexButton   bSetUp =        NexButton(2, 53, "bSetUp");
        NexButton   bSetDown =      NexButton(2, 54, "bSetDown");
        NexButton   bMemForward =   NexButton (2, 70, "bMemForward");
        NexButton   bMemBack =      NexButton (2, 69, "bMemBack");
        NexButton  bSave1 =         NexButton (2, 9, "bSave1");
        NexButton  bSave2 =         NexButton (2, 11, "bSave2");
        NexButton  bSave3 =         NexButton (2, 13, "bSave3");
        NexButton  bSave4 =         NexButton (2, 15, "bSave4");
        NexButton  bSave5 =         NexButton (2, 17, "bSave5");
        NexButton  bSave6 =         NexButton (2, 19, "bSave6");
        NexButton  bSave7 =         NexButton (2, 21, "bSave7");
        
        NexButton  bDelete1 =       NexButton (2, 10, "bDelete1");
        NexButton  bDelete2 =       NexButton (2, 12, "bDelete2");
        NexButton  bDelete3 =       NexButton (2, 14, "bDelete3");
        NexButton  bDelete4 =       NexButton (2, 16, "bDelete4");
        NexButton  bDelete5 =       NexButton (2, 18, "bDelete5");
        NexButton  bDelete6 =       NexButton (2, 20, "bDelete6");
        NexButton  bDelete7 =       NexButton (2, 22, "bDelete7");
        
        NexButton  btoFile1 =       NexButton (2, 45, "btoFile1");
        NexButton  btoFile2 =       NexButton (2, 46, "btoFile2");
        NexButton  btoFile3 =       NexButton (2, 47, "btoFile3");
        NexButton  btoFile4 =       NexButton (2, 48, "btoFile4");
        NexButton  btoFile5 =       NexButton (2, 49, "btoFile5");
        NexButton  btoFile6 =       NexButton (2, 50, "btoFile6");
        NexButton  btoFile7 =       NexButton (2, 51, "btoFile7");
        
        NexButton  bLoad1 =         NexButton (2, 23, "bLoad1");
        NexButton  bLoad2 =         NexButton (2, 27, "bLoad2");
        NexButton  bLoad3 =         NexButton (2, 28, "bLoad3");
        NexButton  bLoad4 =         NexButton (2, 24, "bLoad4");
        NexButton  bLoad5 =         NexButton (2, 25, "bLoad5");
        NexButton  bLoad6 =         NexButton (2, 26, "bLoad6");
        NexButton  bLoad7 =         NexButton (2, 29, "bLoad7");
        
        NexButton   bLoadMem =       NexButton (2, 52, "bLoadMem");            //Loads data save on SD card into saved memory
        NexDSButton btMemOff =      NexDSButton (2, 56, "Memory.btMemOff");   //  flips the bit on enablePin to turn the motor controller ON and OFF
        NexHotspot  m0       =      NexHotspot ( 6, 2, "m0");                 // Hotspot for the custom bit settings
        NexHotspot  m2      =       NexHotspot (6, 11, "m2");
        NexHotspot  m3      =       NexHotspot (6, 12, "m3");
        NexHotspot  m4      =       NexHotspot (6, 15, "m4");
        NexHotspot  m5      =       NexHotspot (6, 16, "m5");
        NexText     tThickEntry =   NexText (6, 3, "tThickEntry");            // Enter the thickness of the stock to be worked by custom bits
        NexDSButton sw0     =       NexDSButton (6,4, "sw0");                 // decimal or fraction thickness
        NexNumber   nBit    =       NexNumber (6, 22, "nBit");                // field to hold the index of the button pressed
        NexButton  bLoad   =       NexButton (6, 20, "bLoad");               // calculates the total steps and moves the bit to correct height
        NexText    t3       =       NexText (6, 19, "Bits.t3");
        NexButton   bBitsZero =     NexButton( 6, 23, "bBitsZero");           // move to 0 position on the Bits screen
        
 
// Any object for which we will have an event handler will need to be listed in the nex_Listen_list array.   The send component ID must be set in the UI on the event
//  which will be handled

//  01/03/2021 --- add the buttons for moving the fence..   not going to add it today
        
        NexTouch *nex_listen_list[] = {
          &bMoveUp,
          &bMoveDown,
          &bBack,
          &bForward,
          &swHow,
          &bChangeBit,
          &bToBottom,
          &btPower,
          &bAutoMove,
          &swFence,
          &bReZero,
          &bLiftBit,
          &bCalib,
          &bZero,
          &bgotoZero,
          &bUp,
          &bDown,
          &bBottomOut,
          &hMoveSpeed,
          &btSetOff,
          &bSetPins,
          &bSetMotor,
          &bSetForward,
          &bSetBack,
          &swWhich, 
          &nSpd0,
          &nSpd2,
          &nSpd4,
          &nSpd6,
          &nSpd8,
          &nSpd10,
          &cbPreSets,
          &bSetUp,
          &bSetDown,
          &bSave1,
          &bSave2,
          &bSave3,
          &bSave4,
          &bSave5,
          &bSave6,
          &bSave7,
          &bDelete1,
          &bDelete2,
          &bDelete3,
          &bDelete4,
          &bDelete5,
          &bDelete6,
          &bDelete7,
          &btoFile1,
          &btoFile2,
          &btoFile3,
          &btoFile4,
          &btoFile5,
          &btoFile6,
          &btoFile7,
          &bLoad1,
          &bLoad2,
          &bLoad3,
          &bLoad4,
          &bLoad5,
          &bLoad6,
          &bLoad7,
          &bLoadMem,
          &btMemOff,
          &bSaveFile,
          &bSDCard,
          &bMemZero,
          &bMemForward,
          &bMemBack,
          &m0,
          &m2,
          &m3,
          &m4,
          &m5,
          &bLoad,
          &bBitsZero,
          NULL
        };
         

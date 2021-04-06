/**
 * Screen.h
 * *************************
 * For Kent State's ATR Lab  
 * 03/19/2021
 * Dan Maher
 * 
 * Code for operation of revision 1 of the custom 
 * power distribution board (PDB) PCB 
 * 
 * *******************************************************
 * 
 * *******************************************************
**/

#ifndef SCREEN_H
#define SCREEN_H

/******************************/
/**********HEADERS*************/
/******************************/
#include "setup.h"

/********************************/
/**********DECLARATIONS**********/
/********************************/
class Screen{
  public:
  // Initializes the I2C display
  static void initializeDisplay();

  // Runs through the private Screen functions to 
  // update the current display
  static void updateDisplay(Data *d);

  // Reads menu navigation buttons
  static void readButtons();

  // Is display responding
  static bool displayStatus;

  // Is display currently in menu mode
  static bool menuMode;
  

  private:
  // Prints last read voltages to Screen
  static void displayVoltage(float v_11, float v_22);

  // Prints last read relay status to Screen
  static void displayRelay(bool r_11, bool r_22);

  // Displays the menu if menuMode is active
  static void displayMenu();

  // Processes button presses depending on current selection
  static void processPress(int button);

  // Button state tracking (negative logic)
  static bool previousUpState;
  static bool previousDownState;
  static bool previousLeftState;
  static bool previousRightState;
  static bool previousCenterState;

  // Current menu selection
  static int menuIndex;
  // Curren menu page
  static int menuPage;
  // Number of entries in menu
  static int menuLength;
};

#endif

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
 * The screen class allows for the operation of an I2C
 * 128x64 OLED display. In its default state, it provides
 * the current voltages and status of the relays. It 
 * also has a menu mode, which allows the user to change
 * certain settings, some of which are saved in EEPROM.
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

  // Displays shutdown screen
  static void showShutdown(char *code);
  
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
  static void displayPage0();
  static void displayPage1();

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
  const static int menuLength;

  //Column Alignmen
  const static int rightAlign;
};

#endif

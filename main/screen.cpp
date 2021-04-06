/**
 * Screen.cpp
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

/******************************/
/**********HEADERS*************/
/******************************/
#include"screen.h"

/********************************/
/**********DEFINITIONS***********/
/********************************/

static void Screen::initializeDisplay(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){ // Address 0x3C
    displayStatus = false;
  }else{
    displayStatus = true;
    //delay(2000);
    display.clearDisplay();
    display.setTextColor(WHITE);
  }
}

static void Screen::updateDisplay(Data *d){

  // This is supposed to trigger a message to Serial if the display is not
  // responding, and reinitialize if it comes back online. But it doesn't work 
  // right because I2C devices freeze the Arduino when disconnected.  
  // So I'm leaving it in in case I can figure out a fix to this later.
  if(!displayStatus){
    initializeDisplay();
    if(!displayStatus){
      return;
    }
  }
  ////////////////////////////////////////////////////////////////////////////
  
  display.clearDisplay();

  readButtons();
  if(menuMode){
    displayMenu();
  }else{
    Screen::displayVoltage(d->getVoltage_11(), d->getVoltage_22());
    Screen::displayRelay(d->getRelay_11(), d->getRelay_22());
  }
  display.display();
}

static void Screen::displayVoltage(float v_11, float v_22){
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(v_11, 1);
  display.setCursor(72, 0);
  display.println(v_22, 1);
  display.setCursor(0, 16);
  display.setTextSize(1);
  display.println("volts       volts");
}

static void Screen::displayRelay(bool r_11, bool r_22){
  display.setTextSize(3);
  display.setCursor(0, 36);
  if(r_11){
    display.println("ON");
  }else{
    display.println("OFF");
  }

  display.setCursor(72, 36);
  if(r_22){
    display.println("ON");
  }else{
    display.println("OFF");
  }
}

static void Screen::displayMenu(){
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("SETTINGS");
  display.setTextSize(1);
  display.setCursor(7, 16);
  display.println("Relay 1");
  display.setCursor(7, 26);
  display.println("Relay 2");  
  display.display();
  display.setCursor(0, 16 + (menuIndex * 10));
  display.println('>');
}

static void Screen::readButtons(){
  if(!menuMode){
    if(!digitalRead(centerBtn)){
      previousCenterState = 0;
    }else if(digitalRead(centerBtn) && !previousCenterState){
      menuMode = true;
      previousCenterState = 1;
    }else{
      return;
    }
  }
  
  if(!digitalRead(centerBtn)){
    previousCenterState = 0;
  }else if(digitalRead(centerBtn) && !previousCenterState){
    previousCenterState = 1;
  }
  
  if(!digitalRead(downBtn)){
    previousDownState = 0;
  }else if(digitalRead(downBtn) && !previousDownState){
    menuIndex = (menuIndex + 1) % 5;
    previousDownState = 1;
  }

  if(!digitalRead(upBtn)){
    previousUpState = 0;
  }else if(digitalRead(upBtn) && !previousUpState){
    menuIndex = (((menuIndex - 1) % 5) + 5) % 5;
    previousUpState = 1;
    Serial.println(menuIndex);
  }
  
}

bool Screen::displayStatus = false;
bool Screen::menuMode = false;
int Screen::previousUpState = 1;
int Screen::previousDownState = 1;
int Screen::previousLeftState = 1;
int Screen::previousRightState = 1;
int Screen::previousCenterState = 1;
int Screen::menuIndex = 0;

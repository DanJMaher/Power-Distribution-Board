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
  if(!displayStatus){
    initializeDisplay();
    if(!displayStatus){
      return;
    }
  }
  
  display.clearDisplay();
  Screen::displayVoltage(d->getVoltage_11(), d->getVoltage_22());
  Screen::displayRelay(d->getRelay_11(), d->getRelay_22());

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

bool Screen::displayStatus = false;

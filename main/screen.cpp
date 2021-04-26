/**
   Screen.cpp
 * *************************
   For Kent State's ATR Lab
   03/19/2021
   Dan Maher

   Code for operation of revision 1 of the custom
   power distribution board (PDB) PCB

 * *******************************************************

 * *******************************************************
**/

/******************************/
/**********HEADERS*************/
/******************************/
#include"screen.h"

/********************************/
/**********DEFINITIONS***********/
/********************************/

static void Screen::initializeDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C
    displayStatus = false;
  } else {
    displayStatus = true;
    display.clearDisplay();
    display.setTextColor(WHITE);
  }
}

static void Screen::updateDisplay(Data *d) {

  // This is supposed to trigger a message to Serial if the display is not
  // responding, and reinitialize if it comes back online. But it doesn't work
  // right because I2C devices freeze the Arduino when disconnected.
  // This seems to be an inherent issue with the wire library, not this code.
  if (!displayStatus) {
    initializeDisplay();
    if (!displayStatus) {
      return;
    }
  }
  ////////////////////////////////////////////////////////////////////////////

  display.clearDisplay();

  readButtons();
  if (menuMode) {
    if (menuPage == 0) {
      displayPage0();
    } else if (menuPage == 1) {
      displayPage1();
    }
  } else {
    Screen::displayVoltage(d->getVoltage_11(), d->getVoltage_22());
    Screen::displayRelay(d->getRelay_11(), d->getRelay_22());
  }
  display.display();
}

static void Screen::showShutdown(char *code){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("CODE:");
  display.setCursor(34,0);
  display.print(code);
  display.setTextSize(2);
  display.setCursor(0, 16);
  display.println("SHUTTING");
  display.setCursor(0,36);
  display.println("DOWN");
  display.setTextSize(1);
  
  display.display();
  while(1){}
}

static void Screen::displayPage0() {
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("SETTINGS");
  display.setTextSize(1);

  display.setCursor(7, 16);
  display.println("11.1v Relay  - ");
  display.setCursor(rightAlign, 16);
  if (data.getRelay_11()) {
    display.println("ON");
  }
  else {
    display.println("OFF");
  }

  display.setCursor(7, 26);
  display.println("22.2v Relay  - ");
  display.setCursor(rightAlign, 26);
  if (data.getRelay_22()) {
    display.println("ON");
  }
  else {
    display.println("OFF");
  }

  display.setCursor(7, 36);
  display.println("11.1v Floor  - ");
  display.setCursor(rightAlign, 36);
  display.println(cutoffVolts_11v);

  display.setCursor(7, 46);
  display.println("22.2v Floor  - ");
  display.setCursor(rightAlign, 46);
  display.println(cutoffVolts_22v);

  display.setCursor(7, 56);
  display.println("USB Comm     - ");
  display.setCursor(rightAlign, 56);
  if (usb.active) {
    display.println("ON");
  } else {
    display.println("OFF");
  }

  display.setCursor(0, 16 + (menuIndex * 10));
  display.println('>');
}

static void Screen::displayPage1() {
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("SETTINGS");
  display.setTextSize(1);

  display.setCursor(7, 16);
  display.println("Wls Comm     - ");
  display.setCursor(rightAlign, 16);
  if (wls.active) {
    display.println("ON");
  } else {
    display.println("OFF");
  }

  display.setCursor(7, 26);
  display.println("Msg Interval - ");
  display.setCursor(rightAlign, 26);
  display.print(sendInterval);
  display.println("ms");

  display.setCursor(7, 36);
  display.println("Comm Test");
  display.setCursor(rightAlign, 36);

  display.setCursor(7, 46);
  display.println("Shut Down");
  display.setCursor(rightAlign, 46);

  display.setCursor(7, 56);
  display.println("Exit Menu");
  display.setCursor(rightAlign, 56);

  display.setCursor(0, 16 + (menuIndex * 10));
  display.println('>');
}

static void Screen::displayVoltage(float v_11, float v_22) {
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(v_11, 1);
  display.setCursor(72, 0);
  display.println(v_22, 1);
  display.setCursor(0, 16);
  display.setTextSize(1);
  display.println("volts       volts");
}

static void Screen::displayRelay(bool r_11, bool r_22) {
  display.setTextSize(3);
  display.setCursor(0, 36);
  if (r_11) {
    display.println("ON");
  } else {
    display.println("OFF");
  }

  display.setCursor(72, 36);
  if (r_22) {
    display.println("ON");
  } else {
    display.println("OFF");
  }
}

static void Screen::readButtons() {
  if (!menuMode) {
    if (!digitalRead(centerBtn)) {
      previousCenterState = 0;
    } else if (digitalRead(centerBtn) && !previousCenterState) {
      menuMode = true;
      previousCenterState = 1;
    } else {
      return;
    }
  }

  if (!digitalRead(centerBtn)) {
    previousCenterState = 0;
  } else if (digitalRead(centerBtn) && !previousCenterState) {
    processPress(centerBtn);
    previousCenterState = 1;
  }

  if (!digitalRead(downBtn)) {
    previousDownState = 0;
  } else if (digitalRead(downBtn) && !previousDownState) {
    menuIndex = (menuIndex + 1) % menuLength;
    if ((menuPage == 0) && (menuIndex == 0)) {
      menuPage = 1;
      menuIndex = 0;
    }
    previousDownState = 1;
  }

  if (!digitalRead(upBtn)) {
    previousUpState = 0;
  } else if (digitalRead(upBtn) && !previousUpState) {
    menuIndex = (((menuIndex - 1) % menuLength) + menuLength) % menuLength;
    if ((menuPage == 1) && (menuIndex == 4)) {
      menuPage = 0;
      menuIndex = 4;
    }
    previousUpState = 1;
  }

  if (!digitalRead(leftBtn)) {
    previousLeftState = 0;
  } else if (digitalRead(leftBtn) && !previousLeftState) {
    processPress(leftBtn);
    previousLeftState = 1;
  }

  if (!digitalRead(rightBtn)) {
    previousRightState = 0;
  } else if (digitalRead(rightBtn) && !previousRightState) {
    processPress(rightBtn);
    previousRightState = 1;
  }
}

static void Screen::processPress(int button) {

  /*
  **  Processes for first menu page
  ** index 0 = 11.1v relay status/control
  ** index 1 = 22.2v relay status/control
  ** index 2 = 11.1v cutoff voltage
  ** index 3 = 22.2v cutoff voltage
  ** index 4 = USB comm enable/disable
  */

  if (menuPage == 0) {

    // Page 0 Index 0/1: The left/right buttons toggle relays on/off
    if (menuIndex == 0 && (button == rightBtn || button == leftBtn)) {
      Supervision::setRelay(relayPin_11v, !data.getRelay_11());
    }
    else if (menuIndex == 1 && (button == rightBtn || button == leftBtn)) {
      Supervision::setRelay(relayPin_22v, !data.getRelay_22());
    }

    //  Page 0 Index 2/3: The left/right buttons adjust relay cutoff voltages
    if (menuIndex == 2 && button == rightBtn) {
      cutoffVolts_11v += .1;
    } else if (menuIndex == 2 && button == leftBtn) {
      cutoffVolts_11v -= .1;
    }
    if (menuIndex == 3 && button == rightBtn) {
      cutoffVolts_22v += .1;
    } else if (menuIndex == 3 && button == leftBtn) {
      cutoffVolts_22v -= .1;
    }

    // Page 0 Index 4: The left/right buttons enable/disable USB comm
    if (menuIndex == 4 && (button == rightBtn || button == leftBtn)) {
      usb.active = !usb.active;
    }


    /*
    ** Processes for second menu page
    ** index 0 = Wireless communcation status/control
    ** index 1 = Communication interval
    ** index 2 = Send test message
    ** index 3 = Shut down the system
    ** index 4 = Menu exit
    */

  } else if (menuPage == 1) {
    // Page 1 Index 0: The left/right buttons enable/disable wireless comm
    if (menuIndex == 0 && (button == rightBtn || button == leftBtn)) {
      wls.active = !wls.active;
    }

    // Page 1 Index 1: The left/right buttons adjust comm message interval
    if (menuIndex == 1 && (button == rightBtn)) {
      sendInterval += 100;
    } else if ((menuIndex == 1) && (button == leftBtn)) {
    sendInterval -= 100;
  }

  //  Page 1 Index 2: Sends test message via all comm methods
  if ((menuIndex == 2) && (button == centerBtn)) {
    usb.sendTestMessage();
    wls.sendTestMessage();
  }
  if ((menuIndex == 3) && (button == centerBtn)) {
    Supervision::powerDown("menu");
  }

  if ((menuIndex == 4) && (button == centerBtn)) {
    // Saves all EEPROM data before exiting menu
    EEPROM.put(cutoffVoltsAddress_11v, cutoffVolts_11v);
    EEPROM.put(cutoffVoltsAddress_22v, cutoffVolts_22v);
    EEPROM.put(usbCommActiveAddress, usb.active);
    EEPROM.put(wlsCommActiveAddress, wls.active);
    EEPROM.put(commIntervalAddress, sendInterval);
    
    menuMode = 0;
    menuPage = 0;
    menuIndex = 0;
  }
}
}

bool Screen::displayStatus = false;
bool Screen::menuMode = false;
bool Screen::previousUpState = 1;
bool Screen::previousDownState = 1;
bool Screen::previousLeftState = 1;
bool Screen::previousRightState = 1;
bool Screen::previousCenterState = 1;
int Screen::menuPage = 0;
int Screen::menuIndex = 0;
const int Screen::menuLength = 5;
const int Screen::rightAlign = 98;

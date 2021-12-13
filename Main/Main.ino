/* Automatic Washing Machine by P.H. Isuru Jayashankha Wijepala, Faculty of Engineering, University of Peradeniya */

#include <Servo.h>
#include <ShiftedLCD.h>
#include <SPI.h>
#include <EEPROM.h>

Servo drainservo;
LiquidCrystal lcd(10);

#define water_tap 04
#define lvl_1 A2
#define lvl_2 A3
#define lvl_3 A4
#define lvl_4 A5
#define btn1 A6
#define btn2 A7
#define buzzer 05
#define select_relay 8
#define switch_relay 6
#define light_relay 7
int y;
int t;
int soak;
int load;

void setup() {

  drainservo.attach(9);
  drainservo.write(25);

  pinMode(water_tap, OUTPUT);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(switch_relay, OUTPUT);
  pinMode(select_relay, OUTPUT);
  pinMode(light_relay, OUTPUT);

  drain_valve(0);
  beep_once();
  lcd.begin(16, 2);
  lcd.print(" HELLO THERE! ");
  wait(2);

  Serial.begin(9600);

}

void loop() {
  int e = EEPROM[0];
  if (e != 0) {
    screen("Power Failure", "Detected");
    wait(2);
    screen("Resuming Process", "Press A to Abort");
    long prev_time = millis();
    while (millis() - prev_time < 5000) {
      int a = btn();
      if (a == 1) {
        beep_once();
        lcd.clear();
        EEPROM[0] = 0;
        goto Home;
      }
    }
    y = EEPROM[1];
    if (e == 2) {
      soak = EEPROM[4];
      load = EEPROM[3];
    }
    if (e == 3) {
      load = EEPROM[3];
      t = EEPROM[5];
    }
    light(1);

    int z = EEPROM[2];
    if (z == 0) {
      goto Flag0;
    }
    if (z == 1) {
      goto Flag1;
    }
    if (z == 2) {
      goto Flag2;
    }
    if (z == 3) {
      goto Flag3;
    }
    if (z == 4) {
      goto Flag4;
    }
    if (z == 5) {
      goto Flag5;
    }
    if (z == 6) {
      goto Flag6;
    }
    if (z == 7) {
      goto Flag7;
    }
    if (z == 8) {
      goto Flag8;
    }
    if (z == 9) {
      goto Flag9;
    }
    if (z == 10) {
      goto Flag10;
    }
    if (z == 11) {
      goto Flag11;
    }
    if (z == 12) {
      goto Flag12;
    }
    if (z == 13) {
      goto Flag13;
    }
  }

Home:

  screen("Select Mode:", "Auto  Man  Quick");

  int mode;
  int x = 0;

  while (true) {
    int k = readbtn();
    if (k == 2) {
      lcd.clear();
      lcd.noCursor();
      break;
    }
    if (k == 1) {
      if (x == 0) {
        lcd.setCursor(0, 1);
        lcd.cursor();
        mode = 0;
      }
      else if (x == 1) {
        lcd.setCursor(6, 1);
        lcd.cursor();
        mode = 1;
      }
      else if (x == 2) {
        lcd.setCursor(11, 1);
        lcd.cursor();
        mode = 2;
      }
      x = x + 1;
    }
    if (x == 3) {
      x = 0;
    }
  }
  x = 0;

  if (mode == 2) {                /////////////////////  Quick Wash Mode

    screen("   Quick Wash", "A.Begin  B.Back");

    while (true) {
      int k = readbtn();
      if (k == 1) {
        break;
      }
      if (k == 2) {
        goto Home;
      }
    }

    screen("Wait a Moment", "");
    int c = water_lvl();
    if (c > 0) {
      light(1);
      lcd.setCursor(13, 0);
      lcd.blink();
      drain(0);
      lcd.noBlink();
      light(0);
    }
    wait(1);

    screen("Add Soap & Clothes", "Then Press A");

    while (true) {
      int a = readbtn();
      if (a == 1) {
        lcd.clear();
        break;
      }
    }
    light(1);
    EEPROM[0] = 1;

    for (y = 0; y < 3; y++) {

      EEPROM[1] = y;
      EEPROM[2] = 0;

Flag0:

      screen("Filling Water", "");
      lcd.setCursor(13, 0);
      lcd.blink();

      fill(2);
      wait(1);

      EEPROM[2] = 1;
      water(1);
      wait(10);
      water(0);

Flag1:

      screen("Wash in", "Progress");
      lcd.blink();
      if (y == 0) {
        wash(10, 16);
      }
      else {
        wash(5, 16);
      }
      wait(1);

      EEPROM[2] = 2;

Flag2:

      screen("Draining", "");
      lcd.setCursor(8, 0);
      lcd.blink();
      if (y != 2) {
        drain(0);
      }
    }
    lcd.noBlink();
    light(0);
    screen("Finished", "");
    EEPROM[2] = 3;
    beep(10);

Flag3:

    screen("Finished Washing", "Press A to Drain");
    light(0);
    while (true) {
      int a = readbtn();
      if (a == 1) {

        EEPROM[2] = 4;
Flag4:

        screen("Draining", "");
        light(1);
        lcd.setCursor(8, 0);
        lcd.blink();
        drain(0);
        lcd.noBlink();
        light(0);
        EEPROM[0] = 0;
        EEPROM[1] = 0;
        goto Home;
      }
    }
  }                                           ////////////////////////////////////////// End of mode == 2

  else if (mode == 0) {                       ///////////////////////////////////////// Automatic Mode

    screen("Auto Mode", "A.Begin  B.Back");
    while (true) {
      int k = readbtn();
      if (k == 1) {
        break;
      }
      if (k == 2) {
        goto Home;
      }
    }
    screen("Wait a Moment", "");
    int c = water_lvl();
    if (c > 0) {
      lcd.setCursor(13, 0);
      lcd.blink();
      light(1);
      drain(0);
      lcd.noBlink();
      light(0);
    }

    screen("Add Soap & Clothes", "Then Press A");

    while (true) {
      int a = readbtn();
      if (a == 1) {
        lcd.clear();
        break;
      }
    }

    load;   // variable to store the load setting. 2 for LOW, 3 for MID, 4 for HIGH.
    screen("Select Load", "LOW   MID   HIGH");
    while (true) {
      int k = readbtn();
      if (k == 2) {
        lcd.clear();
        lcd.noCursor();
        break;
      }
      if (k == 1) {
        if (x == 0) {
          lcd.setCursor(0, 1);
          lcd.cursor();
          load = 2;
        }
        else if (x == 1) {
          lcd.setCursor(6, 1);
          lcd.cursor();
          load = 3;
        }
        else if (x == 2) {
          lcd.setCursor(12, 1);
          lcd.cursor();
          load = 4;
        }
        x = x + 1;
      }
      if (x == 3) {
        x = 0;
      }
    }
    x = 0;
    EEPROM[3] = load;

    soak; // variable to store whether soak is needed. 1 if needel. else 0;
    screen("Soak 10 mins?", "A.Yes     B.No");
    while (true) {
      int a = readbtn();
      if (a == 1) {
        soak = 1;
        lcd.clear();
        break;
      }
      else if (a == 2) {
        soak = 0;
        break;
      }
    }
    EEPROM[4] = soak;

    screen("All set", "Press A to Begin");
    while (true) {
      int a = readbtn();
      if (a == 1) {
        lcd.clear();
        break;
      }
    }
    EEPROM[0] = 2;
    light(1);

    for (y = 0; y < 4; y++) {
      EEPROM[1] = y;

      EEPROM[2] = 5;
Flag5:

      screen("Filling Water", "");
      lcd.setCursor(13, 0);
      lcd.blink();
      fill(load);
      wait(1);

      EEPROM[2] = 6;
      water(1);
      wait(10);
      water(0);
Flag6:

      if (soak == 1 and y == 0) {
        screen("Pre soak...", "");
        lcd.setCursor(11, 0);
        lcd.blink();
        wash(3, 16);
        screen("Soaking...", "");
        lcd.setCursor(11, 0);
        lcd.blink();
        wait(600);
        lcd.noBlink();
      }

      EEPROM[2] = 7;
Flag7:

      screen("Wash in", "Progress");
      lcd.blink();
      if (y == 0) {
        wash(12, 16);
      }
      else {
        wash(5, 16);
      }
      wait(1);

      EEPROM[2] = 8;
Flag8:

      screen("Draining", "");
      lcd.setCursor(8, 0);
      lcd.blink();
      if (y != 3) {
        drain(0);
      }
    }
    lcd.noBlink();
    light(0);

    light(0);
    screen("Finished", "");
    EEPROM[2] = 9;
    beep(20);
Flag9:
    screen("Finished Washing", "Press A to Drain");
    light(0);
    while (true) {
      int a = readbtn();
      if (a == 1) {
        EEPROM[2] = 10;
Flag10:
        light(1);
        screen("Draining", "");
        lcd.setCursor(8, 0);
        lcd.blink();
        drain(0);
        light(0);
        lcd.noBlink();
        goto Home;
      }
    }
    EEPROM[0] = 0;
    //////////////////////////////////////////////////////// End of automatic mode.
  }

  else if (mode == 1) {                                       //////////////////////// Manual Mode.
    screen("Manual Mode", "A.Begin  B.Back");

    while (true) {
      int k = readbtn();
      if (k == 1) {
        break;
      }
      if (k == 2) {
        goto Home;
      }
    }
menu:
    screen("Select Option:", "Fill Drain Wash");
    int mmode;
    while (true) {
      int k = readbtn();
      if (k == 2) {
        lcd.clear();
        lcd.noCursor();
        EEPROM[0] = 3;
        break;

      }
      if (k == 1) {
        if (x == 0) {
          lcd.setCursor(0, 1);
          lcd.cursor();
          mmode = 1;
        }
        else if (x == 1) {
          lcd.setCursor(5, 1);
          lcd.cursor();
          mmode = 2;
        }
        else if (x == 2) {
          lcd.setCursor(11, 1);
          lcd.cursor();
          mmode = 3;
        }
        x = x + 1;
      }
      if (x == 3) {
        x = 0;
      }
    }
    x = 0;

    if (mmode == 1) {
      load;   // variable to store the load setting. 2 for LOW, 3 for MID, 4 for HIGH.
      screen("Select Level", "LOW   MID   HIGH");
      while (true) {
        int k = readbtn();
        if (k == 2) {
          lcd.clear();
          lcd.noCursor();
          break;
        }
        if (k == 1) {
          if (x == 0) {
            lcd.setCursor(0, 1);
            lcd.cursor();
            load = 2;
          }
          else if (x == 1) {
            lcd.setCursor(6, 1);
            lcd.cursor();
            load = 3;
          }
          else if (x == 2) {
            lcd.setCursor(12, 1);
            lcd.cursor();
            load = 4;
          }
          x = x + 1;
        }
        if (x == 3) {
          x = 0;
        }
      }
      x = 0;

      EEPROM[3] = load;
      EEPROM[2] = 11;
Flag11:
      lcd.clear();
      screen("Filling Water", "");
      lcd.setCursor(13, 0);
      lcd.blink();
      light(1);
      fill(load);
      wait(1);
      water(1);
      wait(10);
      water(0);
      lcd.noBlink();
      light(0);
      beep_once();
      EEPROM[0] = 0;
      screen("Finished filling", "A.Menu    B.Home");
      while (true) {
        int k = readbtn();
        if (k == 1) {
          goto menu;
        }
        if (k == 2) {
          goto Home;
        }
      }
    }
    else if (mmode == 2) {
      EEPROM[2] = 12;
Flag12:

      screen("Draining", "");
      lcd.setCursor(8, 0);
      lcd.blink();
      light(1);
      drain(0);
      lcd.noBlink();
      beep_once();
      EEPROM[0] = 0;
      light(0);
      screen("Drain complete", "A.Menu    B.Home");
      while (true) {
        int k = readbtn();
        if (k == 1) {
          goto menu;
        }
        if (k == 2) {
          goto Home;
        }
      }
    }
    else if (mmode == 3) {
      t;   // variable to store wash time
      screen("Select wash time", "A:5min   B:10min");
      while (true) {
        int k = readbtn();
        if (k == 1) {
          t = 5;
          break;
        }
        if (k == 2) {
          t = 10;
          break;
        }
      }
      EEPROM[5] = t;
      EEPROM[2] = 13;
      
Flag13:

      screen("Wash in Progress", "");
      lcd.blink();
      light(1);
      wash(t, 16);
      lcd.noBlink();
      light(0);
      beep_once();

      EEPROM[0] = 0;
      screen("Wash complete", "A.Menu    B.Home");
      while (true) {
        int k = readbtn();
        if (k == 1) {
          goto menu;
        }
        if (k == 2) {
          goto Home;
        }
      }

    }
  }


}

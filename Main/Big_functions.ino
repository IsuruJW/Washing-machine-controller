int btn() {

  int lastA = 0;
  int lastB = 0;
  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 60;
  int A = 0, a, B = 0, b;

  while (true) {
    a = analogRead(btn1);
    b = analogRead(btn2);
    if (a > 500) {
      A = 1;
    }
    else {
      A = 0;
    }
    if (b > 500) {
      B = 1;
    }
    else {
      B = 0;
    }
    if ( A != lastA or B != lastB) {
      lastDebounceTime = millis();
    }
    if (millis() - lastDebounceTime > debounceDelay) {
      break;
    }
    lastA = A;
    lastB = B;

  }
  if (A == 1) {
    return 1;
  }
  else if (B == 1) {
    return 2;
  }
  else {
    return 0;
  }
}

void wash(int t0, int ct) { //t = time to wash in minutes, ct = half cycle time in seconds
  long t = t0 * 60000;
  ct = ct * 1000;
  long start_time = millis();
  long prev_time = millis();
  while (true) {

    prev_time = millis();
    while (millis() - prev_time < 0.25 * ct) {
      digitalWrite(switch_relay, LOW);
      digitalWrite(select_relay, LOW);
      if (millis() - start_time > t) {
        goto finish;
      }
    }
    prev_time = millis();
    while (millis() - prev_time < 0.25 * ct) {
      digitalWrite(switch_relay, LOW);
      digitalWrite(select_relay, HIGH);
      if (millis() - start_time > t) {
        goto finish;
      }
    }
    prev_time = millis();
    while (millis() - prev_time < 0.5 * ct) {
      digitalWrite(switch_relay, HIGH);
      digitalWrite(select_relay, HIGH);
      if (millis() - start_time > t) {
        goto finish;
      }
    }
    prev_time = millis();
    while (millis() - prev_time < 0.25 * ct) {
      digitalWrite(switch_relay, LOW);
      digitalWrite(select_relay, HIGH);
      if (millis() - start_time > t) {
        goto finish;
      }
    }
    prev_time = millis();
    while (millis() - prev_time < 0.25 * ct) {
      digitalWrite(switch_relay, LOW);
      digitalWrite(select_relay, LOW);
      if (millis() - start_time > t) {
        goto finish;
      }
    }
    prev_time = millis();
    while (millis() - prev_time < 0.5 * ct) {
      digitalWrite(switch_relay, HIGH);
      digitalWrite(select_relay, LOW);
      if (millis() - start_time > t) {
        goto finish;
      }
    }
    if (millis() - start_time > t) {
finish:
      digitalWrite(switch_relay,LOW);
      break;
    }
  }
}

void screen(String x,String y){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(x);
    lcd.setCursor(0,1);
    lcd.print(y);
}

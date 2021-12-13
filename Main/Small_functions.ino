void light(int x) {
  if (x == 1) {
    digitalWrite(light_relay, HIGH);
  }
  else {
    digitalWrite(light_relay, LOW);
  }
}

int readbtn() {
  int x = 0;
  while (x == 0) {
    x = btn();
  }
  beep_once();
  wait(0.05);
  return x;
}

void beep_once() {
  long t = millis();
  while (millis() - t < 100) {
    digitalWrite(buzzer, HIGH);
  }
  digitalWrite(buzzer, LOW);
}

void beep(int t){
  long p = t*1000;
  int x = 200;
  long prev_time = millis();
  while(millis()-prev_time<p){
    for(int y=0; y<4;y++){
      beep_once();
      wait(0.06);
    }
    wait(2);
  }
}

void wait(float t) { // wait fot t seconds
  t = t * 1000;
  long prev_time = millis();
  while (millis() - prev_time < t) {

  }
}

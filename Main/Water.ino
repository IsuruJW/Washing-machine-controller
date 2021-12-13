void water(int x){
  if (x==1){
    digitalWrite(water_tap,HIGH);
  }
  else if (x==0){
    digitalWrite(water_tap,LOW);
  }
}

void drain_valve(int x){
  int pos;
   if (x==1){
    for (pos = 20; pos <= 85; pos += 1) { 
    drainservo.write(pos);                                     
  }
   }
  else if (x==0){
    for (pos = 85; pos >= 20; pos -= 1) { 
    drainservo.write(pos);                                    
    }
   }
  
}

void drain(int x){    // drain until the water level is x
  while(true){
    int l = water_lvl();
    if(l>x){
      drain_valve(1);
    }
    else{
      drain_valve(0);
      break;
      }
  }
}

int check_water_lvl(){
  int L1 = analogRead (lvl_1);
  int L2 = analogRead (lvl_2);
  int L3 = analogRead (lvl_3);
  int L4 = analogRead (lvl_4);
  int ref = 400;
  
  if (L4<ref){
    return 4;
  }
  else if (L3<ref){
    return 3;
  }
  else if (L2<ref){
    return 2;
  }
  else if (L1<ref){
    return 1;
  }
  else {
    return (0);
  }
}

int water_lvl(){
  up:
  int wl = check_water_lvl();
  wait(2);
  int l = check_water_lvl();
  if(wl == l){
    return(wl);
  }
  else{
    goto up;
  }
}

void fill(int x){   // fills water until water level reaches x
  while(true){
    int l = water_lvl();
    if(l>=x){
      water(0);
      break;
    }
    else{
      water(1);
    }
  }
}

const int ALC = 0;
int AclVal = 0;

const int Trig = 2;
const int Echo = 3;

int Duration;
float Distance;

const int motorA = 8;
const int motorB = 6;
const int PWM_motAB = 9;
const int motorC = 10;
const int motorD = 5;
const int PWM_motCD = 11;

int tick = 0;
void setup(){
  Serial.begin(9600);
  pinMode(Trig,OUTPUT); //信号用ピン
  pinMode(Echo,INPUT  ); //信号用ピン
  pinMode(motorA,OUTPUT); //信号用ピン
  pinMode(motorB,OUTPUT); //信号用ピン
  pinMode(motorC,OUTPUT); //信号用ピン
  pinMode(motorD,OUTPUT); //信号用ピン
}

//前進
void forward()
{
  digitalWrite(motorA,HIGH);
  digitalWrite(motorB,LOW);
  digitalWrite(motorC,HIGH);
  digitalWrite(motorD,LOW);

  analogWrite(PWM_motAB,255); 
  analogWrite(PWM_motCD,255); 
}

//千鳥足で前進
void chidoriForward()
{
  digitalWrite(motorA,HIGH);
  digitalWrite(motorB,LOW);
  digitalWrite(motorC,HIGH);
  digitalWrite(motorD,LOW);

  if (tick < 20) {
    analogWrite(PWM_motAB,255); 
    analogWrite(PWM_motCD,120); 
  } else {
    analogWrite(PWM_motAB,200); 
    analogWrite(PWM_motCD,255);   
  }
  
  tick = (tick + 1) % 40;
}

//後退
void backward()
{
  digitalWrite(motorA,LOW);
  digitalWrite(motorB,HIGH);
  digitalWrite(motorC,LOW);
  digitalWrite(motorD,HIGH);

  analogWrite(PWM_motAB,255); 
  analogWrite(PWM_motCD,255); 
}

void _stop()
{
  //全てのモータを停止
  digitalWrite(motorA,LOW);
  digitalWrite(motorB,LOW);
  digitalWrite(motorC,LOW);
  digitalWrite(motorD,LOW);
}

void loop(){
  //アルコールセンサの値を読む
  AclVal = analogRead(ALC);
  Serial.print("alc ");
  Serial.println(AclVal);

  //超音波センサで距離を測定
  digitalWrite(Trig, LOW);
  delayMicroseconds(1);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(1);
  digitalWrite(Trig, LOW);
  Duration = pulseIn(Echo, HIGH);    //発信してから受け取るタイムラグ
  Serial.print(Duration);
  Serial.print(" us ");
  if (Duration > 0) {
    Distance = Duration / 2;
    Distance = Distance * 340 * 100 / 1000000; //時速から距離に換算cm
    Serial.print(Distance);
    Serial.println(" cm");
  }
  
  if(AclVal <= 200){
    _stop();    //アルコールが切れたら止まる
  }
  else{
    if(Distance <= 10)
      backward();    //障害物があると後ずさり
    else
      chidoriForward();    //アルコールがある限り千鳥足で歩く
  }
  delay(200);
}

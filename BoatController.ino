byte PWM_POWER = 10;
byte PWM_DIR = 11;

#define PIN_ENA 9 
#define PIN_ENB 3
#define PIN_IN1 7
#define PIN_IN2 6
#define PIN_IN3 A1
#define PIN_IN4 A2

#define MIN_RC 1200
#define MAX_RC 1800

#define LED_ON 13

#define ROT_NONE 0
#define ROT_LEFT 1
#define ROT_RIGHT 2

struct motor {
  byte pinEnable;
  byte in1;
  byte in2;
};

motor motorL = {PIN_ENA, PIN_IN1, PIN_IN2};
motor motorR = {PIN_ENB, PIN_IN3, PIN_IN4};

void setup() {
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);

  pinMode(PWM_POWER, INPUT);
  pinMode(PWM_DIR, INPUT);
  pinMode(LED_ON, OUTPUT);

  Serial.begin(9600);
}



bool powerOn = false;

void loop() {
  
  int powerRC = 0;
  int powerL = 0;
  int powerR = 0;
  int dir = 0;

  powerRC = map(pulseIn(PWM_POWER, HIGH), MIN_RC, MAX_RC, -255, 255);
  dir = map(pulseIn(PWM_DIR, HIGH), MIN_RC, MAX_RC, -255, 255);

  powerL = powerRC;
  powerR = powerRC;
  
  if(powerRC > 20) {
    if (dir < -20) powerR = map(-dir, 255, 0, 0, powerRC);
    else if (dir > 20) powerL = map(dir, 255, 0, 0, powerRC);
    setMotor(motorL, powerL, ROT_LEFT);
    setMotor(motorR, powerR, ROT_RIGHT);
    
  }
  else if (powerRC < -20){
    powerL = -powerRC;
    powerR = -powerRC;
    if (dir < -20) powerL = map(-dir, 255, 0, 0, -powerRC);
    else if (dir > 20) powerR = map(dir, 255, 0, 0, -powerRC);
    setMotor(motorL, powerL, ROT_RIGHT);
    setMotor(motorR, powerR, ROT_LEFT);
  }
  else {
    setMotor(motorL, 0, ROT_NONE);
    setMotor(motorR, 0, ROT_NONE);
  }
  
  delay(10);
}
  
void setMotor(motor m, byte power, byte rotDir) {

   byte in1 = rotDir == ROT_LEFT ? HIGH : LOW;
   byte in2 = rotDir == ROT_RIGHT ? HIGH : LOW;
   analogWrite(m.pinEnable, power);
   digitalWrite(m.in1, in1);
   digitalWrite(m.in2, in2);
  
}

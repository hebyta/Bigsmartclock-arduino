//Variables

byte hours = 0;
byte minutes = 0;
byte seconds = 0;
byte milliseconds = 0;

unsigned long currentTime;
unsigned long lastTime = 0;

const byte INITIAL_HOURS = 0;
const byte INITIAL_MINUTES = 0;
const byte INITIAL_SECONDS = 0;
const byte INITIAL_MILLISECONDS = 0;
const byte INTERVAL_MILLISECONDS = 10;


//Métodos

void setup() {
  Serial.begin(9600);

  hours = INITIAL_HOURS;
  minutes = INITIAL_MINUTES;
  seconds = INITIAL_SECONDS;
  milliseconds = INITIAL_MILLISECONDS;

}

void loop() {
  calculate();
  draw();

}

void calculate() {
  currentTime = millis();
  if (shouldBeClockUpdated()) {
    calculateTime();   
    lastTime = currentTime;
  }

}

boolean shouldBeClockUpdated() {
  return currentTime >= lastTime + INTERVAL_MILLISECONDS;

}

void draw() {
  String output = "";
  String hoursStr;
  String minutesStr;
  String secondsStr;
  if (hours < 10) {
    hoursStr = "0" + hours;
  }
  else {
    hoursStr = "" + hours;
  }
    if (minutes < 10) {
    minutesStr = "0" + minutes;
  }
  else {
    minutesStr = "" + minutes;
  }
    if (seconds < 10) {
    secondsStr = "0" + seconds;
  }
  else {
    secondsStr = "" + seconds;
  }
  output = hoursStr + ":" + minutesStr + ":" + secondsStr;
  Serial.println(output); 
}

void calculateTime() {
  milliseconds += INTERVAL_MILLISECONDS;
  if (milliseconds >= 1000) {
    seconds++;
    milliseconds = 0;
  }
  if (seconds >= 60) {
    minutes++;
    seconds = 0;
  }
  if (minutes >= 60) {
    hours++;
    minutes = 0;
  }
  if (hours >= 24) {
    hours = 0;
  }
}

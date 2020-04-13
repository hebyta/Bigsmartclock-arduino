//pines

#define BUTTON_UP_PIN 4
#define BUTTON_DOWN_PIN 7
#define BUTTON_START_STOP_PIN 8
#define BUTTON_MODE_PIN 2


//Variables

byte mode;
byte buttonUp;
byte buttonDown;
byte buttonStartStop;
byte buttonMode;
unsigned long lastTimeReadButtons;
int years;
byte month;
byte days;
char *week[7] = {"Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo"};
byte daysOfMonths[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
char *monthOfYears[12] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
byte hours;
byte minutes;
byte seconds;
unsigned long milliseconds;
unsigned long currentTime;
unsigned long lastTimeClock;
unsigned long elapsedTime;

const int INITIAL_YEARS = 2020;
const byte INITIAL_MONTH = 1;
const byte INITIAL_DAYS = 1;
const byte INITIAL_HOURS = 0;
const byte INITIAL_MINUTES = 0;
const byte INITIAL_SECONDS = 0;
const byte INITIAL_MILLISECONDS = 0;
const int INTERVAL_MILLISECONDS = 10;
const int INTERVAL_BUTTON_MILLISECONDS = 200;
const byte CLOCK_MODE = 1;
const byte CHRONOMETER_MODE = 2;
const byte COUNTDOWN_MODE = 3;


//Métodos

void setup() {
  Serial.begin(9600);

  years = INITIAL_YEARS;
  month = INITIAL_MONTH;
  days = INITIAL_DAYS;
  hours = INITIAL_HOURS;
  minutes = INITIAL_MINUTES;
  seconds = INITIAL_SECONDS;
  milliseconds = INITIAL_MILLISECONDS;
  mode = CLOCK_MODE;
  pinMode(BUTTON_UP_PIN, INPUT);
  pinMode(BUTTON_DOWN_PIN, INPUT);
  pinMode(BUTTON_START_STOP_PIN, INPUT);
  pinMode(BUTTON_MODE_PIN, INPUT);
}


void loop() {
  readTime();
  readSensors();
  calculate();
  draw();
}


void readSensors() {
  readActuators();
}


void readActuators() {
  if (shouldBeReadButtons()) {
    buttonUp = digitalRead(BUTTON_UP_PIN);
    buttonDown = digitalRead(BUTTON_DOWN_PIN);
    buttonStartStop = digitalRead(BUTTON_START_STOP_PIN);
    buttonMode = digitalRead(BUTTON_MODE_PIN);
    lastTimeReadButtons = currentTime;

  }
}


void readTime() {
  currentTime = millis();
  if (lastTimeClock > currentTime) {
    lastTimeClock = currentTime;
  }
  if (lastTimeReadButtons > currentTime) {
    lastTimeReadButtons = currentTime;
  }
  elapsedTime = currentTime - lastTimeClock;
}


void calculate() {
  checkMode();
  if (shouldBeClockUpdated()) {
    calculateClockTime();
    lastTimeClock = currentTime;
  }

}

boolean shouldBeReadButtons() {
  return currentTime >= lastTimeReadButtons + INTERVAL_BUTTON_MILLISECONDS;
}

boolean shouldBeClockUpdated() {
  return currentTime >= lastTimeClock + INTERVAL_MILLISECONDS;
}



void draw() {
  if (mode == CLOCK_MODE) {
    drawClock();
  }
}


void checkMode() {
  if (buttonMode == HIGH) {
    if (mode == CLOCK_MODE) {
      mode = CHRONOMETER_MODE;
    }
    else if (mode == CHRONOMETER_MODE) {
      mode = COUNTDOWN_MODE;
    }
    else if (mode == COUNTDOWN_MODE) {
      mode = CLOCK_MODE;
    }
    Serial.println(mode);
    buttonMode = LOW;
  }

}


void drawClock() {
  String output = "";
  String yearsStr;
  String monthStr;
  String daysStr;
  String hoursStr;
  String minutesStr;
  String secondsStr;
  String millisecondsStr;
  int dayOfWeek = getDayOfWeek();

  if (years < 10) {
    yearsStr = "000" + String(years);
  }
  else if (years < 100) {
    yearsStr = "00" + String(years);
  }
  else if (years < 1000) {
    yearsStr = "0" + String(years);
  }

  else {
    yearsStr = String(years);
  }

  if (month < 10) {
    monthStr = "0" + String(month);
  }
  else {
    monthStr = String(month);
  }

  if (days < 10) {
    daysStr = "0" + String(days);
  }
  else {
    daysStr = String(days);
  }


  if (hours < 10) {
    hoursStr = "0" + String(hours);
  }
  else {
    hoursStr = String(hours);
  }

  if (minutes < 10) {
    minutesStr = "0" + String(minutes);
  }
  else {
    minutesStr = String(minutes);
  }

  if (seconds < 10) {
    secondsStr = "0" + String(seconds);
  }
  else {
    secondsStr = String(seconds);
  }

  if (milliseconds < 100) {
    millisecondsStr = "0" + String(milliseconds / 10);
  }
  else {
    millisecondsStr = String(milliseconds / 10);
  }

  //output = String(week[dayOfWeek - 1]) + "  " + String(monthOfYears[month - 1]) + "  " + daysStr + "/" + monthStr + "/" + yearsStr + "  " + hoursStr + ":" + minutesStr + ":" + secondsStr + "." + millisecondsStr;
  //output = hoursStr + ":" + minutesStr + ":" + secondsStr + "." + millisecondsStr;

  //Serial.println(output);
}


void calculateClockTime() {
  byte daysMonths = daysOfMonths[month - 1];
  if (month == 2 && isLeapYears()) {
    daysMonths = 29;

  }

  milliseconds += elapsedTime;
  if (milliseconds >= 1000) {
    seconds++;
    milliseconds = milliseconds - 1000;
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
    days++;
    hours = 0;
  }
  if (days > daysMonths) {
    month++;
    days = 1;

  }
  if (month > 12) {
    years++;
    month = 1;
  }
}

boolean isLeapYears() {
  if (years % 4 == 0) {
    if (years % 100 != 0) {
      return true;
    }
    else if (years % 100 == 0 && years % 400 == 0) {
      return true;
    }
  }
  return false;
}

int getDayOfWeek() {
  int aux = (14 - month) / 12;
  int yearsAux = years - aux;
  int monthAux = month + 12 * aux - 2;
  int dayAux = (days + yearsAux + yearsAux / 4 - yearsAux / 100 + yearsAux / 400 + (31 * monthAux) / 12) % 7;
  if (dayAux == 0) {
    return 7;
  }
  else {
    return dayAux;
  }
}

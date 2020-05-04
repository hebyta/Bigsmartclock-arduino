//Etiquetas

#define INITIAL_YEARS 2020
#define INITIAL_MONTH 4
#define INITIAL_DAYS 27
#define INITIAL_HOURS 3
#define INITIAL_MINUTES 57
#define INITIAL_SECONDS 0
#define INITIAL_MILLISECONDS 0
#define INTERVAL_MILLISECONDS_CLOCK 10
#define INTERVAL_BUTTON_MILLISECONDS 200
#define INTERVAL_MILLISECONDS_CHRONOMETER 10
#define INTERVAL_RESET 2000
#define INTERVAL_MILLISECONDS_COUNTDOWN 10
#define CLOCK_MODE 1
#define CHRONOMETER_MODE 2
#define COUNTDOWN_MODE 3
#define CHRONOMETER_START_MODE 4
#define CHRONOMETER_STOP_MODE 5

//Etiquetas pines

#define BUTTON_UP_PIN 4
#define BUTTON_DOWN_PIN 7
#define BUTTON_START_STOP_PIN 8
#define BUTTON_MODE_PIN 2


//Variables

boolean isCountDownRunning = false;
boolean isChronometerRunning = false;
byte mode;
byte buttonUp;
byte buttonDown;
byte buttonStartStop;
byte buttonMode;
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
unsigned int hoursChronometer;
byte minutesChronometer;
byte secondsChronometer;
unsigned long millisecondsChronometer;
unsigned int initialHoursCountDown;
byte initialMinutesCountDown;
byte initialSecondsCountDown;
unsigned int hoursCountDown;
byte minutesCountDown;
byte secondsCountDown;
unsigned millisecondsCountDown;
unsigned long currentTime;
unsigned long lastTimeClock;
unsigned long elapsedTimeClock;
unsigned long lastTimeChronometer;
unsigned long elapsedTimeChronometer;
unsigned long lastTimeForResetChronometer;
unsigned long lastTimeReadButtons;
unsigned long lastTimeCountDown;
unsigned long lastTimeForResetCountDown;
unsigned long elapsedTimeCountDown;


//Métodos

void setup() {
  Serial.begin(9600);

  initialHoursCountDown = 0;
  initialMinutesCountDown = 30;
  initialSecondsCountDown = 0;
  isCountDownRunning = false;
  hoursCountDown = initialHoursCountDown;
  minutesCountDown = initialMinutesCountDown;
  secondsCountDown = initialSecondsCountDown;
  millisecondsCountDown = 0;
  isChronometerRunning = false;
  hoursChronometer = 0;
  minutesChronometer = 0;
  secondsChronometer = 0;
  millisecondsChronometer = 0;
  years = INITIAL_YEARS;
  month = INITIAL_MONTH;
  days = INITIAL_DAYS;
  hours = INITIAL_HOURS;
  minutes = INITIAL_MINUTES;
  seconds = INITIAL_SECONDS;
  milliseconds = INITIAL_MILLISECONDS;
  mode = CLOCK_MODE;
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_START_STOP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_MODE_PIN, INPUT_PULLUP);
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
    lastTimeReadButtons = currentTime; // ultimo instante de lectura de los botones
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
  if (lastTimeChronometer > currentTime) {
    lastTimeChronometer = currentTime;
  }
  if (lastTimeCountDown > currentTime) {
    lastTimeCountDown = currentTime;
  }
  elapsedTimeClock = currentTime - lastTimeClock;
  elapsedTimeChronometer = currentTime - lastTimeChronometer;
  elapsedTimeCountDown = currentTime - lastTimeCountDown;
}


void calculate() {
  checkMode();
  checkStartStop();
  if (shouldBeClockUpdated()) {
    calculateClockTime();
    lastTimeClock = currentTime;
  }
  if (shouldBeChronometerUpdated()) {
    if (isChronometerRunning) {
      calculateChronometer();
    }
    lastTimeChronometer = currentTime;
  }
  if (shouldBeCountDownUpdated()) {
    if (isCountDownRunning) {
      calculateCountDown();
    }
    lastTimeCountDown = currentTime;
  }
}


boolean shouldBeCountDownReset() {
  return currentTime >= lastTimeForResetCountDown + INTERVAL_RESET;
}


boolean shouldBeCountDownUpdated() {
  return currentTime >= lastTimeCountDown + INTERVAL_MILLISECONDS_COUNTDOWN;
}


boolean shouldBeChronometerReset() {
  return currentTime >= lastTimeForResetChronometer + INTERVAL_RESET;
}


boolean shouldBeChronometerUpdated() {
  return currentTime >= lastTimeChronometer + INTERVAL_MILLISECONDS_CHRONOMETER;
}


boolean shouldBeReadButtons() {
  return currentTime >= lastTimeReadButtons + INTERVAL_BUTTON_MILLISECONDS;
}


boolean shouldBeClockUpdated() {
  return currentTime >= lastTimeClock + INTERVAL_MILLISECONDS_CLOCK;
}


void draw() {
  if (mode == CLOCK_MODE) {
    drawClock();
  }
  if (mode == CHRONOMETER_MODE) {
    drawChronometer();
  }
  if (mode == COUNTDOWN_MODE) {
    drawCountDown();
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
    buttonMode = LOW;
  }
}


void checkStartStop() {
  if (buttonStartStop == HIGH) {
    if (mode == CHRONOMETER_MODE) {
      if (lastTimeForResetChronometer == 0) {
        if (isChronometerRunning) {
          isChronometerRunning = false;
        }
        else {
          isChronometerRunning = true;
        }
        lastTimeForResetChronometer = currentTime;
      }
      if (shouldBeChronometerReset()) {
        hoursChronometer = 0;
        minutesChronometer = 0;
        secondsChronometer = 0;
        millisecondsChronometer = 0;
        isChronometerRunning = false;
      }
    }
    if (mode == COUNTDOWN_MODE) {
      if (lastTimeForResetCountDown == 0) {
        if (isCountDownRunning) {
          isCountDownRunning = false;
        }
        else {
          isCountDownRunning = true;
        }
        lastTimeForResetCountDown = currentTime;
      }
      if (shouldBeCountDownReset()) {
        hoursCountDown = initialHoursCountDown;
        minutesCountDown = initialMinutesCountDown;
        secondsCountDown = initialSecondsCountDown;
        millisecondsCountDown = 0;
        isCountDownRunning = false;
      }
    }
  }
  else {
    lastTimeForResetChronometer = 0;
    lastTimeForResetCountDown = 0;
  }
}


void calculateCountDown() {
  if (hoursCountDown == 0 && minutesCountDown == 0 && secondsCountDown == 0) {
    isCountDownRunning = false;
    return;
  }
  millisecondsCountDown += elapsedTimeCountDown;
  if (millisecondsCountDown >= 1000) {
    secondsCountDown--;
    millisecondsCountDown = millisecondsCountDown - 1000;
  }
  if (secondsCountDown <= 0) {
    minutesCountDown--;
    secondsCountDown = 59;
  }
  if (minutesCountDown <= 0) {
    hoursCountDown--;
    minutesCountDown = 59;
  }

}


void drawCountDown() {
  String output = "";
  String hoursStr;
  String minutesStr;
  String secondsStr;
  String millisecondsStr;

  if (hoursCountDown < 10) {
    hoursStr = "0" + String(hoursCountDown);
  }
  else {
    hoursStr = String(hoursCountDown);
  }

  if (minutesCountDown < 10) {
    minutesStr = "0" + String(minutesCountDown);
  }
  else {
    minutesStr = String(minutesCountDown);
  }

  if (secondsCountDown < 10) {
    secondsStr = "0" + String(secondsCountDown);
  }
  else {
    secondsStr = String(secondsCountDown);
  }

  output = hoursStr + ":" + minutesStr + ":" + secondsStr;

  Serial.println(output);
}


void calculateChronometer() {
  millisecondsChronometer += elapsedTimeChronometer;
  if (millisecondsChronometer >= 1000) {
    secondsChronometer++;
    millisecondsChronometer = millisecondsChronometer - 1000;
  }
  if (secondsChronometer >= 60) {
    minutesChronometer++;
    secondsChronometer = 0;
  }
  if (minutesChronometer >= 60) {
    hoursChronometer++;
    minutesChronometer = 0;
  }

}


void drawChronometer() {
  String output = "";
  String hoursStr;
  String minutesStr;
  String secondsStr;
  String millisecondsStr;

  if (hoursChronometer < 10) {
    hoursStr = "0" + String(hoursChronometer);
  }
  else {
    hoursStr = String(hoursChronometer);
  }

  if (minutesChronometer < 10) {
    minutesStr = "0" + String(minutesChronometer);
  }
  else {
    minutesStr = String(minutesChronometer);
  }

  if (secondsChronometer < 10) {
    secondsStr = "0" + String(secondsChronometer);
  }
  else {
    secondsStr = String(secondsChronometer);
  }

  if (millisecondsChronometer < 100) {
    millisecondsStr = "0" + String(millisecondsChronometer / 10);
  }
  else {
    millisecondsStr = String(millisecondsChronometer / 10);
  }

  output = hoursStr + ":" + minutesStr + ":" + secondsStr + "." + millisecondsStr;

  Serial.println(output);
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
  int dayOfWeek = getDayOfWeek(years, month, days);

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

  output = String(week[dayOfWeek - 1]) + "  " + String(monthOfYears[month - 1]) + "  " + daysStr + "/" + monthStr + "/" + yearsStr + "  " + hoursStr + ":" + minutesStr + ":" + secondsStr + "." + millisecondsStr;
  // output = hoursStr + ":" + minutesStr + ":" + secondsStr + "." + millisecondsStr;

  Serial.println(output);
}


void calculateClockTime() {
  byte daysMonths = daysOfMonths[month - 1];
  if (month == 2 && isLeapYears(years)) {
    daysMonths = 29;

  }

  milliseconds += elapsedTimeClock;
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

/** documentacion metodo
   calcula si el año es bisiesto
    @param years año con el que calcularemos si es bisiesto
    @return true si es bisiesto y false si no.
*/
boolean isLeapYears(int years) {
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

int getDayOfWeek(int years, byte month, byte days) {
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

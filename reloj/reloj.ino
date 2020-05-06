//Librerías

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif


//Etiquetas pines

#define BUTTON_UP_PIN 4
#define BUTTON_DOWN_PIN 7
#define BUTTON_START_STOP_PIN 8
#define BUTTON_MODE_PIN 2
#define TENTHS_DISPLAY_PIN 6
#define SECONDS_DISPLAY_PIN 10
#define MINUTES_DISPLAY_PIN 5
#define HOURS_DISPLAY_PIN 3
#define DOTS_DISPLAY_PIN 9


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
Adafruit_NeoPixel displayTenths = Adafruit_NeoPixel(21, TENTHS_DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel displaySeconds = Adafruit_NeoPixel(42, SECONDS_DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel displayMinutes = Adafruit_NeoPixel(42, MINUTES_DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel displayHours = Adafruit_NeoPixel(42, HOURS_DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel displayDots = Adafruit_NeoPixel(5, DOTS_DISPLAY_PIN, NEO_GRB + NEO_KHZ800);


//Constantes

const int INITIAL_YEARS = 2020;
const byte INITIAL_MONTH = 4;
const byte INITIAL_DAYS = 27;
const byte INITIAL_HOURS = 3;
const byte INITIAL_MINUTES = 57;
const byte INITIAL_SECONDS = 0;
const int INITIAL_MILLISECONDS = 0;
const unsigned long INTERVAL_MILLISECONDS_CLOCK = 10;
const unsigned long INTERVAL_BUTTON_MILLISECONDS = 200;
const unsigned long INTERVAL_MILLISECONDS_CHRONOMETER = 10;
const unsigned long INTERVAL_RESET = 2000;
const unsigned long INTERVAL_MILLISECONDS_COUNTDOWN = 10;
const byte CLOCK_MODE = 1;
const byte CHRONOMETER_MODE = 2;
const byte COUNTDOWN_MODE = 3;
const byte CHRONOMETER_START_MODE = 4;
const byte CHRONOMETER_STOP_MODE = 5;
const byte LITTLE_ZERO[21] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0};
const byte LITTLE_ONE[21] = {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1};
const byte LITTLE_TWO[21] = {0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const byte LITTLE_THREE[21] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1};
const byte LITTLE_FOUR[21] = {0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1};
const byte LITTLE_FIVE[21] = {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1};
const byte LITTLE_SIX[21] = {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
const byte LITTLE_SEVEN[21] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const byte LITTLE_EIGHT[21] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
const byte LITTLE_NINE[21] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1};
const byte BIG_ZERO[42] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};
const byte BIG_ONE[42] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1};
const byte BIG_TWO[42] = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const byte BIG_THREE[42] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1};
const byte BIG_FOUR[42] = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
const byte BIG_FIVE[42] = {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
const byte BIG_SIX[42] = {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
const byte BIG_SEVEN[42] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const byte BIG_EIGHT[42] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
const byte BIG_NINE[42] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};


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
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  displayTenths.begin(); // This initializes the NeoPixel library.
  displaySeconds.begin();
  displayMinutes.begin();
  displayHours.begin();
  displayDots.begin();
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


void checkUp() {
  if (buttonUp == HIGH) {
    if (mode == CLOCK_MODE) {

    }
    if (mode == COUNTDOWN_MODE) {

    }
  }
}

void checkDown() {
  if (buttonDown == HIGH) {
    if (mode == CLOCK_MODE) {

    }
    if (mode == COUNTDOWN_MODE) {

    }
  }
}

void drawDoubleDisplay(Adafruit_NeoPixel displayToDraw, int numberDisplay, bool isBig) {
  byte firstDigit;
  byte secondDigit;

  if (numberDisplay >= 0 && numberDisplay < 10) {
    firstDigit = 0;
    secondDigit = numberDisplay;
  }
  else if (numberDisplay >= 10) {
    String numberDisplayStr = String(numberDisplay);
    firstDigit = String(numberDisplayStr[0]).toInt();
    secondDigit = String(numberDisplayStr[1]).toInt();
  }
  drawDisplay(displayToDraw, firstDigit, isBig, 1);
  drawDisplay(displayToDraw, secondDigit, isBig, 2);
}


void drawDisplay (Adafruit_NeoPixel displayToDraw, byte drawDigit, bool isBig, byte positionToDraw) {
  if (isBig) {
    byte numberToDraw[42];
    getDrawNumber(drawDigit, isBig, numberToDraw);
    for (int i = 0; i < 42; i++) {
      if (numberToDraw[i] == 1) {
        int pixelOn;
        if (positionToDraw == 1) {
          pixelOn = i;
        }
        if (positionToDraw == 2) {
          pixelOn = i + 42;
        }
        displayToDraw.setPixelColor(pixelOn, displayToDraw.Color(23, 161, 165)); // Moderately bright green color.
        displayToDraw.setBrightness(5);
        displayToDraw.show();// This sends the updated pixel color to the hardware.
      }
    }
  }
  else {
    byte numberToDraw[21];
    getDrawNumber(drawDigit, isBig, numberToDraw);
    for (int i = 0; i < 21; i++) {
      if (numberToDraw[i] == 1) {
        int pixelOn;
        if (positionToDraw == 1) {
          pixelOn = i;
        }
        if (positionToDraw == 2) {
          pixelOn = i + 21;
        }
        displayToDraw.setPixelColor(pixelOn, displayToDraw.Color(23, 161, 165)); // Moderately bright green color.
        displayToDraw.setBrightness(5);
        displayToDraw.show();// This sends the updated pixel color to the hardware.
      }
    }
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


void drawClock() {
  drawDoubleDisplay(displayTenths, milliseconds, false);

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


void getDrawNumber(byte drawDigit, bool isBig, byte *number) {

  switch (drawDigit) {
    case 0:
      if (isBig) {
        number = BIG_ZERO;
      }
      else {
        number = LITTLE_ZERO;
      }
      break;

    case 1:
      if (isBig) {
        number = BIG_ONE;
      }
      else {
        number = LITTLE_ONE;
      }
      break;
    case 2:
      if (isBig) {
        number = BIG_TWO;
      }
      else {
        number = LITTLE_TWO;
      }
      break;
    case 3:
      if (isBig) {
        number = BIG_THREE;
      }
      else {
        number = LITTLE_THREE;
      }
      break;
    case 4:
      if (isBig) {
        number = BIG_FOUR;
      }
      else {
        number = LITTLE_FOUR;
      }
      break;
    case 5:
      if (isBig) {
        number = BIG_FIVE;
      }
      else {
        number = LITTLE_FIVE;
      }
      break;
    case 6:
      if (isBig) {
        number = BIG_SIX;
      }
      else {
        number = LITTLE_SIX;
      }
      break;
    case 7:
      if (isBig) {
        number = BIG_SEVEN;
      }
      else {
        number = LITTLE_SEVEN;
      }
      break;
    case 8:
      if (isBig) {
        number = BIG_EIGHT;
      }
      else {
        number = LITTLE_EIGHT;
      }
      break;
    case 9:
      if (isBig) {
        number = BIG_NINE;
      }
      else {
        number = LITTLE_NINE;
      }
      break;
  }

}
//rebiba digito a pintar(parametro)
//booleano is big(parametro)

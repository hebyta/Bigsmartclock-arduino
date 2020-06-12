//Librerías
#include <Adafruit_NeoPixel.h>

/**************
   CONSTANTES
 **************/
// Constantes de los pines
const byte MODE_BUTTON_PIN = 34;
const byte START_STOP_BUTTON_PIN = 36;
const byte UP_BUTTON_PIN = 38;
const byte DOWN_BUTTON_PIN = 40;
const byte CONFIG_BUTTON_PIN = 32;
const byte BUZZER_PIN = 46;
const byte HOURS_DISPLAY_PIN = 3;
const byte MINUTES_DISPLAY_PIN = 5;
const byte SECONDS_DISPLAY_PIN = 7;
const byte TENTHS_DISPLAY_PIN = 9;
const byte POINTS_DISPLAY_PIN = 11;
const byte MODE_DISPLAY_PIN = 13;

// Constantes de tiempo
const int ON_OFF_DISPLAY_MILLIS = 50;
const int SECOND_ON_MILLIS = 1000;
const int MAX_BUTTON_PRESSED_MILLIS = 3000;
const unsigned int MAX_TIME_ON_CONFIG = 60000;
const unsigned int MAX_TIME_WITHOUT_RECEIVE = 10000;
const int BUTTON_PRESSED_INTERVAL_MILLIS = 150;
const byte INITIAL_CLOCK_SECONDS = 0;
const byte INITIAL_CLOCK_MINUTES = 0;
const byte INITIAL_CLOCK_HOURS = 12;
const int SENDING_DATA_INTERVAL = 500;
const byte TIME_OFFSET_MILLIS = 2;
const byte TIME_OFFSET_MODULE = 4;

// Constantes de modo
const byte CLOCK_MODE = 0;
const byte CHRONO_MODE = 1;
const byte COUNTDOWN_MODE = 2;
const byte AUTO_CHRONO_MODE = 3;
const byte AUTO_COUNTDOWN_MODE = 4;

// Constantes de los para los displays
const byte HOURS_DISPLAY = 0;
const byte MINUTES_DISPLAY = 1;
const byte SECONDS_DISPLAY = 2;
const byte TENTHS_DISPLAY = 3;
const byte POINTS_DISPLAY = 4;
String bufferData = "";

// Constantes para dibujar los números  (No se crean como "const" por un problema en la conversión en el método getValueFormat() pero a efectos prácticos se tratan como constantes)
byte ONE[] = {0, 1, 1, 0, 0, 0, 0};
byte TWO[] = {1, 1, 0, 1, 1, 0, 1};
byte THREE[] = {1, 1, 1, 1, 0, 0, 1};
byte FOUR[] = {0, 1, 1, 0, 0, 1, 1};
byte FIVE[] = {1, 0, 1, 1, 0, 1, 1};
byte SIX[] = {0, 0, 1, 1, 1, 1, 1};
byte SEVEN[] = {1, 1, 1, 0, 0, 0, 0};
byte EIGHT[] = {1, 1, 1, 1, 1, 1, 1};
byte NINE[] = {1, 1, 1, 0, 0, 1, 1};
byte ZERO[] = {1, 1, 1, 1, 1, 1, 0};
byte LETTER_P[] = {1, 1, 0, 0, 1, 1, 1};
byte LETTER_A[] = {1, 1, 1, 0, 1, 1, 1};
byte LETTER_L[] = {0, 0, 0, 1, 1, 1, 0};
byte LETTER_F[] = {1, 0, 0, 0, 1, 1, 1};

/*************
   VARIABLES
 *************/
// Variables del reloj
volatile int clockMillis;
int clockSeconds;
int clockMinutes;
int clockHours;

// Variables del cronómetro
volatile int chronoMillis;
int chronoSeconds;
int chronoMinutes;
int chronoHours;

// Variables de la cuenta atrás
volatile int countDownMillis;
int countDownSeconds;
int countDownMinutes;
int countDownHours;
int countDownInitialSeconds;
int countDownInitialMinutes;
int countDownInitialHours;

// Variables del cronómetro con autocarga
volatile int autoChronoMillis;
int autoChronoSeconds;
int autoChronoMinutes;
int autoChronoHours;
int autoChronoHoursLimit;
int autoChronoMinutesLimit;
int autoChronoSecondsLimit;
int autoChronoMaxLoops;
int autoChronoCurrentLoop;

// Variables de la cuenta atrás  con autocarga
volatile int autoCountDownMillis;
int autoCountDownSeconds;
int autoCountDownMinutes;
int autoCountDownHours;
int autoCountDownInitialSeconds;
int autoCountDownInitialMinutes;
int autoCountDownInitialHours;
int autoCountDownMaxLoops;
int autoCountDownCurrentLoop;

// Variables de tiempo para los botones
int millisModePressed;
int millisStartStopPressed;
int millisUpPressed;
int millisDownPressed;
int millisBluetoothPressed;
int millisConfigPressed;
int elapsedTimeModeReaded;
int elapsedTimeStartStopReaded;
int elapsedTimeUpReaded;
int elapsedTimeDownReaded;
int elapsedTimeBluetoothReaded;
int elapsedTimeConfigReaded;
unsigned int elapsedTimeOnConfigMode;

// Variables del estado de los botones
byte modeButtonState = LOW;
byte startStopButtonState = LOW;
byte upButtonState = LOW;
byte downButtonState = LOW;
byte bluetoothButtonState = LOW;
byte configButtonState = LOW;

// Variables de estado
byte selectedMode;
byte selectedDisplay;
boolean configMode;
boolean drawHours = true;
boolean drawMinutes = true;
boolean drawSeconds = true;
boolean isChronoRunning;
boolean isCountDownRunning;
boolean isAutoChronoRunning;
boolean isAutoCountDownRunning;
boolean sendData = true;
boolean drawLoops = false;
boolean is24HoursClock = true;
boolean configSaved = false;
unsigned int sendingDataElapsedTime = 0;
unsigned int receivedDataElapsedTime = 0;

// Variables para los displays
byte redColor = 0;
byte greenColor = 244;
byte blueColor = 244;
byte ledBrigthness = 20;
int hoursSegmentSize = 6;
int minutesSegmentSize = 6;
int secondsSegmentSize = 6;
int tenthsSegmentSize = 3;
Adafruit_NeoPixel hoursDisplay(hoursSegmentSize * 7 * 2, HOURS_DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel minutesDisplay(minutesSegmentSize * 7 * 2, MINUTES_DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel secondsDisplay(secondsSegmentSize * 7 * 2, SECONDS_DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel tenthsDisplay(tenthsSegmentSize * 7, TENTHS_DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pointsDisplay(4, POINTS_DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel modeDisplay(5, MODE_DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
//SoftwareSerial bluetooth(BLUETOOH_RX_PIN, BLUETOOH_TX_PIN);
/***********
   MÉTODOS
 ***********/

/**
   Método que se ejecuta al iniciar el dispositivo y se utiliza para establecer la configuración inicial.
*/
void setup() {
  Serial1.begin(9600);
  Serial.begin(57600);
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  activeInterrupt();
  setupPins();
  initValues();
  initLeds();
  //bluetooth.begin(9600)
  drawLedMode();
  drawSeconds = true;
  drawMinutes = true;
  drawHours = true;
  drawClock();
  sendData = true;
}

/**
   Método ejecutado automáticamente por arduino tras ejecutar el setup y repetida e indefinidamente
*/
void loop() {
  readButtons();
  readBluetoothData();
  processBuffer();
  calculateTime();
  draw();
  sendBluetoothData();
}

/**
   Método para activar las interrupciones del timer0
*/
void activeInterrupt() {
  TCCR0A = (1 << WGM01); //Set the CTC mode
  OCR0A = 0xF9; //Value for ORC0A for 1ms

  TIMSK0 |= (1 << OCIE0A); //Set the interrupt request
  sei(); //Enable interrupt

  TCCR0B |= (1 << CS01); //Set the prescale 1/64 clock
  TCCR0B |= (1 << CS00);
}

/**
    Método para establecer la configuración inicial de los pines del dispositivo
*/
void setupPins() {
  pinMode(MODE_BUTTON_PIN, INPUT);
  pinMode(START_STOP_BUTTON_PIN, INPUT);
  pinMode(UP_BUTTON_PIN, INPUT);
  pinMode(DOWN_BUTTON_PIN, INPUT);
  pinMode(CONFIG_BUTTON_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(HOURS_DISPLAY_PIN, OUTPUT);
  pinMode(MINUTES_DISPLAY_PIN, OUTPUT);
  pinMode(SECONDS_DISPLAY_PIN, OUTPUT);
  pinMode(TENTHS_DISPLAY_PIN, OUTPUT);
  pinMode(POINTS_DISPLAY_PIN, OUTPUT);
  pinMode(MODE_DISPLAY_PIN, OUTPUT);
}

/**
    Método para establecer los valores iniciales de las variables
*/
void initValues() {
  selectedMode = CLOCK_MODE;
  isChronoRunning = false;
  isCountDownRunning = false;
  isAutoChronoRunning = false;
  isAutoCountDownRunning = false;
  configMode = false;
  clockMillis = 0;
  clockSeconds = INITIAL_CLOCK_SECONDS;
  clockMinutes = INITIAL_CLOCK_MINUTES;
  clockHours = INITIAL_CLOCK_HOURS;
  chronoMillis = 0;
  chronoSeconds = 0;
  chronoMinutes = 0;
  chronoHours = 0;
  countDownInitialSeconds = 30;
  countDownInitialMinutes = 5;
  countDownInitialHours = 0;
  countDownMillis = 0;
  countDownSeconds = countDownInitialSeconds;
  countDownMinutes = countDownInitialMinutes;
  countDownHours = countDownInitialHours;
  millisModePressed = 0;
  millisStartStopPressed = 0;
  millisUpPressed  = 0;
  millisDownPressed = 0;
  millisBluetoothPressed = 0;
  millisConfigPressed = 0;
  autoChronoMillis = 0;
  autoChronoSeconds = 0;
  autoChronoMinutes = 0;
  autoChronoHours = 0;
  autoChronoHoursLimit = 0;
  autoChronoMinutesLimit = 0;
  autoChronoSecondsLimit = 30;
  autoChronoMaxLoops = 10;
  autoChronoCurrentLoop = 1;
  autoCountDownInitialSeconds = 30;
  autoCountDownInitialMinutes = 0;
  autoCountDownInitialHours = 0;
  autoCountDownMillis = 0;
  autoCountDownSeconds = autoCountDownInitialSeconds;
  autoCountDownMinutes = autoCountDownInitialMinutes;
  autoCountDownHours = autoCountDownInitialHours;
  autoCountDownMaxLoops = 10;
  autoCountDownCurrentLoop = 1;
  elapsedTimeModeReaded = 0;
  elapsedTimeStartStopReaded = 0;
  elapsedTimeUpReaded = 0;
  elapsedTimeDownReaded = 0;
  elapsedTimeBluetoothReaded = 0;
  elapsedTimeConfigReaded = 0;
  elapsedTimeOnConfigMode = 0;
  selectedDisplay = HOURS_DISPLAY;
  drawSeconds = true;
  drawHours = true;
  drawMinutes = true;
}

void initLeds() {
  hoursDisplay.begin();
  hoursDisplay.show();
  minutesDisplay.begin();
  minutesDisplay.show();
  secondsDisplay.begin();
  secondsDisplay.show();
  tenthsDisplay.begin();
  tenthsDisplay.show();
  pointsDisplay.begin();
  pointsDisplay.show();
  modeDisplay.begin();
  modeDisplay.show();
}

void changeDisplayLedNumber(int displayCode, int nLeds) {
  switch (displayCode) {
    case HOURS_DISPLAY:
      hoursDisplay.updateLength(nLeds);
      break;
    case MINUTES_DISPLAY:
      minutesDisplay.updateLength(nLeds);
      break;
    case SECONDS_DISPLAY:
      secondsDisplay.updateLength(nLeds);
      break;
    case TENTHS_DISPLAY:
      tenthsDisplay.updateLength(nLeds);
      break;
  }
}

/**
   Método que se ejecuta cada milisegundo. Es activado automáticamente por las interrupciones del timer0
*/
ISR(TIMER0_COMPA_vect) {
  clockMillis++;
  if (isChronoRunning)
    chronoMillis++;
  if (isCountDownRunning)
    countDownMillis--;
  if (isAutoChronoRunning)
    autoChronoMillis++;
  if (isAutoCountDownRunning)
    autoCountDownMillis--;
  if (startStopButtonState == HIGH)
    millisStartStopPressed++;
  if (elapsedTimeModeReaded <= BUTTON_PRESSED_INTERVAL_MILLIS)
    elapsedTimeModeReaded++;
  if (elapsedTimeStartStopReaded <= BUTTON_PRESSED_INTERVAL_MILLIS)
    elapsedTimeStartStopReaded++;
  if (elapsedTimeUpReaded <= BUTTON_PRESSED_INTERVAL_MILLIS)
    elapsedTimeUpReaded++;
  if (elapsedTimeDownReaded <= BUTTON_PRESSED_INTERVAL_MILLIS)
    elapsedTimeDownReaded++;
  if (elapsedTimeConfigReaded <= BUTTON_PRESSED_INTERVAL_MILLIS)
    elapsedTimeConfigReaded++;
  if (sendData && sendingDataElapsedTime <= SENDING_DATA_INTERVAL)
    sendingDataElapsedTime++;
  if (configMode) {
    elapsedTimeOnConfigMode++;
  }
}

/**
   Método para leer todos los botones del dispositivo y ejecutar la lógica correspondiente a cada botón
*/
void readButtons() {
  readModeButton();
  readStartStopButton();
  readUpButton();
  readDownButton();
  readConfigButton();
}

/**
   Método para leer el botón de modo y actuar en consecuencia
*/
void readModeButton() {
  if (elapsedTimeModeReaded <= BUTTON_PRESSED_INTERVAL_MILLIS)
    return;
  byte state = digitalRead(MODE_BUTTON_PIN);
  if (state !=  modeButtonState && state == HIGH) {
    elapsedTimeOnConfigMode = 0;
    drawAll();
    if (!configMode) {
      changeMode(selectedMode, true);
      //Serial.println("Mode => " + String(selectedMode));
    } else {
      selectedDisplay++;
      //Serial.println("Selected Display => " + String(selectedDisplay));
      if (selectedMode == AUTO_COUNTDOWN_MODE || selectedMode == AUTO_CHRONO_MODE || selectedMode == CLOCK_MODE) {
        if (!drawLoops && selectedDisplay == TENTHS_DISPLAY) {
          turnOnOff(MINUTES_DISPLAY, false);
          turnOnOff(TENTHS_DISPLAY, false);
          turnOnOff(POINTS_DISPLAY, false);
          drawLoops = true;
          drawSeconds = true;
          drawHours = true;
          selectedDisplay = SECONDS_DISPLAY;
        } else if (drawLoops) {
          turnOnOff(MINUTES_DISPLAY, true);
          drawAll();
          drawLoops = false;
          selectedDisplay = HOURS_DISPLAY;
        }
      } else if (selectedDisplay == TENTHS_DISPLAY) {
        drawAll();
        selectedDisplay = HOURS_DISPLAY;
      }
    }
    elapsedTimeModeReaded = 0;
  } else if (state !=  modeButtonState && state == LOW) {
    drawAll();
    elapsedTimeModeReaded = 0;
  }
  modeButtonState = state;
}

/**
   Método para leer el boton Start/Stop y ejecutar las intrucciones necesarias en función del modo en el que esté el reloj
*/
void readStartStopButton() {
  if (elapsedTimeStartStopReaded <= BUTTON_PRESSED_INTERVAL_MILLIS)
    return;
  byte state = digitalRead(START_STOP_BUTTON_PIN);
  if (state != startStopButtonState && state == HIGH) {
    drawAll();
    drawLoops = false;
    elapsedTimeOnConfigMode = 0;
    if (configMode) {
      saveConfig();
      //Serial.println("configMode => " + String(configMode));
    }
    else {
      elapsedTimeStartStopReaded = 0;
      switch (selectedMode) {
        case CHRONO_MODE:
          isChronoRunning = !isChronoRunning;
          if (isChronoRunning && chronoHours == 99 && chronoMinutes == 59 && chronoSeconds == 59 && chronoMillis == 999) {
            initChrono();
          }
          //Serial.println("isChronoRunning => " + String(isChronoRunning));
          break;
        case COUNTDOWN_MODE:
          isCountDownRunning  = !isCountDownRunning;
          if (isCountDownRunning && countDownSeconds <= 0 && countDownMinutes <= 0  && countDownHours <= 0 && countDownMillis <= 0) {
            initCountDown();
          }
          //Serial.println("isCountDownRunning => " + String(isCountDownRunning));
          break;
        case AUTO_CHRONO_MODE:
          isAutoChronoRunning = !isAutoChronoRunning;
          if (isAutoChronoRunning && autoChronoSeconds >= autoChronoSecondsLimit && autoChronoMinutes >= autoChronoMinutesLimit && autoChronoHours >= autoChronoHoursLimit && autoChronoCurrentLoop == autoChronoMaxLoops) {
            initAutoChrono(true);
          }
          //Serial.println("isAutoChronoRunning => " + String(isAutoChronoRunning));
          break;
        case AUTO_COUNTDOWN_MODE:
          isAutoCountDownRunning = !isAutoCountDownRunning;
          if (isAutoCountDownRunning && autoCountDownSeconds <= 0 && autoCountDownMinutes <= 0  && autoCountDownHours <= 0 && autoCountDownMillis <= 0 && autoCountDownCurrentLoop == autoCountDownMaxLoops) {
            initAutoCountDown(true);
          }
          // Serial.println("isAutoCountDownRunning => " + String(isAutoCountDownRunning));
          break;
      }
    }
  } else if (state != startStopButtonState && state == LOW) {
    drawAll();
    elapsedTimeStartStopReaded = 0;
    millisStartStopPressed = 0;
  }
  startStopButtonState = state;
}

/**
   Método para leer el botón Up y ejecutar las instrucciones necesarias para su funcionamiento en función del modo
*/
void readUpButton() {
  if (elapsedTimeUpReaded <= BUTTON_PRESSED_INTERVAL_MILLIS)
    return;
  byte state = digitalRead(UP_BUTTON_PIN);
  if (state == HIGH) {
    //Serial.println("Up Buttonn");
    elapsedTimeUpReaded = 0;
    elapsedTimeOnConfigMode = 0;
    if (configMode) {
      switch (selectedMode) {
        case CLOCK_MODE:
          if (selectedDisplay == HOURS_DISPLAY) {
            drawHours = true;
            clockHours++;
            if (clockHours == 24)
              clockHours = 0;
          }
          if (selectedDisplay == MINUTES_DISPLAY) {
            drawMinutes = true;
            clockMinutes++;
            if (clockMinutes == 60)
              clockMinutes = 0;
          }
          if (selectedDisplay == SECONDS_DISPLAY) {
            drawSeconds = true;
            if (!drawLoops) {
              clockSeconds++;
              if (clockSeconds == 60)
                clockSeconds = 0;
            } else {
              is24HoursClock = !is24HoursClock;
            }
          }
          break;
        case COUNTDOWN_MODE:
          if (selectedDisplay == HOURS_DISPLAY) {
            drawHours = true;
            countDownInitialHours++;
            if (countDownInitialHours == 24)
              countDownHours = 0;
          }
          if (selectedDisplay == MINUTES_DISPLAY) {
            drawMinutes = true;
            countDownInitialMinutes++;
            if (countDownInitialMinutes == 60)
              countDownInitialMinutes = 0;
          }
          if (selectedDisplay == SECONDS_DISPLAY) {
            drawSeconds = true;
            countDownInitialSeconds++;
            if (countDownInitialSeconds == 60)
              countDownInitialSeconds = 0;
          }
          break;
        case AUTO_CHRONO_MODE:
          if (selectedDisplay == HOURS_DISPLAY) {
            drawHours = true;
            autoChronoHoursLimit++;
            if (autoChronoHoursLimit == 24)
              autoChronoHoursLimit = 0;
          }
          if (selectedDisplay == MINUTES_DISPLAY) {
            drawMinutes = true;
            autoChronoMinutesLimit++;
            if (autoChronoMinutesLimit == 60)
              autoChronoMinutesLimit = 0;
          }
          if (selectedDisplay == SECONDS_DISPLAY) {
            drawSeconds = true;
            if (!drawLoops) {
              autoChronoSecondsLimit++;
              if (autoChronoSecondsLimit == 60)
                autoChronoSecondsLimit = 0;
            } else {
              autoChronoMaxLoops++;
              if (autoChronoMaxLoops > 99)
                autoChronoMaxLoops = 0;
            }
          }
          break;
        case AUTO_COUNTDOWN_MODE:
          if (selectedDisplay == HOURS_DISPLAY) {
            drawHours = true;
            autoCountDownInitialHours++;
            if (autoCountDownInitialHours == 24)
              autoCountDownInitialHours = 0;
          }
          if (selectedDisplay == MINUTES_DISPLAY) {
            drawMinutes = true;
            autoCountDownInitialMinutes++;
            if (autoCountDownInitialMinutes == 60)
              autoCountDownInitialMinutes = 0;
          }
          if (selectedDisplay == SECONDS_DISPLAY) {
            drawSeconds = true;
            autoCountDownInitialSeconds++;
            if (autoCountDownInitialSeconds == 60)
              autoCountDownInitialSeconds = 0;
            if (!drawLoops) {
              autoChronoSecondsLimit++;
              if (autoChronoSecondsLimit == 60)
                autoChronoSecondsLimit = 0;
            } else {
              autoCountDownMaxLoops++;
              if (autoCountDownMaxLoops > 99)
                autoCountDownMaxLoops = 0;
            }
          }
          break;
      }
    }
  } else {
    elapsedTimeUpReaded = 0;
  }
  upButtonState = state;
}

/**
   Método para leer el botón Down y ejecutar las instrucciones necesarias para su funcionamiento en funcion del modo
*/
void readDownButton() {
  if (elapsedTimeDownReaded <= BUTTON_PRESSED_INTERVAL_MILLIS)
    return;
  byte state = digitalRead(DOWN_BUTTON_PIN);
  if (state == HIGH) {
    //Serial.println("Down Button");
    elapsedTimeDownReaded = 0;
    elapsedTimeOnConfigMode = 0;
    if (configMode) {
      switch (selectedMode) {
        case CLOCK_MODE:
          if (selectedDisplay == HOURS_DISPLAY) {
            drawHours = true;
            clockHours--;
            if (clockHours == -1)
              clockHours = 23;
          }
          if (selectedDisplay == MINUTES_DISPLAY) {
            drawMinutes = true;
            clockMinutes--;
            if (clockMinutes == -1)
              clockMinutes = 59;
          }
          if (selectedDisplay == SECONDS_DISPLAY) {
            drawSeconds = true;
            if (!drawLoops) {
              clockSeconds--;
              if (clockSeconds == -1)
                clockSeconds = 59;
            } else {
              is24HoursClock = !is24HoursClock;
            }
          }
          break;
        case COUNTDOWN_MODE:
          if (selectedDisplay == HOURS_DISPLAY) {
            drawHours = true;
            countDownInitialHours--;
            if (countDownInitialHours == -1)
              countDownInitialHours = 23;
          }
          if (selectedDisplay == MINUTES_DISPLAY) {
            drawMinutes = true;
            countDownInitialMinutes--;
            if (countDownInitialMinutes == -1)
              countDownInitialMinutes = 59;
          }
          if (selectedDisplay == SECONDS_DISPLAY) {
            drawSeconds = true;
            countDownInitialSeconds--;
            if (countDownInitialSeconds == -4)
              countDownInitialSeconds = 59;
          }
          break;
        case AUTO_CHRONO_MODE:
          if (selectedDisplay == HOURS_DISPLAY) {
            drawHours = true;
            autoChronoHoursLimit--;
            if (autoChronoHoursLimit == -1)
              autoChronoHoursLimit = 23;
          }
          if (selectedDisplay == MINUTES_DISPLAY) {
            drawMinutes = true;
            autoChronoMinutesLimit--;
            if (autoChronoMinutesLimit == -1)
              autoChronoMinutesLimit = 59;
          }
          if (selectedDisplay == SECONDS_DISPLAY) {
            drawSeconds = true;
            if (!drawLoops) {
              autoChronoSecondsLimit--;
              if (autoChronoSecondsLimit == -1)
                autoChronoSecondsLimit = 59;
            } else {
              autoChronoMaxLoops--;
              if (autoChronoMaxLoops < 0)
                autoChronoMaxLoops = 99;
            }
          }
          break;
        case AUTO_COUNTDOWN_MODE:
          if (selectedDisplay == HOURS_DISPLAY) {
            drawHours = true;
            autoCountDownHours--;
            if (autoCountDownHours == -1)
              autoCountDownHours = 23;
          }
          if (selectedDisplay == MINUTES_DISPLAY) {
            drawMinutes = true;
            autoCountDownMinutes++;
            if (autoCountDownMinutes == -1)
              autoCountDownMinutes = 59;
          }
          if (selectedDisplay == SECONDS_DISPLAY) {
            drawSeconds = true;
            if (!drawLoops) {
              autoCountDownInitialSeconds++;
              if (autoCountDownInitialSeconds == -1)
                autoCountDownInitialSeconds = 59;
            } else {
              autoCountDownMaxLoops--;
              if (autoCountDownMaxLoops < 0)
                autoCountDownMaxLoops = 99;
            }
          }
          break;
      }
    }
  } else {
    elapsedTimeDownReaded = 0;
  }
  downButtonState = state;
}

/**
   Método para leer el botón de configuracion y ejecutar la lógica necesaria para su funcinamiento
*/
void readConfigButton() {
  if (elapsedTimeConfigReaded <= BUTTON_PRESSED_INTERVAL_MILLIS)
    return;
  if (selectedMode == CHRONO_MODE)
    return;
  byte state = digitalRead(CONFIG_BUTTON_PIN);
  if (state != configButtonState && state == HIGH) {
    if (configMode)
      saveConfig();
    else {
      selectedDisplay = HOURS_DISPLAY;
      configMode = true;
    }
    elapsedTimeConfigReaded = 0;
    drawAll();
  } else if(state != configButtonState && state == LOW){
    drawAll();
    elapsedTimeConfigReaded = 0;
  }
  configButtonState = state;
}

/**
   Método para calcular todos los tiempos medidos (Reloj, cronómetro, cuenta atras...)
*/
void calculateTime() {
  calculateClock();
  calculateChrono();
  calculateCowntdown();
  calculateAutoChrono();
  calculateAutoCowntdown();
  checkOnConfigModeTime();
  checkButtonsPressed();
}

/**
   Método para calcular el tiempo del reloj
*/
void calculateClock() {
  if (clockMillis >= SECOND_ON_MILLIS) {
    drawSeconds = true;
    if (clockSeconds == 0 || clockSeconds % TIME_OFFSET_MODULE == 0)
      clockMillis = clockMillis - SECOND_ON_MILLIS + TIME_OFFSET_MILLIS + 1;
    else
      clockMillis = clockMillis - SECOND_ON_MILLIS + TIME_OFFSET_MILLIS;
    clockSeconds++;
  }
  if (clockSeconds >= 60) {
    drawMinutes = true;
    clockMinutes++;
    clockSeconds = 0;
  }
  if (clockMinutes >= 60) {
    drawHours = true;
    clockHours++;
    clockMinutes = 0;
  }
  if (clockHours >= 24) {
    drawHours = true;
    clockHours = 0;
  }
}

/**
   Método para calcular el paso del tiempo en el cronómetro y resetearlo cuando sea necesario
*/
void calculateChrono() {
  if (isChronoRunning) {
    if (chronoMillis >= SECOND_ON_MILLIS) {
      drawSeconds = true;
      if (chronoSeconds == 0 || chronoSeconds % TIME_OFFSET_MODULE == 0)
        chronoMillis = chronoMillis - SECOND_ON_MILLIS + TIME_OFFSET_MILLIS + 1;
      else
        chronoMillis = chronoMillis - SECOND_ON_MILLIS + TIME_OFFSET_MILLIS;
      chronoSeconds++;
    }
    if (chronoSeconds >= 60) {
      drawMinutes = true;
      chronoMinutes++;
      chronoSeconds = 0;
    }
    if (chronoMinutes >= 60) {
      drawHours = true;
      chronoHours++;
      chronoMinutes = 0;
    }
    if (chronoHours == 100) {
      drawAll();
      chronoHours = 99;
      chronoSeconds = 59;
      chronoMinutes = 59;
      chronoMillis = 999;
      isChronoRunning = false;
    }
  }
}

/**
   Método para reiniciar la cuenta atrás
*/
void initChrono() {
  chronoMillis = 0;
  chronoSeconds = 0;
  chronoMinutes = 0;
  chronoHours = 0;
  drawAll();
}

/**
   Método para calcular el paso del tiempo durante la cuenta atrás y resetearla cuando sea necesario
*/
void calculateCowntdown() {
  if (isCountDownRunning) {
    if (countDownMillis <= 0 && (countDownSeconds > 0 || countDownMinutes > 0 || countDownHours > 0)) {
      if (countDownSeconds == 0 || countDownSeconds % TIME_OFFSET_MODULE == 0)
        countDownMillis = SECOND_ON_MILLIS - TIME_OFFSET_MILLIS - 1;
      else
        countDownMillis = SECOND_ON_MILLIS - TIME_OFFSET_MILLIS;
      countDownSeconds--;
      drawSeconds = true;
    }
    if (countDownSeconds < 0 && (countDownMinutes > 0 || countDownHours > 0)) {
      countDownSeconds = 59;
      countDownMinutes--;
      drawMinutes = true;
    }
    if (countDownMinutes < 0  && countDownHours > 0) {
      drawHours = true;
      countDownHours--;
      countDownMinutes = 59;
    }
    if (countDownHours < 0) {
      drawHours = true;
      countDownHours = 0;
    }
    if (countDownMillis <= 0 && countDownSeconds <= 0 && countDownMinutes <= 0 && countDownHours <= 0) {
      isCountDownRunning = false;
      drawAll();
      countDownMillis = 0;
      countDownHours = 0;
      countDownMinutes = 0;
      countDownSeconds = 0;
      tone(BUZZER_PIN, 300, 1000);
    }
  }
}

/**
   Método para reiniciar la cuenta atras
*/
void initCountDown() {
  countDownMillis = 0;
  countDownHours = countDownInitialHours;
  countDownMinutes = countDownInitialMinutes;
  countDownSeconds = countDownInitialSeconds;
  drawAll();
}


/**
   Método para calcular el paso del tiempo en el cronómetro con autocarga y resetearlo cuando sea necesario
*/
void calculateAutoChrono() {
  if (isAutoChronoRunning) {
    if (autoChronoMillis >= SECOND_ON_MILLIS) {
      drawSeconds = true;
      if (autoChronoSeconds == 0 || autoChronoSeconds % TIME_OFFSET_MODULE == 0)
        autoChronoMillis = autoChronoMillis - SECOND_ON_MILLIS + TIME_OFFSET_MILLIS + 1;
      else
        autoChronoMillis = autoChronoMillis - SECOND_ON_MILLIS + TIME_OFFSET_MILLIS;
      autoChronoSeconds++;
    }
    if (autoChronoSeconds >= 60) {
      drawMinutes = true;
      autoChronoMinutes++;
      autoChronoSeconds = 0;
    }
    if (autoChronoMinutes >= 60) {
      drawHours = true;
      autoChronoHours++;
      autoChronoMinutes = 0;
    }
    if ((autoChronoHours == 100) || (autoChronoSeconds >= autoChronoSecondsLimit && autoChronoMinutes >= autoChronoMinutesLimit && autoChronoHours >= autoChronoHoursLimit)) {
      drawAll();
      autoChronoCurrentLoop++;
      if (autoChronoCurrentLoop > autoChronoMaxLoops) {
        autoChronoCurrentLoop = autoChronoMaxLoops;
        isAutoChronoRunning = false;
        if (chronoHours == 100) {
          autoChronoHours = 99;
          autoChronoSeconds = 59;
          autoChronoMinutes = 59;
          autoChronoMillis = 999;
        } else {
          initAutoChrono(false);
        }
        tone(BUZZER_PIN, 300, 1000);
      } else {
        initAutoChrono(false);
        tone(BUZZER_PIN, 500, 500);
      }
    }
  }
}

/**
   Método para reiniciar el cronómetro con auto carga
*/
void initAutoChrono(boolean resetCounter) {
  autoChronoHours = 0;
  autoChronoMinutes = 0;
  autoChronoSeconds = 0;
  autoChronoMillis = 0;
  if (resetCounter)
    autoChronoCurrentLoop = 1;
  drawAll();
}

/**
   Método para calcular el paso del tiempo durante la cuenta atrás con auto carga y resetearla cuando sea necesario
*/
void calculateAutoCowntdown() {
  if (isAutoCountDownRunning) {
    if (autoCountDownMillis <= 0 && (autoCountDownSeconds > 0 || autoCountDownMinutes > 0 || autoCountDownHours > 0)) {
      if (autoCountDownSeconds == 0 || autoCountDownSeconds % TIME_OFFSET_MODULE == 0)
        autoCountDownMillis = SECOND_ON_MILLIS - TIME_OFFSET_MILLIS - 1;
      else
        autoCountDownMillis = SECOND_ON_MILLIS - TIME_OFFSET_MILLIS;
      autoCountDownSeconds--;
      drawSeconds = true;
    }
    if (autoCountDownSeconds < 0 && (autoCountDownMinutes > 0 || autoCountDownHours > 0)) {
      autoCountDownSeconds = 59;
      autoCountDownMinutes--;
      drawMinutes = true;
    }
    if (autoCountDownMinutes < 0  && autoCountDownHours > 0) {
      drawHours = true;
      autoCountDownHours--;
      autoCountDownMinutes = 59;
    }
    if (autoCountDownHours < 0) {
      drawHours = true;
      autoCountDownHours = 0;
    }
    if (autoCountDownMillis <= 0 && autoCountDownSeconds <= 0 && autoCountDownMinutes <= 0 && autoCountDownHours <= 0) {
      drawAll();
      autoCountDownCurrentLoop++;
      if (autoCountDownCurrentLoop > autoCountDownMaxLoops) {
        autoCountDownCurrentLoop = autoCountDownMaxLoops;
        autoCountDownMillis = 0;
        autoCountDownSeconds = 0;
        autoCountDownMinutes = 0;
        autoCountDownHours = 0;
        isAutoCountDownRunning = false;
        tone(BUZZER_PIN, 300, 1000);
      } else {
        initAutoCountDown(false);
        tone(BUZZER_PIN, 500, 500);
      }
    }
  }
}

void initAutoCountDown(boolean restartCounter) {
  autoCountDownMillis = 0;
  autoCountDownHours = autoCountDownInitialHours;
  autoCountDownMinutes = autoCountDownInitialMinutes;
  autoCountDownSeconds = autoCountDownInitialSeconds;
  if (restartCounter)
    autoCountDownCurrentLoop = 1;
  drawAll();
}

/**
   Método para controlar el tiempo máximo pulsado en los botones y actuar en consecuencia
*/
void checkButtonsPressed() {
  if (millisStartStopPressed >= MAX_BUTTON_PRESSED_MILLIS && selectedMode == COUNTDOWN_MODE) {
    isCountDownRunning = false;
    initCountDown();
  } else if (millisStartStopPressed >= MAX_BUTTON_PRESSED_MILLIS && selectedMode == CHRONO_MODE) {
    isChronoRunning = false;
    initChrono();
  } else if (millisStartStopPressed >= MAX_BUTTON_PRESSED_MILLIS && selectedMode == AUTO_CHRONO_MODE) {
    isAutoChronoRunning = false;
    initAutoChrono(true);
  } else if (millisStartStopPressed >= MAX_BUTTON_PRESSED_MILLIS && selectedMode == AUTO_COUNTDOWN_MODE) {
    isAutoChronoRunning = false;
    initAutoCountDown(true);
  }
}

/**
   Método para comprobar si el tiempo máximo permitido en el modo configuración ha sido superado y salir del modo configuracion
*/
void checkOnConfigModeTime() {
  if (configMode && elapsedTimeOnConfigMode >= MAX_TIME_ON_CONFIG) {
    saveConfig();
    drawAll();
    drawLoops = false;
    elapsedTimeOnConfigMode = 0;
  }
}

/**
   Método para dibujar cada estado del reloj cuando sea pertinente
*/
void draw() {
  if(configSaved && !drawMinutes){
    drawAll();
    drawLoops = false;
    configSaved = false;  
  }
  drawClock();
  drawChrono();
  drawCountDown();
  drawAutoChrono();
  drawAutoCountDown();
  turnOnOff(POINTS_DISPLAY, true);
  drawLedMode();
}

String getTwoCharFormat(int value) {
  String result = "";
  char firstDigit = '0';
  char secondDigit = String(value)[0];
  if (value > 9) {
    String valueStr = String(value);
    firstDigit = valueStr[0];
    secondDigit = valueStr[1];
  }
  result = result + firstDigit + secondDigit;
  return result;
}

/**
   Método que dibuja el reloj cuando sea necesario
*/
void drawClock() {
  if (selectedMode == CLOCK_MODE) {
    byte hours = clockHours;
    if (!is24HoursClock) {
      if (hours == 0)
        hours = 12;
      else if (hours > 12)
        hours = hours - 12;
    }
    if (drawLoops) {
      if (drawSeconds) {
        if (is24HoursClock)
          drawPixels(SECONDS_DISPLAY, '2', '4');
        else
          drawPixels(SECONDS_DISPLAY, '1', '2');
        drawSeconds = false;
      }
      if (drawHours) {
        drawPixels(HOURS_DISPLAY, 'F', ' ');
        drawHours = false;
      }
    } else {
      if (drawHours) {
        String hourStr = getTwoCharFormat(hours);
        drawPixels(HOURS_DISPLAY, hourStr[0], hourStr[1]);
        drawHours = false;
      }
      if (drawMinutes) {
        String minutesStr = getTwoCharFormat(clockMinutes);
        drawPixels(MINUTES_DISPLAY, minutesStr[0], minutesStr[1]);
        drawMinutes = false;
      }
      if (drawSeconds) {
        String secondsStr = getTwoCharFormat(clockSeconds);
        drawPixels(SECONDS_DISPLAY, secondsStr[0], secondsStr[1]);
        drawSeconds = false;
      }
      if (is24HoursClock)
        turnOnOff(TENTHS_DISPLAY, false);
      else {
        if (clockHours >= 12)
          drawPixels(TENTHS_DISPLAY, 'P', ' ');
        else
          drawPixels(TENTHS_DISPLAY, 'A', ' ');
      }
    }
  }
}

/**
   Método  que dibuja el cronometro cuando sea necesario
*/
void drawChrono() {
  if (selectedMode == CHRONO_MODE) {
    if (drawHours) {
      String hourStr = getTwoCharFormat(chronoHours);
      drawPixels(HOURS_DISPLAY, hourStr[0], hourStr[1]);
      drawHours = false;
    }
    if (drawMinutes) {
      String minutesStr = getTwoCharFormat(chronoMinutes);
      drawPixels(MINUTES_DISPLAY, minutesStr[0], minutesStr[1]);
      drawMinutes = false;
    }
    if (drawSeconds) {
      String secondsStr = getTwoCharFormat(chronoSeconds);
      drawPixels(SECONDS_DISPLAY, secondsStr[0], secondsStr[1]);
      drawSeconds = false;
    }
    if (chronoMillis >= 100)
      drawPixels(TENTHS_DISPLAY, String(chronoMillis)[0], ' ');
    else
      drawPixels(TENTHS_DISPLAY, '0', ' ');
  }
}

/**
   Método para dibujar la cuenta atras cuando sea necesario
*/
void drawCountDown() {
  if (selectedMode == COUNTDOWN_MODE) {
    int hours = countDownHours;
    int minutes = countDownMinutes;
    int seconds = countDownSeconds;
    if (configMode) {
      hours = countDownInitialHours;
      minutes = countDownInitialMinutes;
      seconds = countDownInitialSeconds;
    }
    if (drawHours) {
      String hourStr = getTwoCharFormat(hours);
      drawPixels(HOURS_DISPLAY, hourStr[0], hourStr[1]);
      drawHours = false;
    }
    if (drawMinutes) {
      String minutesStr = getTwoCharFormat(minutes);
      drawPixels(MINUTES_DISPLAY, minutesStr[0], minutesStr[1]);
      drawMinutes = false;
    }
    if (drawSeconds) {
      String secondsStr = getTwoCharFormat(seconds);
      drawPixels(SECONDS_DISPLAY, secondsStr[0], secondsStr[1]);
      drawSeconds = false;
    }
    if (countDownMillis >= 100)
      drawPixels(TENTHS_DISPLAY, String(countDownMillis)[0], ' ');
    else
      drawPixels(TENTHS_DISPLAY, '0', ' ');
  }
}

/**
   Método que dibuja el cronometro con auto carga cuando sea necesario
*/
void drawAutoChrono() {
  if (selectedMode == AUTO_CHRONO_MODE) {
    int hours = autoChronoHours;
    int minutes = autoChronoMinutes;
    int seconds = autoChronoSeconds;
    if (configMode) {
      hours = autoChronoHoursLimit;
      minutes = autoChronoMinutesLimit;
      seconds = autoChronoSecondsLimit;
    }
    if (drawLoops) {
      if (drawSeconds) {
        String maxLoopsStr = getTwoCharFormat(autoChronoMaxLoops);
        drawPixels(SECONDS_DISPLAY, maxLoopsStr[0], maxLoopsStr[1]);
        drawSeconds = false;
      }
      if (drawHours) {
        drawPixels(HOURS_DISPLAY, 'L', ' ');
        drawHours = false;
      }
    } else {
      if (drawHours) {
        String hourStr = getTwoCharFormat(hours);
        drawPixels(HOURS_DISPLAY, hourStr[0], hourStr[1]);
        drawHours = false;
      }
      if (drawMinutes) {
        String minutesStr = getTwoCharFormat(minutes);
        drawPixels(MINUTES_DISPLAY, minutesStr[0], minutesStr[1]);
        drawMinutes = false;
      }
      if (drawSeconds) {
        String secondsStr = getTwoCharFormat(seconds);
        drawPixels(SECONDS_DISPLAY, secondsStr[0], secondsStr[1]);
        drawSeconds = false;
      }
      if (autoChronoMillis >= 100)
        drawPixels(TENTHS_DISPLAY, String(autoChronoMillis)[0], ' ');
      else
        drawPixels(TENTHS_DISPLAY, '0', ' ');
    }
  }
}

/**
   Método para dibujar la cuenta atras con auto carga cuando sea necesario
*/
void drawAutoCountDown() {
  if (selectedMode == AUTO_COUNTDOWN_MODE) {
    int hours = autoCountDownHours;
    int minutes = autoCountDownMinutes;
    int seconds = autoCountDownSeconds;
    if (configMode) {
      hours = autoCountDownInitialHours;
      minutes = autoCountDownInitialMinutes;
      seconds = autoCountDownInitialSeconds;
    }
    if (drawLoops) {
      if (drawSeconds) {
        String maxLoopsStr = getTwoCharFormat(autoCountDownMaxLoops);
        drawPixels(SECONDS_DISPLAY, maxLoopsStr[0], maxLoopsStr[1]);
        drawSeconds = false;
      }
      if (drawHours) {
        drawPixels(HOURS_DISPLAY, 'L', ' ');
        drawHours = false;
      }
    } else {
      if (drawHours) {
        String hourStr = getTwoCharFormat(hours);
        drawPixels(HOURS_DISPLAY, hourStr[0], hourStr[1]);
        drawHours = false;
      }
      if (drawMinutes) {
        String minutesStr = getTwoCharFormat(minutes);
        drawPixels(MINUTES_DISPLAY, minutesStr[0], minutesStr[1]);
        drawMinutes = false;
      }
      if (drawSeconds) {
        String secondsStr = getTwoCharFormat(seconds);
        drawPixels(SECONDS_DISPLAY, secondsStr[0], secondsStr[1]);
        drawSeconds = false;
      }
      if (autoCountDownMillis >= 100)
        drawPixels(TENTHS_DISPLAY, String(autoCountDownMillis)[0], ' ');
      else
        drawPixels(TENTHS_DISPLAY, '0', ' ');
    }
  }
}

/**
   Método para dibujar los números y letras en los displays
*/
void drawPixels(int displayCode, char firstChar, char secondChar) {
  byte firstCharValueFormat[7];
  byte secondCharValueFormat[7];
  int segmentSize = getSegmentSize(displayCode);
  boolean simpleDisplay = isSimpleDisplay(displayCode);
  int simpleDisplayTotalLeds = segmentSize * 7;
  getValueFormat(firstChar, firstCharValueFormat);
  getValueFormat(secondChar, secondCharValueFormat);
  int segment = 0;
  if (displayCode == HOURS_DISPLAY) {
    hoursDisplay.clear();
  } else if (displayCode == MINUTES_DISPLAY) {
    minutesDisplay.clear();
  } else if (displayCode == SECONDS_DISPLAY) {
    secondsDisplay.clear();
  } else if (displayCode == TENTHS_DISPLAY) {
    tenthsDisplay.clear();
  }
  for (int i = 0; i < simpleDisplayTotalLeds; i++) {
    if (i % segmentSize == 0 && i != 0)
      segment++;
    byte firstDisplaySegmentState = firstCharValueFormat[segment];
    int rColor = redColor;
    int gColor = greenColor;
    int bColor = blueColor;
    if (firstDisplaySegmentState  == 0 || firstChar == ' ') {
      rColor = 0;
      gColor = 0;
      bColor = 0;
    } else if (configMode && displayCode == selectedDisplay) {
      if (rColor >= 127)
        rColor = 0;
      else
        rColor = 255;
      if (rColor == 0 && gColor == 0 && bColor == 0) {
        rColor = 255;
        gColor = 255;
        bColor = 255;
      }
    }
    if (displayCode == HOURS_DISPLAY)
      hoursDisplay.setPixelColor(i, hoursDisplay.Color(rColor, gColor, bColor));
    else if (displayCode == MINUTES_DISPLAY)
      minutesDisplay.setPixelColor(i, minutesDisplay.Color(rColor, gColor, bColor));
    else if (displayCode == SECONDS_DISPLAY)
      secondsDisplay.setPixelColor(i, secondsDisplay.Color(rColor, gColor, bColor));
    else if (displayCode == TENTHS_DISPLAY)
      tenthsDisplay.setPixelColor(i, tenthsDisplay.Color(rColor, gColor, bColor));
    if (!simpleDisplay) {
      byte secondDisplaySegmentState = secondCharValueFormat[segment];
      rColor = redColor;
      gColor = greenColor;
      bColor = blueColor;
      if (secondDisplaySegmentState  == 0 || secondChar == ' ') {
        rColor = 0;
        gColor = 0;
        bColor = 0;
      } else if (configMode && displayCode == selectedDisplay) {
        if (rColor >= 127)
          rColor = 0;
        else
          rColor = 255;
        if (rColor == 0 && gColor == 0 && bColor == 0) {
          rColor = 255;
          gColor = 255;
          bColor = 255;
        }
      }
      if (displayCode == HOURS_DISPLAY)
        hoursDisplay.setPixelColor(i + simpleDisplayTotalLeds, hoursDisplay.Color(rColor, gColor, bColor));
      else if (displayCode == MINUTES_DISPLAY)
        minutesDisplay.setPixelColor(i + simpleDisplayTotalLeds, minutesDisplay.Color(rColor, gColor, bColor));
      else if (displayCode == SECONDS_DISPLAY)
        secondsDisplay.setPixelColor(i + simpleDisplayTotalLeds, secondsDisplay.Color(rColor, gColor, bColor));
      else if (displayCode == TENTHS_DISPLAY)
        tenthsDisplay.setPixelColor(i + simpleDisplayTotalLeds, tenthsDisplay.Color(rColor, gColor, bColor));
    }
  }
  int brigtness = ledBrigthness;
  if (displayCode == HOURS_DISPLAY) {
    hoursDisplay.setBrightness(brigtness);
    hoursDisplay.show();
  } else if (displayCode == MINUTES_DISPLAY) {
    minutesDisplay.setBrightness(brigtness);
    minutesDisplay.show();
  } else if (displayCode == SECONDS_DISPLAY) {
    secondsDisplay.setBrightness(brigtness);
    secondsDisplay.show();
  } else if (displayCode == TENTHS_DISPLAY) {
    tenthsDisplay.setBrightness(brigtness);
    tenthsDisplay.show();
  }
}

/**
   Método para encender y apagar el display de las horas
   @param turnOn: true para encender el display, false para apagarlo
*/
void turnOnOff(int displayCode, boolean turnOn) {
  int nPixels = 0;
  if (displayCode == HOURS_DISPLAY)
    nPixels = hoursDisplay.numPixels();
  else if (displayCode == MINUTES_DISPLAY)
    nPixels = minutesDisplay.numPixels();
  else if (displayCode == SECONDS_DISPLAY)
    nPixels = secondsDisplay.numPixels();
  else if (displayCode == TENTHS_DISPLAY)
    nPixels = tenthsDisplay.numPixels();
  else if (displayCode == POINTS_DISPLAY)
    nPixels = pointsDisplay.numPixels();
  for (int i = 0; i < nPixels; i++) {
    int rColor = 0;
    int gColor = 0;
    int bColor = 0;
    if (turnOn) {
      rColor = redColor;
      gColor = greenColor;
      bColor = blueColor;
    }
    if (displayCode == HOURS_DISPLAY)
      hoursDisplay.setPixelColor(i, hoursDisplay.Color(rColor, gColor, bColor));
    else if (displayCode == MINUTES_DISPLAY)
      minutesDisplay.setPixelColor(i, minutesDisplay.Color(rColor, gColor, bColor));
    else if (displayCode == SECONDS_DISPLAY)
      secondsDisplay.setPixelColor(i, secondsDisplay.Color(rColor, gColor, bColor));
    else if (displayCode == TENTHS_DISPLAY)
      tenthsDisplay.setPixelColor(i, tenthsDisplay.Color(rColor, gColor, bColor));
    else if (displayCode == POINTS_DISPLAY)
      pointsDisplay.setPixelColor(i, pointsDisplay.Color(rColor, gColor, bColor));
  }
  int brightness = 0;
  if (turnOn)
    brightness = ledBrigthness;
  if (displayCode == HOURS_DISPLAY) {
    hoursDisplay.setBrightness(brightness);
    hoursDisplay.show();
  } else if (displayCode == MINUTES_DISPLAY) {
    minutesDisplay.setBrightness(brightness);
    minutesDisplay.show();
  } else if (displayCode == SECONDS_DISPLAY) {
    secondsDisplay.setBrightness(brightness);
    secondsDisplay.show();
  } else if (displayCode == TENTHS_DISPLAY) {
    tenthsDisplay.setBrightness(brightness);
    tenthsDisplay.show();
  } else if (displayCode == POINTS_DISPLAY) {
    pointsDisplay.setBrightness(brightness);
    pointsDisplay.show();
  }
}

void drawLedMode() {
  int rColor = 127;
  int gColor = 127;
  int bcolor = 127;
  switch (selectedMode) {
    case CLOCK_MODE:
      break;
    case CHRONO_MODE:
      break;
    case COUNTDOWN_MODE:
      break;
    case AUTO_CHRONO_MODE:
      break;
    case AUTO_COUNTDOWN_MODE:
      break;
  }
  for (unsigned int i = 0; i < modeDisplay.numPixels(); i++) {
    int redTmp = 0;
    int greenTmp = 0;
    int blueTmp = 0;
    if (i == selectedMode) {
      redTmp = rColor;
      greenTmp = gColor;
      blueTmp = bcolor;
    }
    modeDisplay.setPixelColor(i, modeDisplay.Color(redTmp, greenTmp, blueTmp));
  }
  modeDisplay.setBrightness(ledBrigthness);
  modeDisplay.show();
}

/**
   Método que recibe el número de leds de cada segmento de cada display individual en función  del código del display
   @param displayCode: Código de display para dibujar
   @return: Devuelve el número de leds de cada segmento para el display seleccionado
*/
byte getSegmentSize(byte displayCode) {
  int segmentSize = 0;
  switch (displayCode) {
    case HOURS_DISPLAY:
      segmentSize = hoursSegmentSize;
      break;
    case MINUTES_DISPLAY:
      segmentSize = minutesSegmentSize;
      break;
    case SECONDS_DISPLAY:
      segmentSize = secondsSegmentSize;
      break;
    case TENTHS_DISPLAY:
      segmentSize = tenthsSegmentSize;
      break;
  }
  return segmentSize;
}

/**
   Método que indica si se trata de un display simple o doble en función del código del display
   @displayCode: Código de display para dibujar
   @return: Devuelve true si se trata de un display simple o false en caso contrario
*/
byte isSimpleDisplay(byte displayCode) {
  boolean isSimple = false;
  switch (displayCode) {
    case TENTHS_DISPLAY:
      isSimple = true;
      break;
  }
  return isSimple;
}

/**
   Método que devuelve en targetValues el valor del formato a dibujar del valor recibido
   @param value: Valor entre 0 y 9 para obtener el formato
   @param targetValues: Parámetro por referencia que se usará como salida del método
*/
void getValueFormat(char value, byte targetValues[]) {
  if (value == '1') {
    copyArray(ONE, targetValues, 7);
  } else if (value == '2') {
    copyArray(TWO, targetValues, 7);
  } else if (value == '3') {
    copyArray(THREE, targetValues, 7);
  } else if (value == '4') {
    copyArray(FOUR, targetValues, 7);
  } else if (value == '5') {
    copyArray(FIVE, targetValues, 7);
  } else if (value == '6') {
    copyArray(SIX, targetValues, 7);
  } else if (value == '7') {
    copyArray(SEVEN, targetValues, 7);
  } else if (value == '8') {
    copyArray(EIGHT, targetValues, 7);
  } else if (value == '9') {
    copyArray(NINE, targetValues, 7);
  } else if (value == '0') {
    copyArray(ZERO, targetValues, 7);
  } else if (value == 'P' || value == 'p') {
    copyArray(LETTER_P, targetValues, 7);
  } else if (value == 'A' || value == 'a') {
    copyArray(LETTER_A, targetValues, 7);
  } else if (value == 'L' || value == 'l') {
    copyArray(LETTER_L, targetValues, 7);
  } else if (value == 'F' || value == 'f') {
    copyArray(LETTER_F, targetValues, 7);
  }
}

void copyArray(byte source[], byte target[], int len) {
  for (int i = 0; i < len; i++) {
    target[i] = source[i];
  }
}

void sendBluetoothData() {
  if (sendData) {
    sendingDataElapsedTime = 0;
    String data = "&M=" + String(selectedMode) + ";T=" + getClockData() + ";C=" + getChronoData() + ";CD=" + getCountDownData() + ";AC=" + getAutoChronoData() + ";ACD=" + getAutoCountDownData() + ";" + getColorsData() + "|\n";
    char bData[data.length()];
    data.toCharArray(bData, data.length());
    //Serial.println("Sending data: " + data);
    Serial1.write(bData);
  }
}

String getClockData() {
  return String(clockHours) + ":" + String(clockMinutes) + ":" + String(clockSeconds) + ":" + String(is24HoursClock);
}

String getChronoData() {
  return String(chronoHours) + ":" + String(chronoMinutes) + ":" + String(chronoSeconds) + ":" + String(isChronoRunning);
}

String getCountDownData() {
  return String(countDownHours) + ":" + String(countDownMinutes) + ":" + String(countDownSeconds) + ":" + String(countDownInitialHours) + ":" + String(countDownInitialMinutes) + ":" + String(countDownInitialSeconds) + ":" + String(isCountDownRunning);;
}

String getAutoChronoData() {
  return String(autoChronoHours) + ":" + String(autoChronoMinutes) + ":" + String(autoChronoSeconds) + ":" + String(autoChronoHoursLimit) + ":" + String(autoChronoMinutesLimit) + ":" + String(autoChronoSecondsLimit) + ":" + String(autoChronoCurrentLoop) + ":" + String(autoChronoMaxLoops) + ":" + String(isAutoChronoRunning);
}

String getAutoCountDownData() {
  return String(autoCountDownHours) + ":" + String(autoCountDownMinutes) + ":" + String(autoCountDownSeconds) + ":" + String(autoCountDownInitialHours) + ":" + String(autoCountDownInitialMinutes) + ":" + String(autoCountDownInitialSeconds) + ":" + String(autoCountDownCurrentLoop) + ":" + String(autoCountDownMaxLoops) + ":" + String(isAutoCountDownRunning);
}

String getColorsData() {
  return "CR=" + String(redColor) + ";CG=" + String(greenColor) + ";CB=" + String(blueColor) + ";B=" + String(ledBrigthness);
}

void readBluetoothData() {
  while (Serial1.available()) {
    char readed = Serial1.read();
    bufferData += readed;
    receivedDataElapsedTime = 0;
  }
}

void processBuffer() {
  char bufferEnd = '|';
  char pairDelitimer = ';';
  char keyValueDelitimer = '=';
  if (bufferData[bufferData.length() - 1] == bufferEnd) {
    //Serial.println(bufferData);
    String key = "";
    String value = "";
    boolean onKey = true;
    for (unsigned int i = 0; i < bufferData.length(); i++) {
      char cData = bufferData[i];
      if (cData == pairDelitimer || cData == bufferEnd) {
        processKeyValuePair(key, value);
        onKey = true;
      } else if (cData == keyValueDelitimer) {
        onKey = false;
      } else if (onKey) {
        key += cData;
      } else {
        value += cData;
      }
    }
    bufferData = "";
  }
}

void processKeyValuePair(String key, String value) {
  if (key == "command") {
    if (value == "start" || value == "status") {
      sendData = true;
    } else if (value == "stop") {
      sendData = false;
    }
  } else if (key == "sync") {
    setClockTime(value);
  } else if (key == "startStopChrono") {
    isChronoRunning = value == "1";
  } else if (key == "resetChrono") {
    isChronoRunning = false;
    initChrono();
  } else if (key == "startStopCountDown") {
    isCountDownRunning = value == "1";
  } else if (key == "resetCountDown") {
    isCountDownRunning = false;
    setCountDownInitialTime(value);
    initCountDown();
  } else if (key == "startStopAutoChrono") {
    isAutoChronoRunning = value == "1";
  } else if (key == "resetAutoChrono") {
    isAutoChronoRunning = false;
    setAutoChronoMaxTime(value);
    initAutoChrono(true);
  } else if (key == "startStopAutoCountDown") {
    isAutoCountDownRunning = value == "1";
  } else if (key == "resetAutoCountDown") {
    isAutoCountDownRunning = false;
    setAutoCountDownInitialTime(value);
    initAutoCountDown(true);
  } else if (key == "changeColor") {
    changeLedColor(value);
  } else if (key == "changeMode") {
    changeMode(value.toInt(), false);
  } else if (key == "format") {
    is24HoursClock = value == "1";
  }
}

void setClockTime(String value) {
  String hours = "";
  String minutes = "";
  String seconds = "";
  String data = "";
  for (unsigned int i = 0; i < value.length(); i++) {
    char cData = value[i];
    if (cData == ':') {
      if (hours == "")
        hours = data;
      else if (minutes == "")
        minutes = data;
      else if (seconds == "")
        seconds = data;
      data = "";
    }  else {
      data += cData;
    }
  }
  if (data != "" && seconds == "")
    seconds = data;
  if (hours != "" && minutes != "" && seconds != "") {
    clockHours = hours.toInt();
    clockMinutes = minutes.toInt();
    clockSeconds = seconds.toInt();
    drawAll();
  }
}

void setCountDownInitialTime(String value) {
  String hours = "";
  String minutes = "";
  String seconds = "";
  String data = "";
  for (unsigned int i = 0; i < value.length(); i++) {
    char cData = value[i];
    if (cData == ':') {
      if (hours == "")
        hours = data;
      else if (minutes == "")
        minutes = data;
      else if (seconds == "")
        seconds = data;
      data = "";
    }  else {
      data += cData;
    }
  }
  if (data != "" && seconds == "")
    seconds = data;
  if (hours != "" && minutes != "" && seconds != "") {
    countDownInitialHours = hours.toInt();
    countDownInitialMinutes = minutes.toInt();
    countDownInitialSeconds = seconds.toInt();
    drawAll();
  }
}

void setAutoChronoMaxTime(String value) {
  String hours = "";
  String minutes = "";
  String seconds = "";
  String loops = "";
  String data = "";
  for (unsigned int i = 0; i < value.length(); i++) {
    char cData = value[i];
    if (cData == ':') {
      if (hours == "")
        hours = data;
      else if (minutes == "")
        minutes = data;
      else if (seconds == "")
        seconds = data;
      else if (loops == "")
        loops = data;
      data = "";
    }  else {
      data += cData;
    }
  }
  if (data != "" && loops == "")
    loops = data;
  if (hours != "" && minutes != "" && seconds != "" && loops != "") {
    autoChronoHoursLimit = hours.toInt();
    autoChronoMinutesLimit = minutes.toInt();
    autoChronoSecondsLimit = seconds.toInt();
    autoChronoMaxLoops = loops.toInt();
    drawAll();
  }
}

void setAutoCountDownInitialTime(String value) {
  String hours = "";
  String minutes = "";
  String seconds = "";
  String loops = "";
  String data = "";
  for (unsigned int i = 0; i < value.length(); i++) {
    char cData = value[i];
    if (cData == ':') {
      if (hours == "")
        hours = data;
      else if (minutes == "")
        minutes = data;
      else if (seconds == "")
        seconds = data;
      else if (loops == "")
        loops = data;
      data = "";
    }  else {
      data += cData;
    }
  }
  if (data != "" && loops == "")
    loops = data;
  if (hours != "" && minutes != "" && seconds != "" && loops != "") {
    autoCountDownInitialHours = hours.toInt();
    autoCountDownInitialMinutes = minutes.toInt();
    autoCountDownInitialSeconds = seconds.toInt();
    autoCountDownMaxLoops = loops.toInt();
    drawAll();
  }
}

void changeLedColor(String value) {
  String red = "";
  String green = "";
  String blue = "";
  String brightness = "";
  String data = "";
  for (unsigned int i = 0; i < value.length(); i++) {
    char cData = value[i];
    if (cData == ':') {
      if (red == "")
        red = data;
      else if (green == "")
        green = data;
      else if (blue == "")
        blue = data;
      else if (brightness == "")
        brightness = data;
      data = "";
    }  else {
      data += cData;
    }
  }
  if (data != "" && brightness == "")
    brightness = data;
  if (red != "" && green != "" && blue != "" && brightness != "") {
    redColor = red.toInt();
    greenColor = green.toInt();
    blueColor = blue.toInt();
    ledBrigthness = brightness.toInt();
    drawAll();
  }
}

void changeMode(int value, boolean  increment) {
  if (increment)
    value++;
  selectedMode = value;
  if (selectedMode > AUTO_COUNTDOWN_MODE) {
    selectedMode = CLOCK_MODE;
  }
  millisStartStopPressed = 0;
  drawAll();
  drawLedMode();
}

void  drawAll() {
  drawSeconds = true;
  drawHours = true;
  drawMinutes = true;
}

byte negativizeColor(byte color) {
  return 255 - color;
}

void saveConfig() {
  configSaved = true;
  configMode = false;
  switch (selectedMode) {
    case COUNTDOWN_MODE:
      initCountDown();
      break;
    case AUTO_CHRONO_MODE:
      initAutoChrono(true);
      break;
    case AUTO_COUNTDOWN_MODE:
      initAutoCountDown(true);
      break;
  }
  if (drawLoops) {
    drawLoops = false;
    selectedDisplay = HOURS_DISPLAY;
  }
  drawAll();
  elapsedTimeOnConfigMode = 0;
}

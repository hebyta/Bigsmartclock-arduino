//asignacion pines

#define BUTTON_UP_PIN 4
#define BUTTON_DOWN_PIN 7
#define BUTTON_START_STOP_PIN 8
#define BUTTON_MODE_PIN 2


//asignacion Variables

byte buttonUp; //0-255
byte buttonDown;
byte buttonStartStop;
byte buttonMode;
unsigned long lastTimeReadButtons; //ultimo tiempo de lectura botones
int years; //-32.768-32.767
byte month;
byte days;
char *week[7] = {"Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo"}; // array(estos van entre corchetes)de string -128-127????
byte daysOfMonths[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
char *monthOfYears[12] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
byte hours;
byte minutes;
byte seconds;
unsigned long milliseconds; //elimina la parte negativa duplicando la posivitiva superior a 4 billones
unsigned long currentTime; //tiempo actual
unsigned long lastTimeClock; //ultimo tiempo reloj
unsigned long elapsedTime; //tiempo trancurrido

//asignacion constantes

const int INITIAL_YEARS = 2020;
const byte INITIAL_MONTH = 1;
const byte INITIAL_DAYS = 1;
const byte INITIAL_HOURS = 0;
const byte INITIAL_MINUTES = 0;
const byte INITIAL_SECONDS = 0;
const byte INITIAL_MILLISECONDS = 0;
const byte INTERVAL_MILLISECONDS = 10;
const byte INTERVAL_BUTTON_MILLISECONDS = 1000;


//Métodos

void setup() { //metodo de configuracion para arduino, solo se ejecuta una vez al principio, no devuelve información.
  Serial.begin(9600); //begin es la velocidad de transmisionde de datos entre el pc y arduino a traves del puerto serie a 9600 baudios por segundo. serial se utiliza para la comunicación entre la placa Arduino y un pc

  years = INITIAL_YEARS; //a los años le corresponde el valor de la conntante initial years = 2020
  month = INITIAL_MONTH;
  days = INITIAL_DAYS;
  hours = INITIAL_HOURS;
  minutes = INITIAL_MINUTES;
  seconds = INITIAL_SECONDS;
  milliseconds = INITIAL_MILLISECONDS;

}


void loop() { //metodo que contiene el orden de ejecucion del programa, se repite constantemente, no devuelve información.
  readTime(); //llama al metodo leer tiempo
  readSensors(); //llama al metodo leer sensors
  calculate(); //Llama al metodo calcular
  draw(); //llama al metodo pintar

}
void readSensors() { //metodo que contiene la lectura de sensores, no devuelve informacion
  readActuators(); //llama al metodo leear actuadores

}


void readActuators() { //metodo que lee los actuadores, no devuelve información.

  if (shouldBeReadButtons()) { // si ocurre lo que esta en el metodo deben ser leidos los botones, entonces se ejecutaras las intrucciones de dentro del if
    buttonUp = digitalRead(BUTTON_UP_PIN);// dentro de button up se almacena la lectura del pin asociado a button up pin
    buttonDown = digitalRead(BUTTON_DOWN_PIN); //
    buttonStartStop = digitalRead(BUTTON_START_STOP_PIN);
    buttonMode = digitalRead(BUTTON_MODE_PIN);
    Serial.println("buttonUp " + String(buttonUp));
    Serial.println("buttonDown " + String(buttonDown));
    Serial.println("buttonStartStop " + String(buttonStartStop));
    Serial.println("buttonMode " + String(buttonMode));
    Serial.println("lastTimeReadButtons " + String(lastTimeReadButtons));
    Serial.println("currentTime " + String(currentTime));

    lastTimeReadButtons = currentTime; // almacenamos en la variable ultimo tiempo leido de los botones el valor de tiempo actual
  }

}


void readTime() { //control del overflow.metodo encargado de la lectura del tiempo, no devuelve información.
  currentTime = millis(); //Devuelve el número de milisegundos transcurridos desde que la placa Arduino comenzó a ejecutar el programa actual. Este número se desbordará(overflow), después de aproximadamente 50 días y lo guarda en la variable current time
  if (lastTimeClock > currentTime) { //aqui solucionamos el overflow, ya que solo hay un momento en el que el ultimo tiempo es matoy que el actual.si ocurre que el ultimo tiempo del reloj es mayor que el tiempo actual, entonces:
    lastTimeClock = currentTime; //se guardara dentro de la variable ultimo tiempo del reloj el valor del tiempo actual
  }
  if (lastTimeReadButtons > currentTime) { //si ocurre que el ultimo tiempo de lectura de los botones es mayor que el tiempo actual, entonces:
    lastTimeReadButtons = currentTime; //se guardara dentro de la variable ultimo tiempo de lectura de los botones el valor del tiempo actual
  }
  elapsedTime = currentTime - lastTimeClock; //tiempo transcurrido = tiempo actual - ultimo tiempo del reloj
}


void calculate() { //metodo encargado de calcular, no devuelve información.
  if (shouldBeClockUpdated()) { // si ocurre lo que esta en el metodo debe ser actualizado el reloj, entonces:
    calculateClockTime(); //llama al metodo de calculo del tiempo del reloj
    lastTimeClock = currentTime; //se guardara dentro de la variable ultimo tiempo del reloj el valor del tiempo actual
  }

}

boolean shouldBeReadButtons() { //metodo  encargado de calcular cada cuando se lee el resultado de los botones, solo devuelve true o false
  return currentTime >= lastTimeReadButtons + INTERVAL_BUTTON_MILLISECONDS; //devuelve el tiempo actual. Este es mayor o igual que el ultimo tiempo de lectura en los botones + el intervalo de tiempo(1segundo)
}

boolean shouldBeClockUpdated() { //metodo encargado de calcular como transcurre el tiempo en el reloj, solo devuelve true o false
  return currentTime >= lastTimeClock + INTERVAL_MILLISECONDS; //devuelve el tiempo actual. Este es mayor o igual que el ultimo tiempo del reloj + el intervalor de tiempo(10milisegundos)
}



void draw() { //metodo encargado de pintar, no devuelve información.
  drawClock(); //llama al metodo pintar el reloj
}


void drawClock() { //metodo que se encarga de pintar los digitos del reloj, no devuelve información.
  String output; //variable que contruye cadenas de texto a partir de diferentes tipos de datos
  String yearsStr;
  String monthStr;
  String daysStr;
  String hoursStr;
  String minutesStr;
  String secondsStr;
  String millisecondsStr;
  int dayOfWeek = getDayOfWeek(); //se guardara dentro de la variable dia de la semana la llamada al metodo coje el dia de la semana

  if (years < 10) { //si los años son menos que 10, entonces:
    yearsStr = "000" + String(years); //se almacenara en la variable yearsStr con el valor 000 y el numero del año que corresponda
  }
  else if (years < 100) { //si no, si los años son menos que 100, entonces:
    yearsStr = "00" + String(years); //se almacenara en la variable yearsStr con el valor 00 y el numero del año que corresponda
  }
  else if (years < 1000) { //si no, si los años son menos que 1000, entonces:
    yearsStr = "0" + String(years); //se almacenara en la variable yearsStr con el valor 0 y el numero del año que corresponda
  }

  else { //si no, entonces:
    yearsStr = String(years); //se almacenara en la variable yearsStr el numero del año que corresponda al año
  }

  if (month < 10) { //si los meses son menos que 10, entonces:
    monthStr = "0" + String(month); //se almacenara en la variable monthStr con el valor 0 y el numero del mes que corresponda
  }
  else {
    monthStr = String(month); //se almacenara en la variable monthStr el numero del mes que corresponda al año
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

  if (milliseconds < 100) { //si los milisegundos son menos que 100, entonces:
    millisecondsStr = "0" + String(milliseconds / 10); //se almacenara en la variable millisecondsStr con el valor 0 y el numero de milisegundos que corresponda entre 10 para calcular las centesimas
  }
  else { //si no:
    millisecondsStr = String(milliseconds / 10); //se almacenara en la variable millisecondsStr el numero de milisegundos que corresponda entre 10 para calcular las centesimas
  }

  //output = String(week[dayOfWeek - 1]) + "  " + String(monthOfYears[month - 1]) + "  " + daysStr + "/" + monthStr + "/" + yearsStr + "  " + hoursStr + ":" + minutesStr + ":" + secondsStr + "." + millisecondsStr;
  //output = hoursStr + ":" + minutesStr + ":" + secondsStr + "." + millisecondsStr;

  //Serial.println(output);
}


void calculateClockTime() { //metodo encaragado del conteo del reloj, no devuelve información.
  byte daysMonths = daysOfMonths[month - 1]; // se guarda en la variable dias mes el valor del array dias del mes que esta en la posicion mes-1
  if (month == 2 && isLeapYears()) { //si comparamos que el mes sea igual a 2 y bisiesto, entonces:
    daysMonths = 29; //se almacena en la variable dias mes el valor de 29
  }

  milliseconds += elapsedTime; //los milisegundos se van sumando con el tiempo trancurrido -> milliseconds = milliseconds + tiempo transcurrido
  if (milliseconds >= 1000) { //si los milisegundos son mayores o iguales que 1000, entonces:
    seconds++; //incrementamos los segundos en 1 y guardamos ese valor en la variable segundos
    milliseconds = milliseconds - 1000; //una vez superado los 1000, regresamos a 0
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

boolean isLeapYears() { //metodo encargado de calcular el año bisiesto, solo devuelve true o false
  if (years % 4 == 0) { //si se divide los años entre 4, devuelve el resto de la operacion y hace una comparacion con el resultado, si el resto es 0, pude ser bisiesto. Entonces:
    if (years % 100 != 0) { //si se divide los años entre 100, devuelve el resto de la operacion, si no es igual a 0, no es divisible entre 100. Entonces:
      return true; // devuelve un true, es bisiesto.
    }
    else if (years % 100 == 0 && years % 400 == 0) { //si no se cumple el if anterior si los años se divide entre 100, devuelve el resto de la operacion y hace una comparacion con el resultado, si el resto es 0 y los años
      //se divide entre 400, devuelve el resto de la operacion y hace una comparacion con el resultado, si el resto es 0, es bisiesto, entonces:
      return true; // devuelve un true, es bisiesto.
    }
  }
  return false; // si no ocurre nada de lo anterior, devuelve un false, no es bisiesto
}

int getDayOfWeek() { //metodo encargado de calcular en que dia de la semana cae cada dia del año, devuelve un numero entero
  int aux = (14 - month) / 12; //variables segun la congruencia de zeller
  int yearsAux = years - aux;
  int monthAux = month + 12 * aux - 2;
  int dayAux = (days + yearsAux + yearsAux / 4 - yearsAux / 100 + yearsAux / 400 + (31 * monthAux) / 12) % 7;
  if (dayAux == 0) { //si comparamos la avriable dia aux y es igual a 0, entonces:
    return 7; //devuelve un 7, domingo, segun zeller.
  }
  else { //si no:
    return dayAux; //devuelve el dia de la semana que sea, 1-lunes, 2-martes...
  }
}

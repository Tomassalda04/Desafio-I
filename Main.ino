int analogPin = A0;  // Pin para el generador de funciones
int botonIniciarPin = 2;  // Botón para iniciar la recolección
int botonDetenerPin = 4;  // Botón para detener la recolección
bool recibiendo = false;  // Estado de la recolección de datos
int ultimoEstadoIniciar = LOW;
int ultimoEstadoDetener = LOW;
int estadoIniciar = LOW;
int estadoDetener = LOW;
long tiempoLecturaIniciar = 0;
long tiempoLecturaDetener = 0;
long tiempoDelay = 50;  // Tiempo de debounce
int capacidad=50;
int *datos= new int [capacidad];

int analogReadAverage(int pin, int samples) { 
  long sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(pin);
  } 
  return sum / samples;
}

void setup() {
  pinMode(botonIniciarPin, INPUT);
  pinMode(botonDetenerPin, INPUT);
  Serial.begin(9600);  // Iniciar el monitor serial
}

void funcionamientoBotones() {
  int lecturaIniciar = digitalRead(botonIniciarPin);
  int lecturaDetener = digitalRead(botonDetenerPin);

  // Manejo del botón de iniciar (debounce)
  if (lecturaIniciar != ultimoEstadoIniciar) {
    tiempoLecturaIniciar = millis();
  }

  if ((millis() - tiempoLecturaIniciar) > tiempoDelay) {
    if (lecturaIniciar != estadoIniciar) {
      estadoIniciar = lecturaIniciar;

      if (estadoIniciar == HIGH) {
        recibiendo = true;  // Iniciar la recolección de datos
        Serial.println("Recolección de datos iniciada...");
        delay(50);  // Pequeño delay para evitar rebotes
      }
    }
  }
  ultimoEstadoIniciar = lecturaIniciar;

  // Manejo del botón de detener (debounce)
  if (lecturaDetener != ultimoEstadoDetener) {
    tiempoLecturaDetener = millis();
  }

  if ((millis() - tiempoLecturaDetener) > tiempoDelay) {
    if (lecturaDetener != estadoDetener) {
      estadoDetener = lecturaDetener;

      if (estadoDetener == HIGH) {
        recibiendo = false;  // Detener la recolección de datos
        Serial.println("Recoleccion de datos detenida...");
        delay(50);  // Pequeño delay para evitar rebotes
      }
    }
  }
  ultimoEstadoDetener = lecturaDetener;

  // Si recibiendo es true, leer e imprimir la señal del generador
  if (recibiendo) {
    float valorSenal = analogRead(analogPin);  // Leer el valor de la señal
    Serial.println(valorSenal);  // Imprimir el valor de la señal en el monitor serial
    delay(100);  // Ajustar la frecuencia de lectura según sea necesario
  }
}

void loop() {
  funcionamientoBotones();  // Llamar a la función para manejar los botones y la recolección de datos
}






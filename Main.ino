int analogPin = 0;
int botonPin = 2;
int boton2Pin= 4;

void setup() {
  pinMode(botonPin, INPUT);
  pinMode(boton2Pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int lectura = digitalRead(botonPin);
  int lectura2 = digitalRead(boton2Pin);
}




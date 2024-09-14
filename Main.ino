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
int capacidad=1;
int numdatos=0;
int *datos= new int [capacidad];
int amplitud = 0;
bool amplitudCalculada = false;
int frecuencia=0;
int num=0;
int num3=0;

void setup() {
  pinMode(botonIniciarPin, INPUT);
  pinMode(botonDetenerPin, INPUT);
  Serial.begin(9600);  // Iniciar el monitor serial
}

int mayor(int* arr, int tamano) {
  int mayor = arr[0];

  for (int i = 0; i < tamano; i++) {
    if (arr[i] > mayor) {
      mayor = arr[i];
    }
  }
  return mayor;
}

int funFrecuencia(int *datos,int tamano){
  float tiempoT= tamano/10;
  int picos=0;
  int mayor=0;
  int menor=0;
  int actual=0;
  float frecuencia=0;
  int aux=0;
  for(int i=0;i<tamano;i++){
    if(aux==0){
      actual=datos[i];
      if(actual>mayor){
        mayor=actual;
      }
      if(actual<mayor && actual>0){
        picos++;
        aux=1; 
        menor=mayor;
        mayor=0;
      }
    }
    else{
      actual=datos[i];
      if(actual<menor){
        menor=actual;
      }
      if(actual>menor){
        aux=0;
      }
    }
  }

  frecuencia=((picos/tiempoT)*100);
  /*Serial.println();
  Serial.print("picos ");
  Serial.println(picos);
  Serial.print("tamano ");
  Serial.println(tamano);
  Serial.print("frecuencia ");
  Serial.print(frecuencia);*/
  return frecuencia;

}

void recodatos(int *&datos, int dato, int posicion, int capacidad){
  datos[posicion]=dato;
  //posicion+=1;
  if(posicion==capacidad){
  	int nuevacap= capacidad+1;
    int *copydatos= new int[nuevacap];
    for(int i=0; i<capacidad;i++){
      copydatos[i]= datos[i];
    }
  delete[] datos;
  datos=copydatos;
  capacidad=nuevacap;
  posicion+=1;
  }
}

int* funcionamientoBotones() {

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
       
        Serial.println("Recoleccion de datos iniciada...");
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
        for(int i=0;i<capacidad-1;i+=1){
          Serial.print(datos[i]);
          Serial.print(" ");
        }
        delete[] datos;
        delay(50);  // Pequeño delay para evitar rebotes
      }
    }
  }
  
  ultimoEstadoDetener = lecturaDetener;

  // Si recibiendo es true, leer e imprimir la señal del generador
  if (recibiendo) {
    int valorSenal = analogRead(analogPin);  // Leer el valor de la señal
    //datos = recodatos(datos,valorSenal,numdatos,capacidad);
    Serial.println(valorSenal); // Imprimir el valor de la señal en el monitor serial
    
    datos[numdatos]=valorSenal;
    numdatos+=1;
    if(numdatos==capacidad){
      int nuevacap= capacidad+1;
      int *copydatos= new int[nuevacap];
      for(int i=0; i<capacidad;i++){
        copydatos[i]= datos[i];
      }
      delete[] datos;
      datos=copydatos;
      capacidad=nuevacap;
    }
    
    delay(100);  // Ajustar la frecuencia de lectura según sea necesario
  }
  return datos;
}

int funAmplitud(int* arr, int tamano) {
  int mayor = arr[0];
  int menor = arr[0];

  // Recorremos el arreglo para encontrar el mayor y menor
  for (int i = 0; i < tamano; i++) {
    if (arr[i] > mayor) {
      mayor = arr[i];
    }
    if (arr[i] < menor) {
      menor = arr[i];
    }
  }
  // Retorna la suma de mayor y menor dividida entre 2
  return ((mayor-menor)/2)/100;
}


void loop() {
  datos = funcionamientoBotones();// Llamar a la función para manejar los botones y la recolección de dato
  
  //PARA CALCULAR LA AMPLITUD; FRECUENCIA
  if (!amplitudCalculada && !recibiendo && numdatos > 0){ // Ajustar el tamaño con el número actual de datos
    amplitud = funAmplitud(datos, numdatos);
    //Serial.print(numdatos/10);
    Serial.println();
    Serial.print("La amplitud calculada es: ");
    Serial.print(amplitud);
    Serial.print(" V");
    amplitudCalculada = true;
    frecuencia=funFrecuencia(datos,numdatos);
    
    
  }
}

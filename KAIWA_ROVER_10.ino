//-----------------------------------------------------------------
//          Programa hecho por: Konrad Peschka
//          Entidad: Kaiwa Technology
//          Fecha: Noviembre 2015
//-----------------------------------------------------------------


//--------------defino librerias y variables para Ultrasonidos------------------


#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

#include <NewPing.h>

#define MAX_DISTANCE 100 // Maximum distance (in cm) to ping.

NewPing sonar_der(A0,A1, MAX_DISTANCE);
NewPing sonar_centro(A2,A3, MAX_DISTANCE);
NewPing sonar_izq(A4,A5, MAX_DISTANCE);


//--------------------------------------
//     Definicion Variables del Programa
//--------------------------------------

//#define debug                  //comentando o descomentando esta linea activo el codigo para debug


const int distancia_critica = 15;
const int distancia_deteccion=30;

const int tiempo_stop=20;
const int tiempo_giro=50;
const int tiempo_atras=100;
const int tiempo_frente=20;

int distancia_der=0;
int distancia_der_anterior=30;
int distancia_centro=0;
int distancia_centro_anterior=30;
int distancia_izq=0;
int distancia_izq_anterior=30;

const int cantidad=3;
int matriz_der[cantidad];
int matriz_centro[cantidad];
int matriz_izq[cantidad];


int velocidad=160;
const int velocidad_inicial=150;
const int velocidad_incremento=5;
int velocidad_frente=velocidad_inicial;
int velocidad_atras=velocidad_inicial;

int aux_matriz=0;
//----------------------------------------
//  Funcion SETUP
//----------------------------------------

void setup()
{
  
#ifdef debug
 Serial.begin(9600);
#endif

  pinMode(MOTORLATCH, OUTPUT);
  pinMode(MOTORENABLE, OUTPUT);
  pinMode(MOTORDATA, OUTPUT);
  pinMode(MOTORCLK, OUTPUT);

  pinMode(10, INPUT);
 
 movimiento_stop();

 for(int i=0;i<cantidad;i++)
 {
  matriz_der[i]=sonar_der.ping_cm();
  matriz_centro[i]=sonar_centro.ping_cm();
  matriz_izq[i]=sonar_izq.ping_cm();
 }
 
    
}

//----------------------------------------
//  Funcion principal del codigo
//----------------------------------------

void loop()
{
while(digitalRead(10)==LOW)
{
  #ifdef debug
    Serial.print("Boton Arranque\n");
    #endif
     delay(50);
}

while(1)
{
int aux_switch_case=funcion_ultrasonido();

    #ifdef debug
    Serial.print("\naux=");
    Serial.print(aux_switch_case);
    #endif

switch (aux_switch_case) 
{
    case 0:  // Hay algo critico a la derecha
    movimiento_stop();
    delay(tiempo_stop);
    movimiento_atras();
    delay(tiempo_atras);
    movimiento_izquierda();
    delay(tiempo_giro);

    #ifdef debug
    Serial.print("Caso 0");
    #endif

    break;
      
    case 1:  // Hay algo critico en el centro
    movimiento_stop();
    delay(tiempo_stop);
    movimiento_atras();
    delay(tiempo_atras);
    movimiento_izquierda();
    delay(tiempo_giro);

    #ifdef debug
    Serial.print("Caso 1");
    #endif

    break;
      
    case 2:  // Hay algo critico a la izquierda
    movimiento_stop();
    delay(tiempo_stop);
    movimiento_atras();
    delay(tiempo_atras);
    movimiento_derecha();
    delay(tiempo_giro);

    #ifdef debug
    Serial.print("Caso 2");
    #endif

     break;
      
    case 3:  //todo libre adelante
    movimiento_stop();
    delay(tiempo_stop);
    movimiento_frente();
    delay(tiempo_frente);

    #ifdef debug
    Serial.print("Caso 3");
    #endif

      break;
      
    case 4:  //hay algo a la izquierda
    movimiento_stop();
    delay(tiempo_stop);
    movimiento_derecha();
    delay(tiempo_giro);

    #ifdef debug
    Serial.print("Caso 4");
    #endif

      break;
      
    case 5: // hay algo en el medio
    movimiento_stop();
    delay(tiempo_stop);
    movimiento_izquierda();
    delay(tiempo_giro);

    #ifdef debug
    Serial.print("Caso 5");
    #endif

      break;
      
    case 6:  //hay algo a la izquierda
    movimiento_stop();
    delay(tiempo_stop);
    movimiento_derecha();
    delay(tiempo_giro);

    #ifdef debug
    Serial.print("Caso 6");
    #endif

      break;
      
    case 7:  //hay algo derecha
     movimiento_stop();
    delay(tiempo_stop);
    movimiento_izquierda();
    delay(tiempo_giro); 

    #ifdef debug
    Serial.print("Caso 7");
    #endif
   
    break;
      
    case 8:  //no hacemos nada

    #ifdef debug
    Serial.print("Caso 8");
    #endif

      break;
      
    case 9:  //hay algo a la derecha
    movimiento_stop();
    delay(tiempo_stop);
    movimiento_izquierda();
    delay(tiempo_giro); 

    #ifdef debug
    Serial.print("Caso 9");
    #endif

      break;
      
    case 10:  //no hacemso nada

    #ifdef debug
    Serial.print("Caso 10");
    #endif

      break;

     case 11:  //no hacemso nada

    #ifdef debug
    Serial.print("Caso 11");
    #endif

      break;
 
  }

 }

}

// ---------------------------------
// definimos librerias de Ultrasonido
//-----------------------------------

int funcion_ultrasonido(void)
{
  lectura_ultrasonido();
   
 if(distancia_der<=distancia_critica)
 return 0;
 if(distancia_centro<=distancia_critica)
 return 1;
 if(distancia_izq<=distancia_critica)
 return 2; 
 
 if((distancia_der>distancia_deteccion)&&(distancia_centro>distancia_deteccion)&&(distancia_izq>distancia_deteccion))
 return 3; 

  if((distancia_der>distancia_deteccion)&&(distancia_centro>distancia_deteccion)&&(distancia_izq<=distancia_deteccion))
 return 4; 

  if((distancia_der>distancia_deteccion)&&(distancia_centro<=distancia_deteccion)&&(distancia_izq>distancia_deteccion))
 return 5; 

  if((distancia_der>distancia_deteccion)&&(distancia_centro<=distancia_deteccion)&&(distancia_izq<=distancia_deteccion))
 return 6; 

  if((distancia_der<=distancia_deteccion)&&(distancia_centro>distancia_deteccion)&&(distancia_izq>distancia_deteccion))
 return 7; 

  if((distancia_der<=distancia_deteccion)&&(distancia_centro>distancia_deteccion)&&(distancia_izq<=distancia_deteccion))
 return 8; 

  if((distancia_der<=distancia_deteccion)&&(distancia_centro<=distancia_deteccion)&&(distancia_izq>distancia_deteccion))
 return 9; 

  if((distancia_der<=distancia_deteccion)&&(distancia_centro<=distancia_deteccion)&&(distancia_izq<=distancia_deteccion))
 return 10; 

 return 11;
 
}
void lectura_ultrasonido(void)
{
   distancia_der=sonar_der.ping_cm();
   distancia_centro=sonar_centro.ping_cm();
   distancia_izq=sonar_izq.ping_cm();

   #ifdef debug
   Serial.print("\nDer= ");
   Serial.print( distancia_der);
   Serial.print("Centro= ");
   Serial.print(distancia_centro);
   Serial.print("Izq= ");
   Serial.print(distancia_izq);
   #endif
   
  if(distancia_der==0)
  distancia_der=distancia_der_anterior;
  else
  distancia_der_anterior=distancia_der;
  
  if(distancia_centro==0)
  distancia_centro=distancia_centro_anterior;
  else
  distancia_centro_anterior=distancia_centro;
  
  if(distancia_izq==0)
  distancia_izq=distancia_izq_anterior;
  else
  distancia_izq_anterior=distancia_izq;

    
   matriz_der[aux_matriz]=distancia_der;
   matriz_centro[aux_matriz]=distancia_centro;
   matriz_izq[aux_matriz]=distancia_izq;

   aux_matriz++;
   if(aux_matriz>=cantidad)
   aux_matriz=0;

   #ifdef debug
    Serial.print("\nAux_matriz= ");
    Serial.print(aux_matriz);

   for(int i=0;i<cantidad;i++)
   {
   Serial.print("\n");
   Serial.print(matriz_der[i]);
   Serial.print("\t");
   Serial.print(matriz_centro[i]);
   Serial.print("\t");
   Serial.print(matriz_izq[i]);
   }
   #endif
   
   distancia_der=0;
   distancia_centro=0;
   distancia_izq=0;
   
   for(int i=0;i<cantidad;i++)
   {
    distancia_der=distancia_der+matriz_der[i];
    distancia_centro=distancia_centro+matriz_centro[i];
    distancia_izq=distancia_izq+matriz_izq[i];
   }
   distancia_der=distancia_der/cantidad;
   distancia_centro=distancia_centro/cantidad;
   distancia_izq=distancia_izq/cantidad;

      #ifdef debug
   Serial.print("\nDer= ");
   Serial.print( distancia_der);
   Serial.print("Centro= ");
   Serial.print(distancia_centro);
   Serial.print("Izq= ");
   Serial.print(distancia_izq);
   #endif
}




// ---------------------------------
// definimos librerias de movimiento
//-----------------------------------

void movimiento_stop(void)
{
   motor1.setSpeed(1);  
 motor2.setSpeed(1); 
 motor3.setSpeed(1); 
 motor4.setSpeed(1); 

   delay(3);
}

void movimiento_frente(void)
{
  /*
   velocidad_atras=velocidad_inicial;
  velocidad_frente=velocidad_frente+velocidad_incremento;
  if(velocidad_frente>255)
  velocidad_frente=255;
 */
 motor1.setSpeed(velocidad);  
 motor2.setSpeed(velocidad); 
 motor3.setSpeed(velocidad); 
 motor4.setSpeed(velocidad); 

  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);

   delay(3);
  
}

void movimiento_atras(void)
{
    velocidad_frente=velocidad_inicial;
  velocidad_atras=velocidad_atras+velocidad_incremento;
    if(velocidad_atras>255)
  velocidad_atras=255;
  
   motor1.setSpeed(velocidad_atras);  
 motor2.setSpeed(velocidad_atras); 
 motor3.setSpeed(velocidad_atras); 
 motor4.setSpeed(velocidad_atras); 

 
   motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);

  
  delay(3);
  
}

void movimiento_derecha(void)
{
   motor1.setSpeed(velocidad);  
 motor2.setSpeed(velocidad); 
 motor3.setSpeed(velocidad); 
 motor4.setSpeed(velocidad); 

   motor1.run(BACKWARD);
  motor2.run(BACKWARD);
    motor3.run(FORWARD);
  motor4.run(FORWARD);

}

void movimiento_izquierda(void)
{
   motor1.setSpeed(velocidad);  
 motor2.setSpeed(velocidad); 
 motor3.setSpeed(velocidad); 
 motor4.setSpeed(velocidad); 

    motor1.run(FORWARD);
  motor2.run(FORWARD);
    motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  

  

}



/**
 * OneTaskBlink.ino muestra como ejecutar una tareas dentro de un sólo periodo 
 * de tiempo.
 *    
 *   Muestra como ejecutar una tarea {task1} en un intervalo de tiempo.
 *   
 *    task    descripción
 *     1      enciende-apaga grupo de leds durante un intervalo de tiempo de 1000ms
 *    
*/
#define INTERVAL  1000
#define LED_BUILTIN     13  // PB0 
#define LED_ROJO_UNO    14  // PC0,A0
#define LED_AMBAR_UNO   15  // PC1,A1
#define LED_VERDE_UNO   16  // PC2,A2

#define LED_ROJO_DOS    17  // PC3,A3
#define LED_AMBAR_DOS   18  // PC4,A4
#define LED_VERDE_DOS   19  // PC5,A5

void configRecursos();
void togglePin(uint8_t);
void readInputAt(uint8_t ); // lee una entrada digital
bool readPINB(uint8_t );
int freeMemory();


void setup() {
    configRecursos();
}

void loop() {
  
  togglePin(LED_BUILTIN);
  togglePin(LED_ROJO_UNO);
  togglePin(LED_AMBAR_UNO);
  togglePin(LED_VERDE_UNO);
  togglePin(LED_ROJO_DOS);
  togglePin(LED_AMBAR_DOS);
  //togglePin(LED_VERDE_DOS);
  //readInputAt(PB0);

  if(readPINB(PB0))  {PORTC |=  (1<<5);}
  else               {PORTC &= ~(1<<5);}
  
  delay(INTERVAL);
  Serial.println(freeMemory());
}

/** configura recursos de periféricos auxiliares */
void configRecursos(){
    Serial.begin(9600); // habilita enviar datos por stdout. 
    
    PORTB  |= (1<<PB0); // digital input Pull-up {PB0,IO8}
    DDRB   &= (1<<PB0); // digital input Pull-up {PB0,IO8} 
    _NOP();             // insert for synchronization
   
    DDRC |= 0X3F;       // digital output {PC0:PC5,AD0:AD5,IO14:IO19}
 }

/* toggle en un pin de salida digital [2]*/
void togglePin(uint8_t pin){
     digitalWrite(pin, !digitalRead(pin));  // toggle state 
  }   


/* lee una entrada digital */
void readInputAt(uint8_t pin){
  // Reading the Pin Value
  if ((PINB & (1<<pin)) == 1)   { Serial.println("ON"); PORTC |=  (1<<5);} // Writing HIGH
        else                    { Serial.println("OFF");PORTC &= ~(1<<5);} // Writing LOW
}
  
/* retorna estado de una entrada digital */
bool readPINB(uint8_t pin){
  // Reading the Pin Value
  //  if ((PINB & (1<<pin)) == 1)   { return true; }
  //        else                    { return false;}
      return(((PINB & (1<<pin)) == 1)? true:false);
}



#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

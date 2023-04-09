#include <12f629.h>
//#include <16f886.h>
#fuses INTRC_IO,NOWDT, NOMCLR
#use delay (clock = 4000000)

char ledRojo=PIN_a0;
char ledAmarillo=PIN_a1;
char ledVerde=PIN_a2;
/*
int1 BOTON1 =0;
int1 BOTON2 =0;


struct Persona{
   char nombre[6];
   int edad;
   char quienLlama[6];
   int cont;
   int cont1;
   int cont2;
   int flag1;
   int flag3;
 };
 
void copiarNombre(struct Persona *p) {
   p->edad = p->edad * 2;
   
   strcpy(p->quienLlama, "fuiyo");//Se usan punteros para modificar la Persona que se paso como parametro
   p->edad=0;
}

 void Leds(char x){
 
            output_high(x);
            delay_ms(500);                                                                     
            output_low(x);
            delay_ms(500);
   }
   
 void leerEntradas(){
      BOTON1 = INPUT (PIN_a5);
      BOTON2= INPUT (PIN_a3);
 }
 */
 /////////Funciones////////////
 void encender(char luz, int time){
   output_high(luz);
   delay_ms(time*1000);
 }
  void apagar(char luz){
   output_low(luz);
 }
 ////////////////////////////////
 
 
void main() 
 { 
   //setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256);
   //enable_interrupts(INT_TIMER0);
   //set_TIMER0(0x61); //inicializa el timer0
   //enable_interrupts(GLOBAL);
   
   
    output_a(0x00);
    set_tris_A (0b11111000);
    
    
    while(1) 
      { 
         encender(ledRojo,10);
         encender(ledAmarillo,2);
         apagar(ledRojo);
         apagar(ledAmarillo);
         encender(ledVerde,5);
         apagar(ledVerde);
         encender(ledAmarillo,2);
         apagar(ledAmarillo);
         //delay_ms(2000);

      } 
    
    
    
    
    
    /*
    struct Persona jhonny= {"Jhonny",31,"sumadre"};
    



           // Asignación de valores a los campos de la estructura
           //strcpy(jhonny.nombre, "Jhonny");
           //jhonny.edad = 31;
           
    
    while(1) 
    { 
           leerEntradas();
           copiarNombre(&jhonny);
      
          if(  BOTON1 && BOTON2  )  {  //Condicion 2 botones
            char miPin =PIN_a0;
            Leds(miPin);
            miPin =PIN_a1;
            Leds(miPin);
           
            }
   }
   
   
   */
}

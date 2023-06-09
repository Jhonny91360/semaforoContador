//Primer push github
#INCLUDE       <16F886.H>
#FUSES         XT,NOWDT,PROTECT, NOMCLR, FCMEN
#USE           DELAY(CLOCK=4000000)

///Entradas///
int1 redSignal;
int1 yellowSignal;
int1 greenSignal;
///////////////
///Variables///
int timeCounter = 0;
int counterBlink=0;
///Estructura de objeto para cada color/////
struct Color{
   boolean counting;    //Booleano para saber si se esta tomando tiempo de ese color
   boolean decreasing;  //Booleano para saber si se esta decrementando el tiempo de ese color
   int actualTime;      //Para almacenar el conteo actual
   int pastTime;        //Para almacenar el conteo pasado y comparar
   int decTime;         //Para decrementar conteo e ir mostrando en los leds
};
///Estructura de objeto para estado del programa/////
struct Cycle{
   char estate[20];
   boolean initialState;
};


////////// Inicializo objeto de cada color/////////////
struct Color Red = {false,false,0,0,0};
struct Color YellowDown = {false,false,0,0,0};
struct Color YellowUp = {false,false,0,0,0};
struct Color Green = {false,false,0,0,0};
struct Cycle ProgramCycle={"",true};

//////////////////////////Funciones///////////////////////

//Interrupcion Timer1 cada 10ms//
#int_TIMER1                   
VOID TIMER1_isr(){ 
   SET_TIMER1(64286);
   timeCounter+=20;   //Ojo esto deberia ser +1, pero por simulacion se deja en 20
   
   
   //Contar en segun color
   if(Red.counting && timeCounter>=100 ){
      Red.actualTime++;
      output_toggle(pin_a4);
      timeCounter=0;
   }
   if(YellowDown.counting && timeCounter>=100 ){
      YellowDown.actualTime++;
      timeCounter=0;
   }
   if(Green.counting && timeCounter>=100 ){
      Green.actualTime++;
      timeCounter=0;
   }
   if(YellowUp.counting && timeCounter>=100 ){
      YellowUp.actualTime++;
      timeCounter=0;
   }
   
   //Decrementar segun color
   
   if(Red.decreasing && timeCounter>=100){
      Red.decTime--;
      output_toggle(pin_a5);
      timeCounter=0;
      if(Red.decTime<1) Red.decreasing=false;
   }
   
   if(YellowDown.decreasing && timeCounter>=100){
      YellowDown.decTime--;
      timeCounter=0;
      if(YellowDown.decTime<1) YellowDown.decreasing=false;
   }
   
   if(Green.decreasing && timeCounter>=100){
      Green.decTime--;
      timeCounter=0;
      if(Green.decTime<1) Green.decreasing=false;
   }
   
   if(YellowUp.decreasing && timeCounter>=100){
      YellowUp.decTime--;
      timeCounter=0;
      if(YellowUp.decTime<1) YellowUp.decreasing=false;
   }
  
   //Blink durante toma de tiempos
  
   if(ProgramCycle.initialState ){
      counterBlink+=20;   //Ojo esto deberia ser +1, pero por simulacion se deja en 20
      if(counterBlink>=100 ){
      output_toggle(pin_a0);
      counterBlink=0;
      }
   }
  
  
  
  
}

//Leer las entradas de se�ales segun color/////
void readSignals(){    
   redSignal=INPUT (PIN_b0);
   yellowSignal=INPUT (PIN_b1);
   greenSignal=INPUT (PIN_b2);
}


//////////////////////////////////////////////////////////


////////////Void main /////////////////////
void main(){

///Configuracion de puertos///////////////
output_a(0x00);
SET_TRIS_A (0X00);
output_b(0x0f);
SET_TRIS_B (0X00);
enable_interrupts(int_TIMER1);
ENABLE_INTERRUPTS (GLOBAL);
//////////////////////////////////////////
///Configuracion de timer///////////////
SETUP_TIMER_1 (T1_INTERNAL|T1_DIV_BY_8);    //Configuracion para causar interrupcion
SET_TIMER1 (64286);                         //cada 10ms
//////////////////////////////////////

while(true){
        
      while(ProgramCycle.initialState){      //Si el programa esta en su estado inicial(leyendo tiempos)
      
         readSignals();                      //Leo entradas
         while(!redSignal){readSignals();}   //Espero a recibir se�al en rojo
         Red.counting=true;                  //Activo flag para contar rojo
         readSignals();                      //Leo entradas
            while(redSignal){                //Mientras estoy leyendo rojo
                 readSignals();
                 if(yellowSignal) YellowDown.counting=true;  //Reviso si recibo se�al amarillo, para activar flag counting amarillo hacia abajo
               }
         Red.counting=false;                 //Desactivo flag de conteo rojo
         YellowDown.counting=false;          //Desactivo flag de conteo amariilo hacia abajo
         Red.decTime=Red.actualTime;          //Copio conteo en variable que va a decrementar
         YellowDown.decTime=YellowDown.actualTime;          //Copio conteo en variable que va a decrementar
         readSignals();                      //Leo entradas
         while(!greenSignal){readSignals();} //Espero a recibir se�al en verde
         Green.counting=true;                //Activo flag para contar verde
         while(greenSignal){readSignals();}  //Espero a dejar de recibir se�al verde
         Green.counting=false;               //Desactivo flag para contar verde
         Green.decTime=Green.actualTime;          //Copio conteo en variable que va a decrementar
         while(!yellowSignal){readSignals();} //Espero a recibir se�al en amarillo hacia arriba
         YellowUp.counting=true;               //Activo flag para contar amarillo hacia arriba
         while(yellowSignal){readSignals();} //Espero a dejar de recibir se�al de amararillo
         YellowUp.counting=false;            //Desactivo flag de conteo amarillo hacia arriba
         YellowUp.decTime=YellowUp.actualTime;          //Copio conteo en variable que va a decrementar
         ProgramCycle.initialState=false;     //Para forzar siguiente while
      }
      
      while(!ProgramCycle.initialState){     //Visualizar tiempos
      
         readSignals();                      //Leo entradas
         while(!redSignal){readSignals();}   //Espero a recibir se�al en rojo
         Red.decreasing=true;                //Activo flag para decrementar rojo
         readSignals();                      //Leo entradas
            while(redSignal){                //Mientras estoy leyendo rojo
                 readSignals();
                 output_high(pin_a0);        //Activo led red
                 if(yellowSignal){           //Reviso si recibo se�al amarillo, para activar flag decreasing amarillo hacia abajo
                     YellowDown.decreasing=true;
                     output_high(pin_a1);        //Activo led yellow
                     } 
               }
        output_low(pin_a0);                  //Apago red
        output_low(pin_a1);                  //Apago yellow
        Red.decreasing=false;
        YellowDown.decreasing=false;
        
        readSignals();                      //Leo entradas
        while(!greenSignal){readSignals();} //Espero a recibir se�al en verde
        Green.decreasing=true;                //Activo flag para decrementar verde
        output_high(pin_a2);                 //Activo led green
        while(greenSignal){readSignals();}  //Espero a dejar de recibir se�al verde
        output_low(pin_a2);                 //Apago  led green
        Green.decreasing=false;
        
        while(!yellowSignal){readSignals();} //Espero a recibir se�al en amarillo hacia arriba
        YellowUp.decreasing=true;            //Activo flag para contar amarillo hacia arriba
        output_high(pin_a1);                 //Activo led yellow
        while(yellowSignal){readSignals();} //Espero a dejar de recibir se�al de amararillo
        output_low(pin_a1);                 //Apago led yellow
        YellowUp.decreasing=false;
        
        //Terminado el ciclo, recupero tiempos para repetir visualizacion
        Red.decTime=Red.actualTime;          //Copio conteo en variable que va a decrementar
        YellowDown.decTime=YellowDown.actualTime;          //Copio conteo en variable que va a decrementar
        Green.decTime=Green.actualTime;          //Copio conteo en variable que va a decrementar
      }  
 
 
 }

}
///////////////////////////////////////////

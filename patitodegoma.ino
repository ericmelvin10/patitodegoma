/***********************************************
* COMENTARIOS GENERALES     patitodegoma       *
************************************************/
/*

"patitodegoma v0.1" ha sido creado por @ericmelvin10 (Adrian Aceituno) - https://github.com/ericmelvin10/patitodegoma -
Funcionamiento básico de "patitodegoma":
Sin tarjeta SD no hace nada (retorna un error).
Se entiende que está en modo "configuración" y el led mostrará un comportamiento de "respiración" antes de que se ejecuta el programa.
Con tarjeta SD ejecutará el payload que está dentro. Si está en modo debug, se mostrará el estado con unas luces parpadeando.
Cuando está preparado para actuar o "armado" el led permanecerá fijo, o cuando ha terminado correctamente la ejecución.
Mientras se está ejecutando el payload, el led parpadeará y cuando acabe volverá al color fijo (si no hay error).
En caso de error, los dos leds se encenderan alternamente.
El payload se ejecutará una sola vez.
El fichero de payload deberá estar en formato rubberducky y DEBERÁ ACABAR CON UNA LINEA EN BLANCO!!!!!
El fichero de payload por defecto es "payload.txt"
Este proyecto se puede utilizar para pentesting y/o para automatizar pruebas de entrada de teclado (introducción de datos), o lo que se te ocurra.
No me hago responsable del uso que un tercero le pueda dar, así como tampoco me hago responsable del funcionamiento o malfuncionamiento derivado de la modificación de este código.


Este proyecto se basa en el trabajo de otras personas:

- https://github.com/mharjac/bad_ducky
- https://github.com/basic4/USB-Rubber-Ducky-Clone-using-Arduino-Leonardo-Beetle
- https://github.com/jerwuqu/ardoducky
- https://github.com/Screetsec/Pateensy
- https://github.com/Seytonic/Duckduino-microSD
- https://malduino.com/


TODO: Actualmente no se aceptan combinaciones de más de 2 teclas simultàneas (modificador + otra tecla) por lo que teclas como control + alt + sup o control+shift+esc no funcionan.
- permitir combinaciones de con 2 teclas modificadoras pulsadas.



*/




/***********************************************
* INCLUDES                                     *
***********************************************/

//#include <Keyboard.h> //cargamos las librerias para utilizar arduino como un teclado (us keyboards)
#include "Keyboard.h"   //version custom de keyboard modificada para teclado español
#include <SD.h>       //cargamos las librerias para acceder a la tarjeta SD
#include <SPI.h>      //utilizamos la salida del puerto serie para leer el debug





/***********************************************
* DEFINICIONES                                 *
***********************************************/
//#define DEBUG 1           //definición para debugar por serial, comentar si no se quiere que salga info por serial.
//definiciones de teclas no standard.
#define KEY_MENU 0xED
#define KEY_BREAK 0xD0
#define KEY_NUMLOCK 0xDB
#define KEY_PRINTSCREEN 0xCE
#define KEY_SCROLLOCK 0xCF
#define KEY_SPACE 0xB4






/***********************************************
* VARIABLES                                    *
***********************************************/
const String fileName = "payload.txt";    //define el nombre del fichero de payload, por si lo queréis cambiar
int led = 8;
float smoothness_pts = 200;               //se utiliza para la velocidad de la respiración del led. cuanto más grande el valor más lento el cambio.
int mode = 0;                             //modo en el que se encuentra el patitodegoma: configuracion (0), armado (1), en ejecución (2), error (-1)
const int chipSelect = 4;                 //se utiliza para indicar el puerto de la tarjeta SD
File payload_file;                        //esta es la ruta del fichero de payload
File config_file;                         //esta es la ruta del fichero de config (si fuera necesario)
String payload = "";                      //aqui guardamos el contenido del  payload.
int defaultDelay = 0;                     //este es el delay por defecto, se utiliza para igualarlo a la sintaxis del script de rubberducky DEFAULTDELAY y/o DEFAULT_DELAY
bool special = false;                     //esta variable sirve para que la modifique la función de eval sentence.



/***********************************************
* SETUP                                        *
************************************************/
void setup() {
// put your setup code here, to run once:

pinMode(led, OUTPUT);             //led digital más cercano al procesador
pinMode(LED_BUILTIN, OUTPUT);     //seteamos el led que viene en la placa, mas alejado del procesador.
Keyboard.begin();                 //activamos la actividad de teclado de nuestro arduino
#ifdef DEBUG
Serial.begin(9600);               //inicializamos el puerto serie, solo para debug, para la version final comentar esta linea.
delay(500);
#endif

//esperamos un poco, mientras ponemos la luz en modo "respiración" -- está eliminado para mejorar la velocidad de ejecución
#ifdef DEBUG
for (int i = 0; i < 3 ; i++)
{
  iluminacion(0);
}
#endif

//después de 3 segundos ejecutamos el payload ;)


mode = exec_payload();              //ejecutamos el payload
Keyboard.end();                     //finalizamos la actuación del teclado
}







/**********************************************
* metodos y funciones                         *
**********************************************/


////////////////
//exec_payload//
////////////////
int exec_payload()
{
  //comprobamos que hay tarjeta SD
  if (!SD.begin(chipSelect))
  {
    #ifdef DEBUG
    Serial.println("No hay tarjeta SD");
    #endif
    return -1; //error
  }

  delay (100);
  //si no da error leemos el fichero de payload
  //payload_file = SD.open("payload.txt");
  payload_file = SD.open(fileName);
  //si existe el fichero de payload seguimos
  if (payload_file)
  {
    #ifdef DEBUG
    Serial.println("Fichero abierto");
    #endif
    //creamos las variables sólo si el fichero existe
    char cp;                    //puntero del lector de caracteres (char pointer)
    String cmd = "";            //comando (la primera palabra de cada linea)
    String sentence = "";       //sentencia (el resto de palabras hasta final de linea)
    //mientras el fichero no se acabe...
    while (payload_file.available())
    {
      //leemos los caracteres hasta tener la 1a palabra que será el comando.
      //leemos el caracter
      cp = payload_file.read();

      //---------->cmd-----------//
      //si es final de palabra y cmd está vacío, sabemos que la palabra anterior era un comando
      if (((cp == ' ') || (cp == '\n')) && (cmd == ""))
      {
        cmd = payload;
        if (cp == '\n')
        {
          //si el payload es REM, borramos el cmd y passamos de lo que venga después hasta encontrar un salto de caracter.
          if (cmd == "REM"){

            while ((payload_file.available()) && (cp != "\n"))
            {
              cp = payload_file.read();
            }
            cmd ="";
          }

          //si el payload es MENU o APP, lo ejecutamos
          else if (cmd == "MENU" || cmd == "APP")
          {
            cmd="";
            Keyboard.press(KEY_MENU);
            delay(20);
            Keyboard.release(KEY_MENU);
          }


          //si es ENTER, lo ejecutamos
          else if (cmd == "ENTER")
          {
            cmd="";
            Keyboard.press(KEY_RETURN);
            Keyboard.release(KEY_RETURN);
          }

          else if ((cmd == "WINDOWS") || (cmd == "GUI"))
          {

            cmd = "";
            Keyboard.press(KEY_LEFT_GUI);
            delay(defaultDelay);
            Keyboard.release(KEY_LEFT_GUI);

          }


          //reseteamos el sentence por si las moscas
          sentence = "";
        }
        //utilizaremos el payload para guardar lo siguiente (cmd o sentence)
        payload = "";

      }


      //------->sentence-----------//
      else
      {
        //si es final de linea sabemos que debemos ejecutar la sentencia.
        if ((cp == '\n') && (cmd != ""))
        {
          sentence = payload;

          //Primero miramos el tipo de cmd, los que llevan una sentencia "normal"
          if (cmd == "DEFAULT_DELAY" || cmd == "DEFAULTDELAY")
          {
            defaultDelay = sentence.toInt();
          }

          else if (cmd == "DELAY")
          {
            delay(sentence.toInt());
          }

          else if (cmd == "STRING")
          {
            Keyboard.print(sentence);
          }

          //ahora vamos a revisar el resto de cmd "modificadores"
          else if ((cmd == "WINDOWS") || (cmd == "GUI"))
          {

            sentence = eval_sentence(sentence);
            Keyboard.press(KEY_LEFT_GUI);
            if (special==true) {Keyboard.press(sentence.toInt());}
            else {Keyboard.print(sentence);}
            Keyboard.releaseAll();
          }

          else if (cmd == "SHIFT")
          {
            sentence = eval_sentence(sentence);
            Keyboard.press(KEY_LEFT_SHIFT);
            if (special==true) {Keyboard.press(sentence.toInt());}
            else {Keyboard.print(sentence);}
            Keyboard.releaseAll();
          }

          else if (cmd == "ALT")
          {
            sentence = eval_sentence(sentence);
            Keyboard.press(KEY_LEFT_ALT);
            if (special==true) {Keyboard.press(sentence.toInt());}
            else {Keyboard.print(sentence);}
            Keyboard.releaseAll();
          }

          else if ((cmd == "CONTROL") || (cmd == "CTRL"))
          {
            sentence = eval_sentence(sentence);
            Keyboard.press(KEY_LEFT_CTRL);
            if (special==true) {Keyboard.press(sentence.toInt());}
            else {Keyboard.print(sentence);}
            Keyboard.releaseAll();
          }

          else if ((cmd == "CTRL-SHIFT") || (cmd == "CONTROL-SHIFT"))
          {
            sentence = eval_sentence(sentence);
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_SHIFT);
            if (special==true) {Keyboard.press(sentence.toInt());}
            else {Keyboard.print(sentence);}
            Keyboard.releaseAll();
          }

          else if ((cmd == "GUI-SHIFT") || (cmd == "WINDOWS-SHIFT"))
          {
            sentence = eval_sentence(sentence);
            Keyboard.press(KEY_LEFT_GUI);
            Keyboard.press(KEY_LEFT_SHIFT);
            if (special==true) {Keyboard.press(sentence.toInt());}
            else {Keyboard.print(sentence);}
            Keyboard.releaseAll();
          }

          else if ((cmd == "CTRL-GUI") || (cmd == "CTRL-WINDOWS") || (cmd == "CONTROL-GUI") || (cmd=="CONTROL-WINDOWS"))
          {
            sentence = eval_sentence(sentence);
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press(KEY_LEFT_GUI);
            if (special==true) {Keyboard.press(sentence.toInt());}
            else {Keyboard.print(sentence);}
            Keyboard.releaseAll();
          }

          else if ((cmd == "ALT-GUI") || (cmd == "ALT-WINDOWS"))
          {
            sentence = eval_sentence(sentence);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press(KEY_LEFT_GUI);
            if (special==true) {Keyboard.press(sentence.toInt());}
            else {Keyboard.print(sentence);}
            Keyboard.releaseAll();
          }

          //limpiamos las variables
          cmd = "";
          sentence = "";
          payload = "";
          special = false;

        }

        else
        {
          //si no es ni CMD ni final de linea, añadimos al payload.
          payload += cp;
        }

      }
        //mientras trabaja iluminamos
        #ifdef DEBUG
        Serial.println((String)"CMD:" + cmd);
        Serial.println((String)"Sentence:" + sentence);
        iluminacion(2);
        #endif

    }

    payload_file.close();
    return 1; //armado
  }

  //si no existe el fichero de payload salimos con error
  else {
    Serial.print((String)"Error: el fichero "+payload_file+" no existe");
    return -1;  //error
  }

}

/////////////////
//eval_sentence//
/////////////////
String eval_sentence(String sent)
{

  if ((sent == "BREAK") || (sent == "PAUSE"))
  {
    sent = KEY_BREAK;
  }
  else if (sent == "CAPSLOCK")
  {
    sent = KEY_CAPS_LOCK;
  }
  else if (sent == "DELETE")
  {
    sent = KEY_DELETE;
  }
  else if (sent == "END")
  {
    sent = KEY_END;
  }
  else if ((sent == "ESC") || (sent == "ESCAPE"))
  {
    sent = KEY_ESC;
  }
  else if (sent == "HOME")
  {
    sent = KEY_HOME;
  }
  else if (sent == "INSERT")
  {
    sent = KEY_INSERT;
  }
  else if (sent == "NUMLOCK")
  {
    sent = KEY_NUMLOCK;
  }
  else if (sent == "PAGEUP")
  {
    sent = KEY_PAGE_UP;
  }
  else if (sent == "PAGEDOWN")
  {
    sent = KEY_PAGE_DOWN;
  }
  else if (sent == "PRINTSCREEN")
  {
    sent = KEY_PRINTSCREEN;
  }
  else if (sent == "SCROLLOCK")
  {
    sent = KEY_SCROLLOCK;
  }
  else if (sent == "SPACE")
  {
    sent = KEY_SPACE;
  }
  else if (sent == "TAB")
  {
    sent = KEY_TAB;
  }
  else if ((sent == "DOWNARROW") || (sent == "DOWN"))
  {
    sent = KEY_DOWN_ARROW;
  }
  else if ((sent == "LEFTARROW") || (sent == "LEFT"))
  {
    sent = KEY_LEFT_ARROW;
  }
  else if ((sent == "RIGHTARROW") || (sent == "RIGHT"))
  {
    sent = KEY_RIGHT_ARROW;
  }
  else if ((sent == "UPARROW") || (sent == "UP"))
  {
    sent = KEY_UP_ARROW;
  }

  //devolvemos sent, tanto si la hemos modificado como si no :)
  return sent;
}


///////////////
//iluminacion//
///////////////
void iluminacion(int m)
{

  //configuraci�n - Estado inicial, si nada se ejecuta, se mantiene así.
  //respiraci�n
  if (m == 0){
    digitalWrite(led, LOW);
    for (int i=0; i<smoothness_pts; i++){
     float pwm_val = 255.0*sqrt(1.0 -  pow(abs((2.0*(i/smoothness_pts))-1.0),2.0));
     analogWrite(LED_BUILTIN,int(pwm_val));
     delay(5);
    }
  }

  //armado  - ejecución completada o preparado para ejecutar
  //led fijo
  if (m == 1){
    digitalWrite(LED_BUILTIN, LOW); //apagamos el led de configuraci�n
    digitalWrite(led, HIGH);        //encendemos el led
  }

  //ejecutando - mientras se ejecuta el script
  //blink
  if (m == 2){
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(led,HIGH);
    delay(50);
    digitalWrite(led,LOW);
    delay(50);
  }

  //Error
  //alternancia entre leds
  if (m == -1){
    digitalWrite(LED_BUILTIN,LOW);
    digitalWrite(led, HIGH);
    delay (500);
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(led, LOW);
    delay(500);
  }

}



/***********************************************
* LOOP                                         *
************************************************/

void loop() {
  // put your main code here, to run repeatedly:

  iluminacion(mode);                 //iluminación para obtener información del resultado de la ejecución

}

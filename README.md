# patitodegoma
another rubber ducky for atmega32u4

"patitodegoma v0.1" ha sido creado por @ericmelvin10 (Adrian Aceituno) - https://github.com/ericmelvin10/patitodegoma 

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


Se ha realizado un workarround para poder utilizar 2 teclas modificadoras que supone escribir estas combinaciones de forma especial en el payload.
CTRL-SHIFT
GUI-SHIFT
CTRL-GUI
ALT-GUI


He utilizado el siguiente hardware:
https://es.aliexpress.com/item/1005002928161736.html?spm=a2g0o.search0304.0.0.598f5ab4HTKoQq&algo_pvid=407746d5-00cd-43a1-a41d-55279969b1f5&algo_exp_id=407746d5-00cd-43a1-a41d-55279969b1f5-14

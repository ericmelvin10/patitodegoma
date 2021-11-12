# patitodegoma
another rubber ducky for atmega32u4


Funcionamiento básico de "patitodegoma":
Sin tarjeta SD no hace nada. Se entiende que está en modo "configuración" y el led mostrará un comportamiento de "respiración".
Con tarjeta SD ejecutará el payload que está dentro. Cuando esté preparado para actuar o "armado" el led permanecerá fijo.
Mientras se está ejecutando el payload, el led parpadeará y cuando acabe volverá al color fijo.
El payload se ejecutará una sola vez, cada vez que se conecte la placa por usb siempre que tenga una tarjeta SD con un payload válido.
Si hay un error, los 2 leds se encenderan alternativamente.


Este proyecto se basa en el trabajo de otras personas:

- https://github.com/mharjac/bad_ducky
- https://github.com/basic4/USB-Rubber-Ducky-Clone-using-Arduino-Leonardo-Beetle
- https://github.com/jerwuqu/ardoducky
- https://github.com/Screetsec/Pateensy
- https://github.com/Seytonic/Duckduino-microSD
- https://malduino.com/


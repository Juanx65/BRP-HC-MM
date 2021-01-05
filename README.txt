BRP usando HC con MM
04/01/2021

Autor: Juan Aguilera Castillo

Preliminares:

Este proyecto se elaboro en un entorno UNIX mediante WSL2 en el editor VS Code.

Se subió este trabajo a github: https://github.com/Juanx65/BRP-HC-MM


Instrucciones de ejecución:

Para compilar el programa simplemente utlice el siguiente comando en el bash

  make

Para ejecutar el programa debe ejecutar en bash:

  time ./main config_H_W.txt

Donde _H_ y _W_ representan las W columnas y H  tiers de el archivo de prueba a utilizar, como se aprecia en este ejemplo usando el archivo _config_3_3.txt_

  time ./main config_3_3.txt


Para interpretar el resultado obtendio mediante este programa, la solución entregada viene dada por un vector de vectores que se imprimira en el bash de la siguiente forma:

        [
          [n i j k l]
        ]

Donde:

    n numero de extracción del bloque.
    i columna de origen del bloque.
    j tier de origen del bloque.
    k columna de llegada del bloque, con -1 como fuera de la bahía.
    j tier de llegada del bloque, donde la extracción fuera del patio se representa por−1

Un ejemplo de solución se veria de la sigueinte manera:

    [
      [ 1  2  1  -1  -1 ]
      [ 2  0  1  -1  -1 ]
      [ 6  1  0  0  1 ]
      [ 3  1  1  -1  -1 ]
      [ 4  1  2  -1  -1 ]
      [ 5  2  2  -1  -1 ]
      [ 6  0  1  -1  -1 ]
      [ 7  0  2  -1  -1 ]
    ]

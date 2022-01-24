# Informatica-Grafica-IG



| UNIVERSIDAD DE GRANADA | Doble Grado de Ingeniería Informática y Matemáticas |
| ---------------------- | --------------------------------------------------- |
| Curso 2021-2022        | 4º curso                                            |
| Asignatura             | Informática Gráfica                                 |



##### Archivos

- **recursos**: esta carpeta contiene archivos de código fuente, imágenes, modelos 3D (archivos .ply) y otros, que se deben usar tal cual se entregan, sin que se deban modificar. Tiene estas sub-carpetas:	
  - **src**: archivos de código fuente C++ (**.cpp**)
  - **include**: archivos de cabecera C++ (**.h**)
  - **shaders**: archivos de código fuente GLSL (**.gsl**)
  - **plys**: archivos con modelos 3D en formato PLY (**.ply**)
  - **imgs**: archivos de imágenes para texturas en formato JPEG (**.jpg**/**.jpeg**)
  - **make**: varios archivos con información de configuración para el proceso de compilación.
- **trabajo**: esta carpeta contiene el código fuente sobre el que se han realizado las prácticas.
  - **src**: archivos de código fuente C++ completado (**.cpp**)
  - **include**: archivos de cabecera C++  completado (**.h**)
  - **plys**: archivos con modelos 3D en formato PLY  nuevos (**.ply**)
  - **imgs**: archivos de imágenes para texturas en formato JPEG (**.jpg**)
  - **make-unix** archivos para compilar la aplicación usando `make` en sistemas operativos basados en Unix.
  - **cmake-unix** archivos para compilar la aplicación usando `cmake` en sistemas operativos basados en Unix.

 

##### Prerequisitos software

Estas prácticas han sido realizadas en LINUX. 

- Tener instalado **g++**
- **Librería Glew**      `sudo apt install libglew-dev`
- **Librería GLFW **     `sudo apt install libglfw3-dev`
- **Librería JPEG**     `sudo apt install libjpeg-dev`



### Compilación y ejecución

##### CMake

1. Acceder a *trabajo/cmake-unix/build*
2. Ejecutar `cmake ..` .  Si no hay errores, esto generará en la carpeta build diversas sub-carpetas y archivos, entre ellos el archivo Makefile . Si se quiere volver a generar todo, bastaría con vaciar la carpeta *build* y de nuevo hacer `cmake ..`
3. Cada vez que se quiera compilar la carpeta *build* hay que hacer `make`. Si no hay errores, esta orden debe de generar el archivo ejecutable ***pracs_ig_exe*** en la carpeta *build*. Para eliminar los archivos generados al compilar y forzar que se vuelva a compilar todo, ejecutar `make clean`
4. Cada vez que se quiera ejecutar el programa hay que hace `cd` a la carpeta de trabajo y ejecutar `cmake-unix/build/pracs_ig_exe`



##### Make

1. Acceder a *trabajo/make-unix*
2. Ejecutar `make`.  Si no hay errores, esta orden debe de generar el archivo ejecutable ***pracs_ig_exe*** en la subcarpeta *bin* (junto con todos los archivos *.o*, en la sub-carpeta *objs*), después se ejecuta el programa.
3.  Para eliminar todos los archivos generados al compilar se puede ejecutar `make clean` y luego de nuevo `make`.



### Descripción 

Prácticas enfocadas en aprender en el uso de OpenGL:

1. Modelos PLY y poligonales (incluido modelos por revolución)
2. Modelos jerárquicos y grados de libertad para crear una escena con movimiento.
3. Materiales, fuentes de luz y texturas.
4. Interacción con la escena: selección y uso de cámaras.

Todo esto se consigue a partir de una base de modelos simples de mallas. La práctica está programada para ser visualizada por primitiva de triángulos 

> en caso de cambiar la primitiva todos los modelos e incluso algunos métodos deberían de ser modificados

En el desarrollo de esta práctica se propone crear un  modelo jerárquico propio que contenga varios grados de libertad. En el caso de este repositorio el modelo creado es un **dron** que *carga y descarga mercancía de una caja* (hélices, movimiento del dron, apertura y cierre de compuertas y subida y bajada de la carga). La imagen del dron:

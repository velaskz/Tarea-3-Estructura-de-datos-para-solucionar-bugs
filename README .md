INTEGRANTES:
  ● Benjamin Velasquez, Aporte: 100/100.
  ● Benjamin Rodriguez, Aporte: 100/100.
  ● Sean Jamen        , Aporte: 100/100.


CORRECTO FUNCIONAMIENTO:
● La función 1 recorre perfectamente los libros ingresados, almacena las palabras, cantidad de lineas, cantidad de caracteres y  mapas especificados.

● La función 7 busca por palabras efectivamente dentro del contenido del libros especificado

PROBLEMAS:

● Al ingresar las palabras en el mapa conteodepalabras(funcion Addline) la funcion aumenta la frecuencia de la palabra solamente si repite en esa linea.

● El arbol binario en la funcion 2, no se muestran por pantalla todos los libros, muestra algunos pero los muestra ordenados, creemos que es un bug de la estructura de los mapas ordenados.

● La función 3 muestra los libros que no contienen la palabra y por problemas del mapa de palabras no puede mostrar correctamente los datos.

● La función 4 no reacciona al momento de probarla con muchos libros en el sistema y por problemas del mapa de palabras no puede mostrar correctamente los datos.

● La función 5 no esta mostrando los datos correctos, pero si recorre todo lo necesario y por problemas del mapa de palabras no puede mostrar correctamente los datos.

● La función 6 entra en un bucle que calcula de mala manera la relevancia de las palabras porque creemos que lower_than_int no funciona bien con el codigo y por problemas del mapa de palabras no puede mostrar correctamente los datos.


DESCRIPCION DE FUNCIONES:
1.Cargar documentos:
  ● El usuario proporciona una lista de nombres de archivos txt separados por
  espacios.
  ● Para cada archivo, se crea una instancia de la estructura de libro y se lee su
  contenido.
  ● Se calcula la cantidad de palabras, caracteres y lineas del libro.
  ● Se inserta el libro en el mapa de libros usando su titulo como clave.
  ● Se inserta el libro en el mapa de libros usando su ID como clave.
  
2. Mostrar documentos ordenados:
  ● Se recorren todos los libros en el mapa de libros y se almacenan en un mapa ordenado.
  ● Se muestra la información de cada libro (ID, título, cantidad de palabras y
  caracteres).
  
3. Buscar libro por titulo:
 ● El usuario proporciona palabras.
 ● Se buscan en el mapa de libros, los titulos relacionados a dichas palabras.
 ● se muestran por pantallas dichos titulos asociados a las palabras.
 
4.Palabras con mayor frecuencia:
  ● El usuario ingresa el ID de un libro.
  ● Se obtiene el contenido del libro.
  ● Se realiza un análisis para calcular la frecuencia de cada palabra en el libro.
  ● Se muestran las 10 palabras que se repiten con mayor frecuencia y la
  cantidad de veces que aparecen.
  
5.Palabras más relevantes:
   ● El usuario ingresa el titulo de un libro.
   ● Se obtiene de este las 10 palabras más relevantes de este y se muestran por pantalla.
   
6. Buscar por palabra:
  ● El usuario ingresa una palabra.
  ● Se recorren todos los libros en el mapa de libros y se calcula la relevancia de
  la palabra en cada libro.
  ● Los libros se ordenan por relevancia (de mayor a menor).
  ● Se muestran los libros (ID y título) que contienen la palabra en su contenido,
  ordenados por relevancia
  
7.Mostrar palabra en su contexto dentro del libro:
  ● El usuario ingresa el titulo de un libro y una palabra a buscar.
  ● se muestra por pantalla cada una de las apariciones de la palabra en el contexto del libro (con 5 palabras hacia atras y delante).
   
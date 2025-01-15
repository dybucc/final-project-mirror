# Proyecto final: _minesweeper_

Proyecto final de primer cuatrimestre para la asignatura de informática del
grado de ingeniería multimedia en la ETSE, Valencia.

Se ha desarrollado una versión simplificada del juego _minesweeper_. Para ello,
se han implementado instrucciones proveídas como parte del proyecto, en que se
detallaba el diseño a seguir.

---

## Detalles técnicos sobre la administración del proyecto

### Versionado

Para el versionado del proyecto, se ha decidido seguir el modelo [SemVer]. Se ha
considerado la versión inicial de desarrollo, i.e. 0.1.0, como aquella que
refleja todos los cambios hasta adaptar la primera iteración de los
requerimientos en las instrucciones. Cada commit con una nueva versión tiene un
tag asignado.

### Control de versiones

Debido a la falta de la sesión de control de versiones para la asignatura de ISU
por el evento metereológico de la DANA, se ha decidido seguir un modelo
particular de control de versiones.

Éste se trata de [Conventional Commits]. Esta especificación representa cambios
notables en el modo en que se reportan commits en el repositorio, puesto que
implica utilizar identificadores o elementos estructurales específicos para cada
tipo de cambio.

Relativo a los detalles, toda la información se puede encontrar en la página
previamente enlazada. En ésta, se reflejan un rango básico de mensajes
dependiendo del tipo de commit a realizar. Si se trata de un commit que añade
una nueva finalidad, se describe con `feat`, mientras que si se trata de la
corrección de un bug, se emplea `fix`. A ello sigue un scope o ámbito opcional
entre paréntesis, que es particular a cada proyecto.

Para este proyecto, se decidió hacer uso de cuatro ámbitos diferentes para
cada commit:

- `init`
  + Concierne cualquier función relativa a la inicialización del juego. Este tipo de implementación refiere a las funciones de inicialización al principio de cada partida.
- `juego`
  + Concierne toda función relativa al procesamiento del juego, es decir, todas aquellas partes en las que participa el usuario durante el bucle de lógica principal. Esto incluye el procesamiento final, pero sin las funciones de guardado.
- `fin`
  + El resto de funciones relacionadas con el procesamiento realizado al final del juego, i.e. procesamiento de archivos guardados.
- `git`
  + Concierne cualquier tipo de operación sobre el VCS utilizado; en este caso, Git. Una excepción es el commit inicial, que a continuación se explica con mayor detalle.

Sobre el commit inicial, el hecho de que se ha modificado el registrado
originalmente por el profesorado ha sido para mejor reflejar el modelo seguido
(Conventional Commits.) A pesar de que en la página de la especificación oficial
no se mencionan tipos específicos, sí que se comenta la posibilidad de emplear
otros elementos estructurales adicionales a `fix` y `feat`.

Debido a ello, se ha considerado hacer uso de los elementos especificados en
este [GitHub Gist]. En esta ampliación de la especificación, se mencionan
posibles elementos estructurales y sus usos. Para el presente proyecto, se han
empleado los siguientes:

- `feat`
  + Mencionado con anterioridad.
- `fix`
  + Íbid.
- `refactor`
  + Concerniendo modificaciones sobre el proyecto que reescriben código.
- `style`
  + Concerniendo modificaciones puramente visuales pero no funcionales, e.g. erratas.
- `docs`
  + Concerniendo modificaciones y creaciones de documentación.
- `build`
  + Concerniendo el aumento del versionado del proyecto en la sección [Changelog], y la automacion de build jobs para la memoria.
- `chore`
  + Concerniendo cualquier tipo de tarea trivial relacionada con el controlador de versiones, e.g. excluir archivos binarios.

En esta especificación, se menciona un tipo de commit específico para el commit
inicial, que se ha seguido. Cabe mencionar que la modificación de este commit
solo ha implicado una modificación de sus atributos, siguiendo el modelo de
coautoría de GitHub y GitLab; el resto del patch se ha mantenido intacto.

Finalmente, cabe mencionar que los commits a los que no se les ha adjuntado un
ámbito son triviales, i.e. commits de tipo `style`; o son commits que no pueden
considerarse parte de ningún ámbito específico, e.g. un `feat` para implementar
tipos.

---

## Changelog

### 0.3.21

- refactor(init): modifica procesamiento auxiliar

### 0.3.20

- refactor(juego): elimina valor innecesario

### 0.3.19

- refactor(juego): elimina limpieza final terminal
- sytle: modifica orden de saltos de linea

### 0.3.18

- refactor(juego): condiciona (des)marcado de celdas

### 0.3.17

- fix(juego): modifica suma de intentos
- style: cambia orden de funciones

### 0.3.16

- refactor(init): modifica nombre de inicializacion

### 0.3.15

- refactor(juego): aclara parametros de funcion menu

### 0.3.14

- refactor(fin): cambia tipo de tamano de intentos

### 0.3.13

- refactor: hace constante variable global
- style: reordena headers por orden alfabetico

### 0.3.12

- refactor(fin): modifica escritura de jugadores

### 0.3.11

- refactor(fin): elimina bucle innecesario

### 0.3.10

- refactor(fin): modifica lectura nombre jugador

### 0.3.9

- fix(fin): corrige lectura de datos desde archivo

### 0.3.8

- fix(fin): corrige rango de nombres de jugador
- style: modifica nombre de un registro de partida

### 0.3.7

- refactor(juego): modifica condicion de procesado

### 0.3.6

- fix(juego): corrige inicializacion necesaria

### 0.3.5

- refactor(juego): elimina inicializacion variable

### 0.3.4

- fix(juego): corrige exploracion de celdas vecinas

### 0.3.3

- fix(init): corrige numero de minas finales

### 0.3.2

- fix(init): corrige posiciones de minas aleatorias

### 0.3.1

- fix(init): corrige numero de minas aleatorias
- style: corrige declaracion para seguir guia

### 0.3.0

- feat(juego): refina deteccion de minas vecinas

### 0.2.1

- refactor(juego): modifica presentacion del tablero

### 0.2.0

- feat(juego): modifica indice para procesar celdas

### 0.1.1

- fix(init): corrige inicializacion de minas
- style: corrige errata en opcion de menu

### 0.1.0

- Versión inicial de desarrollo

---

## Licencia

Licenciado bajo _The Unlicense_. Para mas información, ver el archivo de
licencia.

<!-- Links -->

[semver]: https://semver.org/
[conventional commits]: https://www.conventionalcommits.org/en/v1.0.0/
[github gist]: https://gist.github.com/qoomon/5dfcdf8eec66a051ecd85625518cfd13
[changelog]: #changelog

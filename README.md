# Caravana del Desierto

Trabajo Práctico de Algoritmos y Estructuras de Datos - UNLaM
Comision 02-3300 - Ciclo 1C 2026

## Descripción

Juego de mesa por consola en el que un jugador (J) debe recorrer una ruta
desértica desde el Campamento Inicial (I) hasta la Ciudad Refugio (S),
evitando ser interceptado por Bandidos (B) y aprovechando los recursos
distribuidos a lo largo del camino: Premios (P), Vidas extra (V), Oasis (O)
y Tormentas de arena (T).

## Integrantes del Grupo 'Tortuga'

| Apellido y Nombre | DNI |
|---|---|
| Luna Gauna, Gonzalo Thiago | 45738676 |
| Mascasin Muñoz, Maximiano | 41150839 |
| Zacarias, Franco Hernan | 46422064 |

## Estructura del proyecto

```
CaravanaDelDesierto/
├── TDA/                  TDAs genéricos reutilizados de clase
│   ├── lista.h / lista.c     Lista circular doblemente enlazada
│   ├── cola.h  / cola.c      Cola dinámica
│   └── arbol.h / arbol.c     Árbol binario de búsqueda
│
├── Juego/                Lógica específica del juego
│   ├── tipos.h               Structs y constantes compartidas
│   ├── archivos.h / .c       Lectura/escritura de config, caravana, jugadores y partidas
│   ├── tablero.h  / .c       Generación y visualización del tablero
│   ├── juego.h    / .c       Lógica de turnos, movimientos y efectos
│   └── ranking.h  / .c       Índice de jugadores (BST) y ranking
│
├── Archivos/             Archivos de datos generados/leídos en runtime
│   ├── config.txt            Parámetros de configuración (editable)
│   ├── caravana.txt          Tablero generado al iniciar una partida
│   ├── jugadores.dat         Registro de jugadores
│   ├── indice.dat             Índice (BST) de jugadores
│   └── partidas.dat            Historial de partidas jugadas
│
├── Registros/              Registro de Jugadores
│   ├── registros.h / .c       Manejo de Indices
│
└── main.c                Punto de entrada y menú principal
```

## Configuración (`Archivos/config.txt`)

Antes de jugar se puede editar `Archivos/config.txt` para ajustar el tablero:

```
cantidad_posiciones:25
vidas_inicio:3
maximo_bandidos:2
maximo_premios:3
maximo_vidas_extra:1
maximo_oasis:2
maximo_tormentas:3
```

| Parámetro | Descripción |
|---|---|
| `cantidad_posiciones` | Número total de casilleros del tablero |
| `vidas_inicio` | Vidas con las que arranca el jugador |
| `maximo_bandidos` | Cantidad de bandidos en el tablero |
| `maximo_premios` | Cantidad de premios (1 punto cada uno) |
| `maximo_vidas_extra` | Cantidad de vidas extra distribuidas |
| `maximo_oasis` | Cantidad de oasis (protección de 1 turno) |
| `maximo_tormentas` | Cantidad de tormentas (pierde 1 turno) |

## Cómo jugar

1. Compilar el proyecto
2. Ejecutar el `.exe` generado
3. Ingresar el nombre de jugador (si ya jugó antes, se cargan sus estadísticas)
4. Desde el menú principal:
   - **1.** Jugar una nueva partida
   - **2.** Ver el registro de jugadores
   - **3.** Ver el registro de partidas
   - **4.** Ver el ranking de jugadores
   - **5.** Salir

### Durante la partida

- Cada turno se lanza un dado virtual (1 a 6)
- El jugador elige moverse **adelante (A)** o **atrás (S)**
- Si se sobrepasa la Ciudad Refugio, el jugador rebota y retrocede las
  posiciones sobrantes
- Los bandidos se mueven automáticamente acercándose al jugador por el
  camino más corto (la ruta es circular)
- Al finalizar la partida se muestra el historial de movimientos en
  formato `FX` (Forward X casillas) / `BX` (Backward X casillas)

## Representación del tablero (`caravana.txt`)

```
01:[I J]
02:.
03:[V T]
...
16:[O J]
...
25:S
```

- Un solo elemento en la celda → se escribe el carácter directo (`P`, `O`, `T`, `B`, `S`, etc.)
- Más de un elemento en la misma celda → se listan entre corchetes,
  separados por espacios (ej: `[O B J]`)
- Celda vacía → `.`

## Estructuras de datos utilizadas

| Requerimiento del TP | TDA implementado |
|---|---|
| Ruta del desierto (tablero) | Lista Circular Doblemente Enlazada |
| Cola de movimientos del jugador y bandidos | Cola Dinámica |
| Índice de jugadores | Árbol Binario de Búsqueda (BST), persistido en `indice.dat` |

## Persistencia de datos

- **`jugadores.dat`**: registro principal de cada jugador (nombre, puntos
  totales, partidas jugadas)
- **`indice.dat`**: índice del BST (nombre + offset en `jugadores.dat`),
  permite reconstruir el árbol de forma balanceada al iniciar el juego
- **`partidas.dat`**: historial de todas las partidas jugadas (puntos,
  cantidad de movimientos, resultado)

## Funcionalidades agregadas

- La pantalla se limpia automáticamente entre turnos
- El jugador debe presionar Enter para avanzar después de ver los
  resultados de su turno (efectos de celda y colisiones con bandidos)
- Mensaje informativo cuando el casillero no produce ningún efecto
- Historial de Partidas

# Juego de Ajedrez en C con ncurses

Repositorio completo del Proyecto Final - IE-0117 (II-2025)

## Contenido
- `src/` - código fuente (modularizado)
  - `main.c` - punto de entrada
  - `board.c/.h` - representación del tablero
  - `engine.c/.h` - reglas y validación de movimientos
  - `ui.c/.h` - interfaz ncurses
- `latex/` - informe y presentación en LaTeX
- `Makefile` - comandos para compilar y limpiar
- `screenshots/` - capturas (vacío por defecto)

## Requisitos
- `gcc`
- `make`
- `libncurses5-dev` / `libncursesw5-dev`

En Debian/Ubuntu:
```bash
sudo apt update
sudo apt install build-essential libncurses5-dev libncursesw5-dev texlive-latex-recommended texlive-latex-extra texlive-fonts-recommended
```

## Compilación y ejecución
Desde la raíz del repositorio:
```bash
make
./bin/ajedrez_ncurses
```

Para limpiar:
```bash
make clean
```

## Cómo contribuir / estructura
- Hacer `feature branches` y `pull requests`.
- Documentar cambios en el `README.md`.
- Issues para bugs y mejoras.

## Autor
Emily Maryan Flores Rojas

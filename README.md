# cub3D

Proyecto de raycasting basado en MiniLibX. Permite cargar un mapa `.cub` con
texturas, colores y una rejilla de celdas para renderizar un escenario 3D.

## Requisitos

- GNU Make
- `cc` (clang o gcc)
- Dependencias del sistema para X11 (MiniLibX en Linux)

## Compilación

```bash
make
```

> Nota: si MiniLibX no compila en tu entorno, revisa los requisitos de X11
> (libx11-dev, libxext-dev, etc.).

## Uso

```bash
./cub3D assets/maps/good/other/subject_map.cub
```

Si no se pasa un mapa válido, el programa mostrará el mensaje de uso:

```
Usage: ./cub3d <map.cub>
```

## Mapas de ejemplo

Los mapas de prueba se encuentran en `assets/maps/good` y `assets/maps/bad`.

## Limpiar

```bash
make clean   # borra objetos
make fclean  # borra binario y logs
```

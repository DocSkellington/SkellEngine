Objects   {#map_objects}
====
[TOC]

Objects are part of an _object layer_ in a Tiled Map. They can be used as graphical or logical/physical entities.

Tiles in an object layer are not supported.

# Graphical
## Shapes
Rectangles, ellipses (and circles), polylines and convex polygons are supported.

Concave shapes are not correctly supported. In fact, outlines are correctly rendered but not the interior of the shape. The next image is an example of a concave shape.

![Example of a concave shape](concaveShape.png "Concave Shape")

### Supported Custom Properties
The following properties can be defined for a shape:

  * `fill` defines the fill colour of the shape
  * `outline` defines the colour of the outline
  * `thickness` defines the thickness of the outline

## Text
Text is in development.
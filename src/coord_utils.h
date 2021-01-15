//
// Created by brych on 15/01/2021.
//

#ifndef _RTS_COORD_UTILS_H
#define _RTS_COORD_UTILS_H

#define RECT_COORDINATES(rect) rect.tl.x, rect.tl.y, rect.br.x, rect.br.y

typedef struct {
    float x; float y;
} Point;

typedef struct {
    Point tl;
    Point br;
} Rect;

#endif //_RTS_COORD_UTILS_H

#ifndef WM_TYPES_H
#define WM_TYPES_H

#define OFF 0
#define ON 1

typedef struct _POINT
{
	int x, y;
} ATOMPOINT, *PATOMPOINT;

typedef struct _RECT
{
	int x, y, width, height;
} ATOMRECT, *PATOMRECT;

#endif

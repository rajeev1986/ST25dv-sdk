#ifndef _MENU_CURSOR_H_
#define _MENU_CURSOR_H_

#if !defined(__MENU_CONFIG_H__)
  #error Please include this file after including Menu_config.h
#endif

typedef enum {
  MENU_BAR_VERTICAL = 0,
  MENU_BAR_HORIZONTAL
} Menu_Bar_Direction_t;

typedef struct {
  ColorStyles_t Style;
  uint32_t X;
  uint32_t Y;
  uint32_t Length;
  Menu_Bar_Direction_t Direction;
} Menu_Bar_t;

struct Menu_Cursor_t;
typedef void (*Menu_CursorCallback_t)(struct Menu_Cursor_t*) ;

typedef struct Menu_Cursor_t {
  Menu_Bar_t *Bar;
  uint32_t Value;
  uint32_t Max;
  uint32_t Min;
  const char *Picture;
  uint32_t Width;
  uint32_t Height;
  uint32_t X;
  uint32_t Y;
  uint32_t minX;
  uint32_t maxX;
  uint32_t minY;
  uint32_t maxY;
  Menu_CursorCallback_t Callback;
} Menu_Cursor_t;

Menu_Status_t Menu_CursorInit(Menu_Bar_t *bar, uint32_t minValue, uint32_t maxValue, uint32_t defaultValue, const char* picture, uint32_t margin, Menu_CursorCallback_t cb, Menu_Cursor_t* cursor);
Menu_Status_t Menu_BarInit(uint32_t X, uint32_t Y, uint32_t Length, Menu_Bar_Direction_t direction, ColorStyles_t style, Menu_Bar_t *bar);
Menu_Status_t Menu_CursorSetValue(Menu_Cursor_t *cursor,uint32_t value);
uint8_t Menu_CursorInput(Menu_Cursor_t *cursor, Menu_Position_t *touch);

#endif

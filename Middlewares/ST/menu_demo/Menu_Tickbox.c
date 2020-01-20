#include "Menu_Plot.h"
#include "Menu_Tickbox.h"

void Menu_DisplayTickbox(Menu_Tickbox_t *t)
{
  Menu_Plot_Point_t a,b,b2,c;
  uint32_t tick_incr = t->size /4;
  a.x = t->x + tick_incr;
  a.y = t->y + tick_incr;
  b.x = t->x + 2*tick_incr + 1;
  b.y = t->y + 3*tick_incr - 1;
  b2.x = b.x ;
  b2.y = b.y + 1;
  c.x = t->x + 5*tick_incr;
  c.y = t->y;

  if(!t->state)
  {
    Menu_SetStyle(CLEAR_PLAIN);
    Menu_DisplaySegment(a,b);
    Menu_DisplaySegment(b2,c);
  }

  Menu_SetStyle(PLAIN);

  Menu_FillRectangle(t->x,t->y,t->size,2);
  Menu_FillRectangle(t->x,t->y+t->size,t->size,2);
  Menu_FillRectangle(t->x,t->y,3,t->size);
  Menu_FillRectangle(t->x+t->size,t->y,3,t->size+2);
/*
  Menu_DisplayStringAt(t->x + t->size + Menu_GetFontWidth(),
                       t->y + ((int)t->size - Menu_GetFontHeight())/2,
                       t->str);
*/
  if(t->state)
  {
    Menu_SetStyle(TICKBOX_CHECK);
    Menu_DisplaySegment(a,b);
    Menu_DisplaySegment(b2,c);
  }
}

void Menu_TickboxInput(Menu_Tickbox_t* t, Menu_Position_t* touch)
{
  uint32_t tick_incr = t->size /4;  
  if((touch->X > (t->x - tick_incr)) && (touch->X < (t->x + t->size + tick_incr)) &&
    (touch->Y > (t->y - tick_incr)) && (touch->Y < (t->y + t->size + tick_incr)))
  {
     t->state = !t->state;
     Menu_DisplayTickbox(t);
  }
}

void Menu_RefreshTickbox(Menu_Tickbox_t* t)
{
  Menu_DisplayTickbox(t);
  Menu_SetStyle(PLAIN);
  Menu_DisplayStringAt(t->x + t->size + Menu_GetFontWidth(),
                       t->y + ((int)t->size - Menu_GetFontHeight()),
                       t->str);

}

Menu_Tickbox_t Menu_InitTickbox(uint32_t x, uint32_t y, uint32_t size, uint8_t state, char *str)
{
  Menu_Tickbox_t t;
  t.x = x;
  t.y = y;
  t.size = size;
  t.state = state;
  t.str = str;

  Menu_RefreshTickbox(&t);

  return t;
}


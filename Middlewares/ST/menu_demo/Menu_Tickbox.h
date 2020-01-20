#ifndef _MENU_TICKBOX_H_
#define _MENU_TICKBOX_H_
#include "Menu_config.h"

typedef struct {
uint32_t x;
uint32_t y;
uint32_t size;
uint8_t state;
char *str;
} Menu_Tickbox_t;

void Menu_DisplayTickbox(Menu_Tickbox_t *t);
Menu_Tickbox_t Menu_InitTickbox(uint32_t x, uint32_t y, uint32_t size, uint8_t state, char *str);
void Menu_TickboxInput(Menu_Tickbox_t* t, Menu_Position_t* touch);
void Menu_RefreshTickbox(Menu_Tickbox_t* t);


#endif

#ifndef _MENU_TAB_H_
#define _MENU_TAB_H_

#include "stdint.h"
#include "Menu_config.h"

typedef struct {
char* tabName;
Menu_Callback_t firstCb;
Menu_Callback_t tabCb;
} Menu_Tab_Setup_t;

typedef struct {
  Menu_Tab_Setup_t *setup;
  uint32_t nbTab;
  uint32_t x;
  uint32_t y;
  uint32_t width;
  uint32_t height;
  uint32_t tabRound;
  uint32_t tabHeight;
  uint32_t tabWidth;
  uint32_t current;
} Menu_Tab_t;

void Menu_TabLoop(Menu_Tab_Setup_t* setup, uint32_t nbTab);

#endif

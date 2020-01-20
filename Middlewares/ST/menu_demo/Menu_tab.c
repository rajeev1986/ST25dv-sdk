#include "Menu_tab.h"
#include "string.h"

void Menu_DisplayTabName(Menu_Tab_t* tab, uint32_t tabId)
{
    uint32_t maxChar = tab->tabWidth / Menu_GetFontWidth();
    char name[30] = {'\0'};
    uint32_t nameLength = strlen(tab->setup[tabId].tabName);
    nameLength = (nameLength > maxChar)?  maxChar : nameLength;
    uint32_t xName = tab->x + tabId * tab->tabWidth + ((tab->tabWidth - (nameLength * Menu_GetFontWidth()))/2);

    memcpy(name,tab->setup[tabId].tabName, nameLength);
    Menu_DisplayStringAt(xName, tab->y + tab->tabRound/2,name);

}

void Menu_TabLoop(Menu_Tab_Setup_t* setup, uint32_t nbTab)
{
  Menu_Tab_t tab;
  tab.setup = setup;
  tab.nbTab = nbTab;
  tab.x = 10;
  tab.y = Menu_GetFontHeight() + 10;
  tab.width = Menu_GetDisplayWidth() - 2*tab.x;
  tab.height = Menu_GetDisplayHeight() - 2*tab.y;
  tab.current = 0;
  tab.tabRound = 8;
  tab.tabHeight = Menu_GetFontHeight() + tab.tabRound;
  tab.tabWidth = tab.width / tab.nbTab;

  Menu_Position_t touch;
  Menu_Callback_Status_t status = MENU_CALLBACK_CONTINUE;
  
  Menu_SetStyle(TAB);
  Menu_DrawTab(tab.x,tab.y,tab.width,tab.height,tab.tabRound,tab.tabHeight,0,tab.nbTab);
  Menu_SetStyle(PLAIN);
  Menu_DisplayTabName(&tab,0);
  Menu_SetStyle(GREY);
  for (int i = 1; i < tab.nbTab ; i ++)
    Menu_DisplayTabName(&tab,i);
  // run first callback for first tab
  if(setup[tab.current].firstCb != NULL)
    status = setup[tab.current].firstCb();

  while(status == MENU_CALLBACK_CONTINUE)
  {
    // run callback fn
    if(tab.setup[tab.current].tabCb != NULL)
      status = tab.setup[tab.current].tabCb();

    Menu_ReadPosition(&touch);
    if(touch.Sel)
    {
      if ((touch.Y < (tab.y + tab.tabHeight)) && (touch.X > tab.x))
      {
        uint32_t nxtTab = (touch.X - tab.x) / tab.tabWidth;
        if(nxtTab >= tab.nbTab)
        {
        	nxtTab = tab.nbTab - 1;
        }
        if(nxtTab != tab.current)
        {
          Menu_SetStyle(GREY);
          Menu_DisplayTabName(&tab,tab.current);
          Menu_SetStyle(CLEAR_TAB);
          Menu_DrawTabHeader(tab.x,tab.y,tab.width,tab.height,tab.tabRound,tab.tabHeight,tab.current,tab.nbTab);
          Menu_SetStyle(TAB);
          Menu_DrawTabHeader(tab.x,tab.y,tab.width,tab.height,tab.tabRound,tab.tabHeight,nxtTab,tab.nbTab);
          Menu_SetStyle(PLAIN);
          Menu_DisplayTabName(&tab,nxtTab);
          tab.current = nxtTab;
          if(setup[tab.current].firstCb != NULL)
            status = setup[tab.current].firstCb();
        }
      } else if (touch.Y > (tab.y + tab.height)) {
          return;
      }
    }
  }
}


#ifndef _MENU_PLOT_H_
#define _MENU_PLOT_H_

#include "stdint.h"
#include "Menu_config.h"

typedef struct {int x; int y;} Menu_Plot_Point_t;
typedef struct {Menu_Plot_Point_t *points; uint32_t length; ColorStyles_t style;} Menu_Plot_t ;

typedef struct {
  uint32_t x;
  uint32_t y;
  uint32_t width;
  uint32_t height;
  int maxVal;
  int minVal;
  int minX;
  int maxX;
  float xFactor;
  float yFactor;
} Menu_Plot_Geo_t;

Menu_Plot_Geo_t Menu_DisplayCenterPlot(uint32_t width, uint32_t height, Menu_Plot_t *plots, uint32_t nbPlots, uint32_t minRange);
void Menu_DisplaySegment(Menu_Plot_Point_t a, Menu_Plot_Point_t b);
uint8_t Menu_PlotInput(Menu_Plot_Geo_t *geo, Menu_Position_t *touch, Menu_Plot_Point_t *value);

#endif // _MENU_PLOT_H_

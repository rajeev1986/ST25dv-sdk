#include "Menu_plot.h"
#include "math.h"
#include "stdio.h"



static Menu_Plot_Point_t seg1 = {3 , 3 };
static Menu_Plot_Point_t seg2 = {-3 , 3};

static Menu_Plot_Point_t getPointPosition(Menu_Plot_Geo_t *geo, Menu_Plot_Point_t *p)
{
  Menu_Plot_Point_t p_pix;
  p_pix.x = (p->x - (int)geo->minX) / geo->xFactor;
  p_pix.x += geo->x;
  p_pix.y =  - (p->y - (int)geo->minVal) / geo->yFactor;
  p_pix.y += geo->y + geo->height;
  // compute 
//  p_pix.x = geo->x  + ((plots->points[p].x * (int)geo->width) / Xrange) /*- (Menu_GetFontWidth() / 2)*/;
//  p_pix.y = geo->y + (((plots->points[p].y - (int)geo->minVal) * (int)geo->height) / (int)Yrange) /*- (Menu_GetFontHeight() / 2)*/;
  return p_pix;
}

void Menu_DisplaySegment(Menu_Plot_Point_t a, Menu_Plot_Point_t b)
{
  float adj = a.y - b.y;
//  float opp = b.y - a.y;
  float opp = b.x - a.x;
  float hyp = sqrt(adj*adj + opp*opp);
  float cos = opp / hyp;
  float sin = adj / hyp;

  for(int i = 0 ; i < hyp ; i +=1)
  {
      Menu_FillRectangle(a.x + i * cos,a.y - i * sin,2,2);
  }

}

static Menu_Plot_Point_t Menu_AddPoints (Menu_Plot_Point_t* a, Menu_Plot_Point_t* b)
{
  Menu_Plot_Point_t result;
  result.x = a->x + b->x;
  result.y = a->y + b->y;
  return result;
}

static Menu_Plot_Point_t Menu_SubPoints (Menu_Plot_Point_t* a, Menu_Plot_Point_t* b)
{
  Menu_Plot_Point_t result;
  result.x = a->x - b->x;
  result.y = a->y - b->y;
  return result;
}

void Menu_DisplaySinglePoint(Menu_Plot_Point_t* p)
{
  Menu_DisplaySegment(Menu_SubPoints(p,&seg1), Menu_AddPoints(p,&seg1));
  Menu_DisplaySegment(Menu_AddPoints(p,&seg2), Menu_SubPoints(p,&seg2));
}

void Menu_DisplayPoints(Menu_Plot_Geo_t *geo, Menu_Plot_t *plots)
{
  Menu_Plot_Point_t p_pos, last_p_pos;
//  const char txt[] = "X";
  //uint32_t Xrange = geo->maxX - geo->minX;
  //uint32_t Yrange = geo->maxVal - geo->minVal;
  //Menu_SetStyle(PLOT_AXES);

  for(int pid = 0; pid < plots->length; pid++)
  {
    p_pos = getPointPosition(geo, &plots->points[pid]);
    //Menu_DisplayStringAt(x,y,txt);
    Menu_DisplaySinglePoint(&p_pos);
    if(pid > 0)
      Menu_DisplaySegment(last_p_pos, p_pos);
    last_p_pos = p_pos;
  }
}


void Menu_DisplayGrid(Menu_Plot_Geo_t *geo)
{
  float first_value = 5 *ceil((float)geo->minVal / 5);
  float value = first_value;
  uint32_t range = geo->maxVal - geo->minVal;
  // try to display 5 segments at round values
  float raw_division = (float)range / 25;
  float division = round(raw_division) * 5;
  uint32_t gridSpace = division / geo->yFactor ;
  uint32_t firstY = geo->y + geo->height + ((int)geo->minVal - first_value) / geo->yFactor ;

  if(division > 0)
  {
    Menu_SetStyle(PLOT_GRID);
    for(uint32_t y = firstY ; y > (geo->y); y = y - gridSpace)
    {
      char txt[20];
      if(value != 0)
      {
        Menu_FillRectangle(geo->x,y,geo->width,0);
      }
      if(y > (geo->y + Menu_GetFontHeight()))
      {
        sprintf(txt,"%0.0f",value);
        Menu_DisplayStringAt(geo->x, y - Menu_GetFontHeight(),txt);
      }
      value += division;
    }
  }

  Menu_SetStyle(PLOT_AXES);
  Menu_Plot_Point_t zero = {0,0};
  zero = getPointPosition(geo,&zero);
  // display vertical axe if on the screen
  if((geo->minX <= 0) && (geo->maxX >= 0))
  {
    Menu_Plot_Point_t end;
    end.x = zero.x;
    end.y = geo->y;

    Menu_DisplaySegment(Menu_AddPoints(&end,&seg1), end);
    Menu_DisplaySegment(Menu_AddPoints(&end,&seg2), end);
 //   Menu_DisplaySegment(zero,end);
    Menu_FillRectangle(zero.x,geo->y,2,geo->height);
  }
  if((geo->minVal <= 0) && (geo->maxVal >= 0))
//  if((first_value <= 0) && (value >= 0))
  {
    Menu_Plot_Point_t end;
    end.x = geo->x + geo->width;
    end.y = zero.y;
    Menu_DisplaySegment(Menu_SubPoints(&end,&seg1), end);
    Menu_DisplaySegment(Menu_AddPoints(&end,&seg2), end);
//    Menu_DisplaySegment(zero,end);
    Menu_FillRectangle(geo->x,zero.y,geo->width,1);
  }
}

Menu_Plot_Geo_t Menu_DisplayCenterPlot(uint32_t width, uint32_t height, Menu_Plot_t *plots, uint32_t nbPlots, uint32_t minRange)
{
  Menu_Plot_Geo_t geo;

  uint32_t dispHeight = Menu_GetDisplayHeight();
  uint32_t dispWidth = Menu_GetDisplayWidth();
  geo.width = width - 6; // -6 is for the points display which goes a bit out of the graph
  geo.height = height - 6;
  geo.x = (dispWidth - geo.width) / 2 ;
  geo.y = (dispHeight - geo.height) / 2 ;
  geo.maxVal = plots[0].points[0].y;
  geo.minVal = plots[0].points[0].y;
  geo.maxX = plots[0].points[0].x;
  geo.minX = plots[0].points[0].x;
  for(int i = 0; i < nbPlots; i++)
  {
    for(int j = 0; j < plots[i].length; j++)
    {
      if(geo.maxVal < plots[i].points[j].y)
        geo.maxVal = plots[i].points[j].y; 
      if(geo.minVal > plots[i].points[j].y)
        geo.minVal = plots[i].points[j].y; 
      if(geo.maxX < plots[i].points[j].x)
        geo.maxX = plots[i].points[j].x; 
      if(geo.minX > plots[i].points[j].x)
        geo.minX = plots[i].points[j].x; 
    }
  }
  // add range so that the curve doesn't touch the border of the graph

  if(geo.maxVal > 50)
  {
    geo.maxVal *= 1.10;
  } else if (geo.maxVal < -50)
  {
    geo.maxVal *= 0.9;
  } else {
    geo.maxVal += 5;
  }
  if(geo.minVal > 50)
  {
    geo.minVal *= 0.90;
  } else if (geo.minVal < -50)
  {
    geo.minVal *= 1.10;
  } else {
    geo.minVal -= 5;
  }  

  // check if minimum defined margin is correct
  if((geo.maxVal - geo.minVal) < minRange)
  {
    uint32_t margins = (minRange - (geo.maxVal - geo.minVal))/2;
    geo.maxVal += margins;
    geo.minVal -= margins;
  }
  geo.xFactor = ((float)(geo.maxX - geo.minX)) /geo.width;
  geo.yFactor = ((float)(geo.maxVal - geo.minVal)) /geo.height;

  // clear any points drawn outside the graphe
  Menu_SetStyle(CLEAR_PLAIN);
  Menu_FillRectangle(geo.x - 3,geo.y - 3,3,geo.height + 6);
    Menu_FillRectangle(geo.x - 3,geo.y - 3,geo.width + 6, 3);
  Menu_FillRectangle(geo.x + geo.width,geo.y - 3,4,geo.height + 6);
    Menu_FillRectangle(geo.x - 3,geo.y + geo.height,geo.width + 6, 4);

  Menu_SetStyle(GREY);
  Menu_FillRectangle(geo.x,geo.y,geo.width,geo.height);

  Menu_DisplayGrid(&geo);

  for(int k = 0; k < nbPlots; k++)
  {
    Menu_SetStyle(plots[k].style);
    Menu_DisplayPoints(&geo,&plots[k]);
  }
  return geo;
}

uint8_t Menu_PlotInput(Menu_Plot_Geo_t *geo, Menu_Position_t *touch, Menu_Plot_Point_t *value)
{
  uint8_t valid = 0;
  if(touch->Sel)
  {
    if((touch->X > geo->x) && (touch->X < (geo->x + geo->width)) && 
        (touch->Y > geo->y) && (touch->Y < (geo->y + geo->height)))
    {
      value->x = (int)geo->minX + geo->xFactor * (touch->X - geo->x);
      value->y = (int)geo->minVal + geo->yFactor * ((geo->y + geo->height) - touch->Y);
      valid = 1;
    }
  }
  return valid;
}


#include "Menu_config.h"
#include "Menu_cursor.h"

static void Menu_DisplayBar(Menu_Bar_t *bar);
static uint32_t Menu_CursorGetPosition(Menu_Cursor_t *cursor);
static void Menu_ClearCursor(Menu_Cursor_t *cursor);


/** @brief Initialize a Menu_Bar_t structure with provided inputs and draw it on the display
  * @param x Bar position on X-axis
  * @param y Bar position on Y-axis
  * @param length Bar length
  * @param direction Bar direction (Vertical/Horizontal)
  * @param style Bar style (color)
  * @param bar Pointer on the Menu_Bar_t instance to initialize
  * @retval MENU_OK Bar has been displayed
  * @retval MENU_ERROR Bar has not been displayed
  */
Menu_Status_t Menu_BarInit(uint32_t x, uint32_t y, uint32_t length, Menu_Bar_Direction_t direction, ColorStyles_t style, Menu_Bar_t *bar)
{
  bar->X = x;
  bar->Y = y;
  bar->Length = length;
  bar->Direction = direction;
  bar->Style = style;

  if((bar->X > Menu_GetDisplayWidth()) || (bar->Y > Menu_GetDisplayHeight()))
  {
    return MENU_ERROR;
  }
  if(bar->Direction == MENU_BAR_VERTICAL)
  {
    if((bar->Y + bar->Length) > Menu_GetDisplayHeight())
    {
      return MENU_ERROR;
    }
  } else {
    if((bar->X + bar->Length) > Menu_GetDisplayWidth())
    {
      return MENU_ERROR;
    }
  }

  Menu_DisplayBar(bar);
  return MENU_OK;
}

/** @brief Initialize a Menu_Cursor_t structure with provided inputs and draw it on the display
  * @param bar pointer on the Menu_Bar_t instance on which the cursor will move
  * @param minValue Cursor minimum value
  * @param maxValue Cursor maximum value
  * @param defaultValue Cursor initial value
  * @param picture Cursor picture
  * @param margin Margin used to detect a touch on the cursor
  * @param cb A callback function to be called when the cursor value change 
  * @param cursor Pointer on the Menu_Cursor_t instance to be initialized
  * @retval MENU_OK Cursor has been displayed
  * @retval MENU_ERROR Cursor has not been displayed
  */
Menu_Status_t Menu_CursorInit(Menu_Bar_t *bar, uint32_t minValue, uint32_t maxValue, uint32_t defaultValue, const char* picture, uint32_t margin, Menu_CursorCallback_t cb, Menu_Cursor_t* cursor)
{
  uint32_t displayWidth = Menu_GetDisplayWidth();
  uint32_t displayHeight = Menu_GetDisplayHeight();

  cursor->Bar = bar;
  cursor->Min = minValue;
  cursor->Max = maxValue;
  cursor->Value = defaultValue;
  cursor->Picture = picture;
  cursor->Callback = cb;
  Menu_GetPictureDim(cursor->Picture, &(cursor->Width),&(cursor->Height));


  if(cursor->Bar->Direction == MENU_BAR_VERTICAL)
  {
    if(cursor->Bar->X < (cursor->Width / 2))
    {
      return MENU_ERROR;
    }
    cursor->X = cursor->Bar->X - (cursor->Width / 2);
    cursor->Y = Menu_CursorGetPosition(cursor);
  } else {
    if(cursor->Bar->Y < (cursor->Height / 2))
    {
      return MENU_ERROR;
    }
    cursor->X = Menu_CursorGetPosition(cursor);
    cursor->Y = cursor->Bar->Y - (cursor->Height / 2);
  }

  if(cursor->Bar->Direction == MENU_BAR_VERTICAL)
  {
    cursor->minX = (cursor->Bar->X > margin) ? (cursor->Bar->X - margin) : 0;

    cursor->maxX = cursor->Bar->X + margin;
    cursor->maxX = (cursor->maxX > displayWidth) ? displayWidth : cursor->maxX;
    
    cursor->minY = (cursor->Bar->Y > margin) ? cursor->Bar->Y - margin : 0;

    cursor->maxY = cursor->Bar->Y + cursor->Bar->Length + margin;
    cursor->maxY = (cursor->maxY > displayHeight) ? displayHeight : cursor->maxY;
  } else {
    cursor->minX = (cursor->Bar->X > margin) ? cursor->Bar->X - margin : 0;

    cursor->maxX = cursor->Bar->X + cursor->Bar->Length + margin;
    cursor->maxX = (cursor->maxX > displayWidth) ? displayWidth : cursor->maxX;

    cursor->minY = (cursor->Y > margin) ? cursor->Y - margin : 0;

    cursor->maxY = cursor->Y + margin;
    cursor->maxY = (cursor->maxY > displayHeight) ? displayHeight : cursor->maxY;
  }

  Menu_DisplayPicture(cursor->X, cursor->Y, cursor->Picture);

  return MENU_OK;
}


/** @brief Set the value for a cursor, updating its position accordingly
  * @param cursor Pointer on the cursor to modify
  * @param value the value to be set
  * @retval MENU_OK The cursor has been updated
  * @retval MENU_ERROR The cursor has not been updated
  */
Menu_Status_t Menu_CursorSetValue(Menu_Cursor_t *cursor, uint32_t value)
{

  if((value < cursor->Min) || (value > cursor->Max))
  {
    return MENU_ERROR;
  }

  Menu_ClearCursor(cursor);
  cursor->Value = value;
  if(cursor->Bar->Direction == MENU_BAR_VERTICAL)
  {
    cursor->Y = Menu_CursorGetPosition(cursor);
  } else {
    cursor->X = Menu_CursorGetPosition(cursor);
  }
  Menu_DisplayPicture(cursor->X, cursor->Y, cursor->Picture);
  cursor->Callback(cursor);

  return MENU_OK;
}

/** @brief Check a touchscreen position to detect a change on the cursor, and update it accordingly
  * @param cursor Pointer on the cursor to check for an input
  * @param touch Pointer on a touchscreen position
  * @retval 0 the cursor has not changed
  * @retval 1 the cursor has changed
  */
uint8_t Menu_CursorInput(Menu_Cursor_t *cursor, Menu_Position_t *touch)
{
  if(touch->Sel)
  {
    if((touch->X > cursor->minX) && (touch->X < cursor->maxX) &&
        (touch->Y > cursor->minY) && (touch->Y < cursor->maxY))
    {
      // the user touched the cursor area: update the cursor accordingly
      Menu_ClearCursor(cursor);
      // update position & value
      if(cursor->Bar->Direction == MENU_BAR_VERTICAL)
      {
        if(touch->Y < cursor->Bar->Y)
          touch->Y = cursor->Bar->Y;
        if(touch->Y > (cursor->Bar->Y + cursor->Bar->Length))
          touch->Y = cursor->Bar->Y + cursor->Bar->Length;
        cursor->Y = touch->Y;
        cursor->Value = ((cursor->Max - cursor->Min) *(cursor->Y - cursor->Bar->Y)) / cursor->Bar->Length;
      } else {
        if(touch->X < cursor->Bar->X)
          touch->X = cursor->Bar->X;
        if(touch->X > (cursor->Bar->X + cursor->Bar->Length))
          touch->X = cursor->Bar->X + cursor->Bar->Length;
        cursor->X = touch->X;
        cursor->Value = ((cursor->Max - cursor->Min) *(cursor->X - cursor->Bar->X)) / cursor->Bar->Length;

      }
      // display new position
      Menu_DisplayPicture(cursor->X, cursor->Y, cursor->Picture);
      cursor->Callback(cursor);

      // touch detected
      return 1;
    }
  }
  // no touch detected  
  return 0;
}


static void Menu_ClearCursor(Menu_Cursor_t *cursor)
{
  Menu_SetStyle(CLEAR_PLAIN);
  Menu_FillRectangle(cursor->X,cursor->Y,cursor->Width,cursor->Height);
  Menu_DisplayBar(cursor->Bar);
}

static uint32_t Menu_CursorGetPosition(Menu_Cursor_t *cursor)
{
  if(cursor->Bar->Direction == MENU_BAR_VERTICAL)
  {
    return cursor->Bar->Y + ((cursor->Bar->Length * cursor->Value) / (cursor->Max - cursor->Min));
  } else {
    return cursor->Bar->X + ((cursor->Bar->Length * cursor->Value) / (cursor->Max - cursor->Min));
  }
}

static void Menu_DisplayBar(Menu_Bar_t *bar)
{
  Menu_SetStyle(bar->Style);
  if (bar->Direction == MENU_BAR_VERTICAL)
  {
    Menu_DisplayRectangle(bar->X,bar->Y,0,bar->Length);
  } else {
    Menu_DisplayRectangle(bar->X,bar->Y,bar->Length,0);
  }
}

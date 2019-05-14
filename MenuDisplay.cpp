#include "MenuDisplay.h"

void MenuDisplay::updateDisplay(Menu* currentMenu) {
  if (!currentMenu->needsRedraw()) return;
  char buffer[20];
  //display->setTextSize(1);
  //display->setTextColor(WHITE);
  //display->clear();//Display();
  //display->setFont(&FreeMonoBold9pt7b);
  display->setFont(Arial_bold_14);
  uint8_t startIndex = currentMenu->getScrollOffset();
  uint8_t active = currentMenu->getSelectedItem();
  for (int i=0; i<4; i++) { 
    display->setCursor(0,i*2);
    //display->clearToEOL();
    MenuItem* item = currentMenu->getItem(startIndex+i);
    display->setInvertMode(0);
    //display->setTextColor(WHITE, BLACK);
    if (startIndex+i==active) {
      //display->setCursor(0,12+i*15);
      
      if (currentMenu->isActivated()) {
        display->print("O ");
        //display->setTextColor(BLACK, WHITE);
        //display->fillRect(15, i*15, 128, 14, WHITE);
        display->setInvertMode(1);
      } else {
          display->print("> ");
      }
    } else {
        display->print("  ");
    }
    //display->setCursor(15,12+i*15);
    display->setCursor(12,i*2);
    
    if (item!=NULL) {
        item->getText(buffer);
        display->print(buffer);
        display->clearToEOL();
        item->doneRedraw();
    } else {
        display->clearToEOL();
    }
  }
  //print_float(inp, 4, 2);
  //display->display();
  currentMenu->doneRedraw();
}

#define MAX_DIGITS 10
void MenuDisplay::print_num_padded(int32_t c, char base, int padded_length, char padding_character)
{
  char storage[MAX_DIGITS];
  int32_t i = MAX_DIGITS;
  int32_t cpos;
  int j = 0;

  cpos=abs(c);
  do
  {
    i--;
    storage[i] = cpos % base;
    cpos = cpos / base;
  } while((i >= 0) && (cpos > 0) );

  /* Take Care of the sign (only for decimal numbers)*/
  if(c < 0)
  {
    // add padding counting minus sign
    for (j = 0; j<padded_length - (MAX_DIGITS - i) - 1; j++) {
      display->print(padding_character);
    }
    display->print('-');
  } else {
    // add padding
    for (j = 0; j<padded_length - (MAX_DIGITS - i); j++) {
      display->print(padding_character);
    }
  }
  /* i is the index of the last digit calculated */
  /* Hence, there is no need to initialize i */
  for( ; i<MAX_DIGITS; i++)
  {
    display->print((int32_t)storage[i]);
  }
}

void MenuDisplay::print_float(float c, int before_digits, int after_digits)
{
  int32_t i;
  float num = c;
  
  if (c<0) 
  {
    display->print("-");
    num=-c;
  } 
  else 
  {
    display->print(" ");
  }

  int32_t whole=abs(num);
  float after=(num-(float)whole);

  print_num_padded(whole, 10, before_digits, ' ');
  display->print("."); 
  for (i=0; i<after_digits; i++) 
  {
    after*=10;
    display->print( (int32_t)after);
    after=after-(int32_t)after;
  }
}

#include "Selectable.h"

void ISelectable::HandleSelection() 
{
   if (ShouldSelect())
      Select();
   else if (ShouldUnSelect())
      UnSelect();
}

void ISelectable::UnSelect() {}

bool ISelectable::ShouldSelect() {return true;}

bool ISelectable::ShouldUnSelect() {return !ShouldSelect();}
#include "GridOccupy.h"

// Add default functionality here for any IGridOccupy functions that are not pure virtual.
void IGridOccupy::MoveTiles(UTile* NewTile)
{
   DeleteFromCurrentTile();
   OccupyNewTile(NewTile);
}
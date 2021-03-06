///////////////////////////////////////////////////////////////////////////////
//
// Copyright 1997-1999 Pandemic Studios, Dark Reign II
//
// Task Utils
//


#ifndef __TASKUTIL_H
#define __TASKUILS_H


///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "team.h"


///////////////////////////////////////////////////////////////////////////////
//
// NameSpace TaskUtil
//
namespace TaskUtil
{

  // Find an unexplored point
  void FindUnexplored(Vector &location, const Point<S32> &cell, U8 tractionType, Team *team);

}

#endif
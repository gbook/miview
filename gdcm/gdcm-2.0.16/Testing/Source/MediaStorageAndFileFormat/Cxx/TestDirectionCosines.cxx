/*=========================================================================

  Program: GDCM (Grassroots DICOM). A DICOM library
  Module:  $URL$

  Copyright (c) 2006-2010 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "gdcmDirectionCosines.h"
#include <set>
#include <iostream>

/*
 * Let's use the CrossDot API to compare IOP safely
 */
static const char * ImageOrientationPatientList[] = {
"0.999081\\0.0426953\\0.00369275\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369272\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369275\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369272\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369275\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426952\\0.00369272\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369272\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369274\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369274\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369272\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369272\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426952\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426952\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369274\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369274\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369274\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369275\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369272\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369275\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369272\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369275\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426952\\0.00369272\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369272\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369274\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369274\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369272\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369272\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426952\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426952\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369273\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369274\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369274\\-0.0419025\\0.955059\\0.293439",
"0.999081\\0.0426953\\0.00369274\\-0.0419025\\0.955059\\0.293439",
};

bool TestCrossDot()
{
  std::set< std::string > myset;
  const unsigned int nipp = sizeof(ImageOrientationPatientList) / sizeof(*ImageOrientationPatientList);
  for( unsigned int i = 0; i < nipp; ++i )
    {
    myset.insert( ImageOrientationPatientList[i] );
    }

  if( myset.size() <= 1 ) return false;

  std::set< std::string >::const_iterator it = myset.begin();
  gdcm::DirectionCosines ref;
  gdcm::DirectionCosines dc;
  for( ; it != myset.end(); ++it )
    {
    ref.SetFromString( it->c_str() );
    for( unsigned int i = 0; i < nipp; ++i )
      {
      dc.SetFromString( ImageOrientationPatientList[i] );
      const double crossdot = ref.CrossDot( dc);
      const double eps = 1. - crossdot;
      if( eps > 1e-6 || eps < 0 ) return false;
      }
    }
  return true;
}

int TestDirectionCosines(int, char *[])
{
  gdcm::DirectionCosines dc;
  const double *dircos = dc;
  if( !dc.IsValid() )
    {
    return 1;
    }
  const double cross[3] = {0,0,1};
  double z[3];
  dc.Cross(z);
  if( z[0] != cross[0] 
   || z[1] != cross[1] 
   || z[2] != cross[2] )
    {
    return 1;
    }

  bool b = TestCrossDot();
  if( !b ) return 1;

  return 0;
}

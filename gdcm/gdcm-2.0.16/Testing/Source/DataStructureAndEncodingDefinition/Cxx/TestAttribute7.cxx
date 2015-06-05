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
#include "gdcmAttribute.h"

int TestAttribute7(int, char *[])
{
  const char bytes[] = "\030\000e\020";
  gdcm::DataElement de( gdcm::Tag(0x28,0x9)  );
  de.SetVR( gdcm::VR::INVALID );
  de.SetByteValue( bytes, 4 );

  gdcm::Attribute<0x28,0x9, gdcm::VR::AT, gdcm::VM::VM1 > at;
  at.SetFromDataElement( de );
  std::cout << at.GetValue(0) << std::endl;

  return 0;
}

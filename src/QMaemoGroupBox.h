//***************************************************************************
//
// GPS Recorder
// A GPS data logger for the Maemo platform.
//
// Copyright (C) 2010 Jean-Charles Lefebvre <polyvertex+gpsrecorder [AT] gmail [DOT] com>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions :
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
//
// Author     : Jean-Charles Lefebvre
// Created On : 2010-08-04 18:45:02
//
// $Id: QMaemoGroupBox.h 230 2013-05-10 09:28:15Z jcl $
//
//***************************************************************************

#ifndef __QMAEMOGROUPBOX_H__
#define __QMAEMOGROUPBOX_H__

#include "stable.h"


//---------------------------------------------------------------------------
// QMaemoGroupBox
//---------------------------------------------------------------------------
class QMaemoGroupBox : public QVBoxLayout
{
  Q_OBJECT

public :
  QMaemoGroupBox (QWidget* pParent=0);
  QMaemoGroupBox (const QString& strText, QWidget* pParent=0);

  void setText    (const QString& strText);
  void setEnabled (bool bEnabled);

  QLabel* label (void) { return m_pLabel; }
  QFrame* frame (void) { return m_pFrame; }


private :
  void construct        (QWidget* pParent);
  void setLayoutEnabled (QLayout* pLayout, bool bEnabled);


private :
  QLabel* m_pLabel;
  QFrame* m_pFrame;
};


#endif // #ifndef __QMAEMOGROUPBOX_H__

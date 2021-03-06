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
// Created On : 2010-10-26 15:09:57
//
// $Id: WndMeansOfTransport.cpp 230 2013-05-10 09:28:15Z jcl $
//
//***************************************************************************

#include "stable.h"


//---------------------------------------------------------------------------
// WndMeansOfTransport
//---------------------------------------------------------------------------
WndMeansOfTransport::WndMeansOfTransport (QWidget* pParent/*=0*/)
: QDialog(pParent)
, m_comboBox(this)
{
  Q_ASSERT(App::instance());
  Q_ASSERT(pParent);

  m_bCanceled = true;

  this->setWindowTitle(tr("Select your means of transport"));
  this->setModal(true);
}



//---------------------------------------------------------------------------
// doExec
//---------------------------------------------------------------------------
bool WndMeansOfTransport::doExec (quint8 ucDefaultMeansOfTransport, const QString& strOtherMotLabel)
{
  m_comboBox.selectCurrentMeansOfTransport(ucDefaultMeansOfTransport);
  m_comboBox.setOtherMeansOfTransport(strOtherMotLabel);

  this->setupControls();

  m_bCanceled = true;
  this->exec();

  return m_bCanceled ? false : true;
}



//---------------------------------------------------------------------------
// setupControls
//---------------------------------------------------------------------------
void WndMeansOfTransport::setupControls (void)
{
  QGridLayout* pGrid;
  QPushButton* pBtnDone;

  pBtnDone = new QPushButton(tr("Done"));
  this->connect(pBtnDone, SIGNAL(clicked()), SLOT(onClickedDone()));

  pGrid = new QGridLayout;
  pGrid->addWidget(&m_comboBox, 0, 0, 1, 5);
  pGrid->addWidget(pBtnDone,    0, 5);

  this->setLayout(pGrid);
}



//---------------------------------------------------------------------------
// onClickedDone
//---------------------------------------------------------------------------
void WndMeansOfTransport::onClickedDone (void)
{
  AppSettings& settings = *App::instance()->settings();

  settings.setLastMeansOfTransport(this->meansOfTransport());
  if (this->meansOfTransport() == GPSRFile::MEANSTRANSPORT_OTHER)
    settings.setLastOtherMeansOfTransport(qPrintable(this->otherMeansOfTransportName()));

  m_bCanceled = false;
  this->done(0);
}

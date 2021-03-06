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
// Created On : 2010-10-26 17:13:01
//
// $Id: QMeansOfTransport.cpp 230 2013-05-10 09:28:15Z jcl $
//
//***************************************************************************

#include "stable.h"


//---------------------------------------------------------------------------
// QMeansOfTransport
//---------------------------------------------------------------------------
QMeansOfTransport::QMeansOfTransport (QWidget* pParent/*=0*/)
: QMaemoComboBox(tr("Means of transport"), pParent)
{
  QVector<quint8>         vecMOT(GPSRFile::meansOfTransportList());
  QVectorIterator<quint8> itMOT(vecMOT);

  if (App::instance() && App::instance()->settings())
    m_strOtherMOT = App::instance()->settings()->getLastOtherMeansOfTransport();


  while (itMOT.hasNext())
  {
    quint8 ucMOT = itMOT.next();

    if (ucMOT == GPSRFile::MEANSTRANSPORT_UNKNOWN ||
      ucMOT == GPSRFile::MEANSTRANSPORT_OTHER ||
      ucMOT == GPSRFile::MEANSTRANSPORT_FLYINGSAUCER)
      continue;

    this->addItem(
      GPSRFile::fullMeansOfTransportToLabel(ucMOT, m_strOtherMOT),
      (int)ucMOT);
  }

  this->sortItems();

  this->insertItem(
    0,
    GPSRFile::fullMeansOfTransportToLabel(GPSRFile::MEANSTRANSPORT_UNKNOWN, m_strOtherMOT),
    (int)GPSRFile::MEANSTRANSPORT_UNKNOWN);

  this->addItem(
    GPSRFile::fullMeansOfTransportToLabel(GPSRFile::MEANSTRANSPORT_OTHER, m_strOtherMOT),
    (int)GPSRFile::MEANSTRANSPORT_OTHER);
  m_iOtherMotIndex = this->count() - 1;


  this->connect(this, SIGNAL(sigSelected(int)), SLOT(onSelected(int)));
  this->setValueLayout(QMaemo5ValueButton::ValueUnderText);
  this->setCurrentIndex(0);
}



//---------------------------------------------------------------------------
// selectCurrentMeansOfTransport
//---------------------------------------------------------------------------
bool QMeansOfTransport::selectCurrentMeansOfTransport (quint8 ucMeansOfTransport)
{
  for (int i = 0; i < this->count(); ++i)
  {
    if (this->itemData(i).toInt() == (int)ucMeansOfTransport)
    {
      this->setCurrentIndex(i);
      return true;
    }
  }

  return false;
}

//---------------------------------------------------------------------------
// setOtherMeansOfTransport
//---------------------------------------------------------------------------
void QMeansOfTransport::setOtherMeansOfTransport (const QString& strOtherMOT)
{
  m_strOtherMOT = strOtherMOT;

  this->setItemText(
    m_iOtherMotIndex,
    GPSRFile::fullMeansOfTransportToLabel(GPSRFile::MEANSTRANSPORT_OTHER, m_strOtherMOT));
}

//---------------------------------------------------------------------------
// meansOfTransport
//---------------------------------------------------------------------------
quint8 QMeansOfTransport::meansOfTransport (void)
{
  return (quint8)this->currentItemData().toInt();
}



//---------------------------------------------------------------------------
// onSelected
//---------------------------------------------------------------------------
void QMeansOfTransport::onSelected (int iIndex)
{
  static int iLastIndex = -1;

  if (this->itemData(iIndex).toInt() == (int)GPSRFile::MEANSTRANSPORT_OTHER)
  {
    // here, user wants to specify its own means of transport

    QString strInput;
    bool    bOk = false;

    strInput = QInputDialog::getText(
      static_cast<QWidget*>(this->parent()),
      tr("Name ?"),
      tr("Please enter your means of transport or leave blank :"),
      QLineEdit::Normal,
      m_strOtherMOT,
      &bOk);

    if (bOk)
    {
      m_strOtherMOT = GPSRFile::validateOtherMeansOfTransport(strInput);

      this->setItemText(
        iIndex,
        GPSRFile::fullMeansOfTransportToLabel(GPSRFile::MEANSTRANSPORT_OTHER, m_strOtherMOT));
    }
    else if (iLastIndex >= 0)
    {
      this->setCurrentIndex(iLastIndex);
    }
  }

  iLastIndex = iIndex;
}

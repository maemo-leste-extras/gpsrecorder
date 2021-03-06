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
// Created On : 2010-04-03 11:58:47
//
// $Id: ExporterSinkCsv.cpp 230 2013-05-10 09:28:15Z jcl $
//
//***************************************************************************

#include "stable.h"


//---------------------------------------------------------------------------
// ExporterSinkCsv
//---------------------------------------------------------------------------
ExporterSinkCsv::ExporterSinkCsv (Exporter* pParent)
: ExporterSink(pParent)
{
  m_cSeparator = ExporterSinkCsv::defaultSeparator();

  this->connect(
    pParent,
    SIGNAL(sigSOF(const char*, time_t, qint32)),
    SLOT(onSOF(const char*, time_t, qint32)) );
  this->connect(
    pParent,
    SIGNAL(sigLocationFix(time_t, const LocationFixContainer&)),
    SLOT(onLocationFix(time_t, const LocationFixContainer&)) );
  this->connect(
    pParent,
    SIGNAL(sigEOF(void)),
    SLOT(onEOF(void)) );

  // read settings
  m_cSeparator = App::instance()->settings()->getCsvSeparator();
}

//---------------------------------------------------------------------------
// ~ExporterSinkCsv
//---------------------------------------------------------------------------
ExporterSinkCsv::~ExporterSinkCsv (void)
{
  this->close();
}



//---------------------------------------------------------------------------
// close
//---------------------------------------------------------------------------
void ExporterSinkCsv::close (void)
{
  ExporterSink::close();
}



//---------------------------------------------------------------------------
// separatorIndex
//---------------------------------------------------------------------------
int ExporterSinkCsv::separatorIndex (char cSep)
{
  const char* pszSeps = ExporterSinkCsv::supportedSeparators();
  const char* psz = strchr(pszSeps, cSep);

  return !psz ? -1 : int(psz - pszSeps);
}



//---------------------------------------------------------------------------
// onSOF
//---------------------------------------------------------------------------
void ExporterSinkCsv::onSOF (const char* pszFilePath, time_t uiTime, qint32 iTimeZoneOffset)
{
  Q_UNUSED(uiTime);
  Q_UNUSED(iTimeZoneOffset);

  Q_ASSERT(pszFilePath);
  Q_ASSERT(pszFilePath[0]);

  this->close();
  m_strFilePath  = pszFilePath;
  m_strFilePath += '.';
  m_strFilePath += this->extension();

  m_pFile = fopen(qPrintable(m_strFilePath), "wb");
  if (!m_pFile)
  {
    qWarning("Could not create %s ! Error %d : %s", qPrintable(m_strFilePath), errno, strerror(errno));
    return;
  }

  // write header
  fprintf(m_pFile,
    "Time" "%c" "Latitude" "%c" "Longitude" "%c" "AltitudeM" "%c" "Heading" "%c" "SpeedKmh\n",
    m_cSeparator, m_cSeparator, m_cSeparator, m_cSeparator, m_cSeparator);
}

//---------------------------------------------------------------------------
// onLocationFix
//---------------------------------------------------------------------------
void ExporterSinkCsv::onLocationFix (time_t uiTime, const LocationFixContainer& fixCont)
{
  Q_UNUSED(uiTime);

  Q_ASSERT(fixCont.hasFix());
  if (!m_pFile || !fixCont.hasFix())
    return;

  const LocationFix& fix = *fixCont.getFix();

  if (!fix.hasFields(FIXFIELD_LATLONG))
    return;

  fprintf(m_pFile,
    "%u%c" "%.6lf%c" "%.6lf%c" "%i%c" "%.2lf%c" "%.2lf\n",
    (fix.hasFields(FIXFIELD_TIME) ? fix.uiTime : 0), m_cSeparator,
    fix.getLatDeg(), m_cSeparator,
    fix.getLongDeg(), m_cSeparator,
    (fix.hasFields(FIXFIELD_ALT) ? fix.iAlt : 0), m_cSeparator,
    fix.getTrackDeg(), m_cSeparator,
    fix.getSpeed(HORIZSPEEDUNIT_KMH) );
}

//---------------------------------------------------------------------------
// onEOF
//---------------------------------------------------------------------------
void ExporterSinkCsv::onEOF (void)
{
  this->close();
}

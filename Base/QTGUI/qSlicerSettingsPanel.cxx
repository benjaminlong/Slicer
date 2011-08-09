// Qt includes
#include <QDebug>
#include <QMainWindow>
#include <QSettings>
#include <QString>
#include <QList>

// CTK includes
#include <ctkLogger.h>

// QtGUI includes
#include "qSlicerApplication.h"
#include "qSlicerSettingsPanel.h"

static ctkLogger logger("org.commontk.libs.widgets.qSlicerSettingsPanel");

// --------------------------------------------------------------------------
// qSlicerSettingsPanelPrivate

//-----------------------------------------------------------------------------
class qSlicerSettingsPanelPrivate
{
  Q_DECLARE_PUBLIC(qSlicerSettingsPanel);
protected:
  qSlicerSettingsPanel* const q_ptr;

public:
  qSlicerSettingsPanelPrivate(qSlicerSettingsPanel& object);
  
  bool RestartRequested;
  QList<QString> reasonsRestart;
};

// --------------------------------------------------------------------------
// qSlicerSettingsPanelPrivate methods
// --------------------------------------------------------------------------
qSlicerSettingsPanelPrivate::qSlicerSettingsPanelPrivate(qSlicerSettingsPanel& object)
  :q_ptr(&object)
{
  this->RestartRequested = false;
  this->reasonsRestart = QList<QString>();
}


// --------------------------------------------------------------------------
// qSlicerSettingsPanel methods
// --------------------------------------------------------------------------
qSlicerSettingsPanel::qSlicerSettingsPanel(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerSettingsPanelPrivate(*this))
{
}

// --------------------------------------------------------------------------
qSlicerSettingsPanel::~qSlicerSettingsPanel()
{
}

// --------------------------------------------------------------------------
bool qSlicerSettingsPanel::restartRequested()const
{
  Q_D(const qSlicerSettingsPanel);
  return d->RestartRequested;
}

// --------------------------------------------------------------------------
void qSlicerSettingsPanel::setRestartRequested(bool value, const QString& reason)
{
  Q_D(qSlicerSettingsPanel);
  d->RestartRequested = value;
  if (!value)
    {
    d->reasonsRestart.removeOne(reason);
    }
  // Else if this settings is not set.
  // Important because we can set the settings severals times
  else if(!d->reasonsRestart.contains(reason))
    {
    d->reasonsRestart.append(reason);
    }

  qDebug() << "Debug : " << d->reasonsRestart.size();
}

// --------------------------------------------------------------------------
QList<QString> qSlicerSettingsPanel::getReasonRestart()
{
  Q_D(qSlicerSettingsPanel);
  return d->reasonsRestart;
}

// --------------------------------------------------------------------------
void qSlicerSettingsPanel::resetSettings()
{
  this->Superclass::resetSettings();
}

// --------------------------------------------------------------------------
void qSlicerSettingsPanel::restoreDefaultSettings()
{
  this->Superclass::restoreDefaultSettings();
}


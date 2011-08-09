/*==============================================================================

  Program: 3D Slicer

  Copyright (c) 2010 Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Qt includes
#include <QMainWindow>
#include <QTranslator>
#include <QLocale>
#include <QSettings>

// CTK includes
#include <ctkLogger.h>

// QtGUI includes
#include "qSlicerApplication.h"
#include "qSlicerSettingsGeneralPanel.h"
#include "ui_qSlicerSettingsGeneralPanel.h"

static ctkLogger logger("org.commontk.libs.widgets.qSlicerSettingsGeneralPanel");

// --------------------------------------------------------------------------
// qSlicerSettingsGeneralPanelPrivate

//-----------------------------------------------------------------------------
class qSlicerSettingsGeneralPanelPrivate: public Ui_qSlicerSettingsGeneralPanel
{
  Q_DECLARE_PUBLIC(qSlicerSettingsGeneralPanel);
protected:
  qSlicerSettingsGeneralPanel* const q_ptr;

public:
  qSlicerSettingsGeneralPanelPrivate(qSlicerSettingsGeneralPanel& object);
  void init();
};

// --------------------------------------------------------------------------
// qSlicerSettingsGeneralPanelPrivate methods

// --------------------------------------------------------------------------
qSlicerSettingsGeneralPanelPrivate::qSlicerSettingsGeneralPanelPrivate(qSlicerSettingsGeneralPanel& object)
  :q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerSettingsGeneralPanelPrivate::init()
{
  Q_Q(qSlicerSettingsGeneralPanel);

  this->setupUi(q);
  /// set the directory where there are all the translations files
  this->LanguageComboBox->setDirectory("/home/benjaminlong/work/slicer/Slicer4-Superbuild/Slicer-build/Applications/SlicerQT");
  /// Default values
  this->LanguageComboBox->setCurrentLanguage("en");

  QObject::connect(this->FontButton, SIGNAL(currentFontChanged(const QFont&)),
                   q, SLOT(onFontChanged(const QFont&)));
  QObject::connect(this->ShowToolTipsCheckBox, SIGNAL(toggled(bool)),
                   q, SLOT(onShowToolTipsToggled(bool)));
  QObject::connect(this->ShowToolButtonTextCheckBox, SIGNAL(toggled(bool)),
                   q, SLOT(onShowToolButtonTextToggled(bool)));
  QObject::connect(this->LanguageComboBox, SIGNAL(currentLanguageNameChanged(const QString&)),
                   q, SLOT(onLanguageNameChanged(const QString&)));

  q->registerProperty("no-splash", this->ShowSplashScreenCheckBox, "checked",
                      SIGNAL(toggled(bool)));
  q->registerProperty("no-tooltip", this->ShowToolTipsCheckBox, "checked",
                      SIGNAL(toggled(bool)));
  q->registerProperty("font", this->FontButton, "currentFont",
                      SIGNAL(currentFontChanged(const QFont&)));
  q->registerProperty("MainWindow/ShowToolButtonText", this->ShowToolButtonTextCheckBox,
                      "checked", SIGNAL(toggled(bool)));
  q->registerProperty("MainWindow/RestoreGeometry", this->RestoreUICheckBox, "checked",
                      SIGNAL(toggled(bool)));
  q->registerProperty("MainWindow/ConfirmRestart", this->ConfirmRestartCheckBox, "checked",
                      SIGNAL(toggled(bool)));
  q->registerProperty("MainWindow/ConfirmExit", this->ConfirmExitCheckBox, "checked",
                      SIGNAL(toggled(bool)));
  q->registerProperty("language", this->LanguageComboBox, "currentLanguage",
                      SIGNAL(currentLanguageNameChanged(const QString&)));

  // Hide 'Restart requested' label
  q->Superclass::setRestartRequested(false,"languages");
}

// --------------------------------------------------------------------------
// qSlicerSettingsGeneralPanel methods
// --------------------------------------------------------------------------
qSlicerSettingsGeneralPanel::qSlicerSettingsGeneralPanel(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerSettingsGeneralPanelPrivate(*this))
{
  Q_D(qSlicerSettingsGeneralPanel);
  d->init();
}

// --------------------------------------------------------------------------
qSlicerSettingsGeneralPanel::~qSlicerSettingsGeneralPanel()
{
}

// --------------------------------------------------------------------------
void qSlicerSettingsGeneralPanel::setRestartRequested(bool value, const QString &reason)
{
  Q_D(qSlicerSettingsGeneralPanel);
  this->Superclass::setRestartRequested(value,reason);
  d->RestartRequestedLabel->setVisible(value);
}

// --------------------------------------------------------------------------
void qSlicerSettingsGeneralPanel::resetSettings()
{
  this->Superclass::resetSettings();
  this->setRestartRequested(false,"languages");
}

// --------------------------------------------------------------------------
void qSlicerSettingsGeneralPanel::restoreDefaultSettings()
{
  bool shouldRestart = false;
  if (this->defaultPropertyValue("language").toStringList()
      != this->previousPropertyValue("language").toStringList())
    {
    shouldRestart = true;
    }
  this->Superclass::restoreDefaultSettings();

  this->setRestartRequested(shouldRestart,"languages");
}


// --------------------------------------------------------------------------
void qSlicerSettingsGeneralPanel::onFontChanged(const QFont& font)
{
  qApp->setFont(font);
}

// --------------------------------------------------------------------------
void qSlicerSettingsGeneralPanel::onShowToolTipsToggled(bool disable)
{
  qSlicerApplication::application()->setToolTipsEnabled(!disable);
}

// --------------------------------------------------------------------------
void qSlicerSettingsGeneralPanel::onShowToolButtonTextToggled(bool enable)
{
  foreach(QWidget* widget, qSlicerApplication::application()->topLevelWidgets())
    {
    QMainWindow* mainWindow = qobject_cast<QMainWindow*>(widget);
    if (mainWindow)
      {
      mainWindow->setToolButtonStyle(enable ? Qt::ToolButtonTextUnderIcon : Qt::ToolButtonIconOnly);
      }
    }
}

// --------------------------------------------------------------------------
void qSlicerSettingsGeneralPanel::onLanguageNameChanged(const QString& name)
{
  Q_D(qSlicerSettingsGeneralPanel);
  d->LanguageComboBox->setCurrentLanguage(name);
  if(this->propertyValue("language").toString() !=
     this->previousPropertyValue("language").toString())
    {
    this->setRestartRequested(true,"languages");
    }
  else
    {
    this->setRestartRequested(false,"languages");
    }
}

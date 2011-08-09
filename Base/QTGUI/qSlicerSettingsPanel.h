#ifndef __qSlicerSettingsPanel_h
#define __qSlicerSettingsPanel_h

// Qt includes
#include <QWidget>
#include <QStringList>

// CTK includes
#include <ctkSettingsPanel.h>

// QtGUI includes
#include "qSlicerBaseQTGUIExport.h"

class QSettings;
class qSlicerSettingsPanelPrivate;

class Q_SLICER_BASE_QTGUI_EXPORT qSlicerSettingsPanel 
  : public ctkSettingsPanel
{
  Q_OBJECT
  //Q_PROPERTY(bool restartRequested READ restartRequested WRITE setRestartRequested)
public:
  /// Superclass typedef
  typedef ctkSettingsPanel Superclass;
  /// Constructor
  explicit qSlicerSettingsPanel(QWidget* parent = 0);
  /// Destructor
  virtual ~qSlicerSettingsPanel();
  
  /// Return True if the application is expected to be restarted.
  bool restartRequested()const;
  /// \sa restartRequested()
  void setRestartRequested(bool value, const QString& reason);

  QList<QString> getReasonRestart();
  
protected slots:
  virtual void resetSettings();
  virtual void restoreDefaultSettings();

protected:
  QScopedPointer<qSlicerSettingsPanelPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerSettingsPanel);
  Q_DISABLE_COPY(qSlicerSettingsPanel);
};

#endif

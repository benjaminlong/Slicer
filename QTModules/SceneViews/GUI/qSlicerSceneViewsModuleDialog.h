#ifndef __qSlicerSceneViewsModuleDialog_h
#define __qSlicerSceneViewsModuleDialog_h

#include <vtkSlicerSceneViewsModuleLogic.h>

#include "qMRMLScreenShotDialog.h"

/// \ingroup Slicer_QtModules_SceneViews
class qSlicerSceneViewsModuleDialog : public qMRMLScreenShotDialog
{
  Q_OBJECT
public:
  qSlicerSceneViewsModuleDialog();
  ~qSlicerSceneViewsModuleDialog();

  /// Set the SceneViews module logic.
  void setLogic(vtkSlicerSceneViewsModuleLogic* logic);

  /// Initialize this dialog with values from an existing annotation Snapshot node.
  void initialize(const QString& nodeId);
  /// Reset the dialog and give it a unique name.
  void reset();

protected:

protected slots:
  virtual void onDialogRejected();
  virtual void onDialogAccepted();

  /// Slots for the restore button
  void onRestoreButtonClicked();

signals:
  void dialogRejected();
  void dialogAccepted();

private:
    vtkSlicerSceneViewsModuleLogic* m_Logic;
};

#endif

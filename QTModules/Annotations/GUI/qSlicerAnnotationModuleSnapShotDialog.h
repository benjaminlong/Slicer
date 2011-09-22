#ifndef __qSlicerAnnotationModuleSnapShotDialog_h
#define __qSlicerAnnotationModuleSnapShotDialog_h

#include "ui_qSlicerAnnotationModuleSnapShotDialog.h"
#include "Logic/vtkSlicerAnnotationModuleLogic.h"

#include "qMRMLScreenShotDialog.h"

/// \ingroup Slicer_QtModules_Annotation
class qSlicerAnnotationModuleSnapShotDialog : public qMRMLScreenShotDialog
{
  Q_OBJECT
public:
  qSlicerAnnotationModuleSnapShotDialog();
  ~qSlicerAnnotationModuleSnapShotDialog();

  /// Set the Annotation module logic.
  void setLogic(vtkSlicerAnnotationModuleLogic* logic);

  /// Initialize this dialog with values from an existing annotation Snapshot node.
  void initialize(const char* nodeId);
  /// Reset the dialog and give it a unique name.
  void reset();

protected:

protected slots:

  virtual void onDialogRejected();
  virtual void onDialogAccepted();

signals:
  void dialogRejected();
  void dialogAccepted();

private:
    vtkSlicerAnnotationModuleLogic* m_Logic;
};

#endif

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

#ifndef __qMRMLScreenShotDialog_h
#define __qMRMLScreenShotDialog_h

// Qt includes
#include <QDialog>
#include <QString>

// QTGUI includes
//#include "qMRMLayoutManager.h"
class qMRMLLayoutManager;

// qMRMLWidget includes
#include "qMRMLWidgetsExport.h"

// VTK includes
//#include <vtkStdString.h>
//#include <vtkImageData.h>
//class vtkStdString;
#include "vtkImageData.h"
//class vtkImageData;

class qMRMLScreenShotDialogPrivate;

class QMRML_WIDGETS_EXPORT qMRMLScreenShotDialog : public QDialog
{
  Q_OBJECT

public:
  typedef QDialog Superclass;

  qMRMLScreenShotDialog();
  ~qMRMLScreenShotDialog();

  void setLayoutManager(qMRMLLayoutManager* newlayoutManager);
  qMRMLLayoutManager* layoutManager()const;

//  void setWindowTitle(const QString& title);
//  QString windowTitle();

  void setNameEdit(const QString& name);
  QString nameEdit()const;

  void setDescription(const QString& description);
  QString description()const;

  void setID(const QString& id);
  QString id()const;

  void setScreenshotType(int type);
  int screenshotType()const;

  void setScaleFactor(double scaleFactor);
  double scaleFactor()const;

  //void setShowResetButton(bool);
  //bool showResetButton();

  void setShowScaleFactorSpinBox(bool);
  bool showScaleFactorSpinBox()const;

  void setImageData(vtkImageData* screenshot);
  vtkImageData* imageData()const;

  /*/// Set the SceneViews module logic.
  virtual void setLogic(vtkSlicerSceneViewsModuleLogic* logic) = 0;*/

  /// Initialize this dialog with valies from an existinf annotation snapshot node
  void initializeDialog();

  /// Reset the dialog
  void resetDialog();

protected:

protected slots:
  virtual void onDialogAccepted() = 0;
  virtual void onDialogRejected() = 0;

  /// Slots for the four radio buttons to select screenShot area
  void onThreeDViewRadioClicked();
  void onRedSliceViewRadioClicked();
  void onYellowSliceViewRadioClicked();
  void onGreenSliceViewRadioClicked();
  void onFullLayoutRadioClicked();
  void onScaleFactorSpinBoxChanged();

  void onNameEditChanged(const QString& newName);
  void onDescriptionChanged();

private:
  void createConnection();

  /// Grab a screenshot of the 3DView or any sliceView.
  /// The screenshotWindow is Red, Green, Yellow for a sliceView or empty for
  /// a ThreeDView
  void grabScreenShot(QString screenshotWindow);

protected:
  QScopedPointer<qMRMLScreenShotDialogPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qMRMLScreenShotDialog);
  Q_DISABLE_COPY(qMRMLScreenShotDialog);
};

#endif


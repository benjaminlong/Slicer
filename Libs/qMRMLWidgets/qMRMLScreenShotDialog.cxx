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

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// QT includes
#include <QPushButton>
#include <QWeakPointer>
#include <QDebug>

// CTK includes
#include <ctkVTKSliceView.h>
#include <ctkVTKWidgetsUtils.h>

// qMRML includes
#include "qMRMLLayoutManager.h"
#include "qMRMLSliceWidget.h"
#include "qMRMLScreenShotDialog.h"
#include "qMRMLThreeDView.h"
#include "qMRMLThreeDWidget.h"
#include "qMRMLUtils.h"

#include "ui_qMRMLScreenShotDialog.h"

// VTK includes
#include <vtkWeakPointer.h>

//-----------------------------------------------------------------------------
class qMRMLScreenShotDialogPrivate : public Ui_qMRMLScreenShotDialog
{
  Q_DECLARE_PUBLIC(qMRMLScreenShotDialog);
protected:
  qMRMLScreenShotDialog* const q_ptr;
public:
  qMRMLScreenShotDialogPrivate(qMRMLScreenShotDialog& object);

  QWeakPointer<qMRMLLayoutManager>  layoutManager;
  vtkWeakPointer<vtkImageData>      imageData;

  /// The ID of the associated snapshot node.
  /// This is NULL if the dialog has no associated snapshot node (== new snapshot mode).
  QString       idName;
  QString       name;
  QString       description;
  int           type;
  double        scaleFactor;
};

//-----------------------------------------------------------------------------
qMRMLScreenShotDialogPrivate::qMRMLScreenShotDialogPrivate(qMRMLScreenShotDialog &object)
  : q_ptr(&object)
{
  this->layoutManager = new qMRMLLayoutManager();
  this->imageData = vtkImageData::New();
}

//-----------------------------------------------------------------------------
// qMRMLScreenShotDialog methods

//-----------------------------------------------------------------------------
qMRMLScreenShotDialog::qMRMLScreenShotDialog() :
    Superclass()
    , d_ptr(new qMRMLScreenShotDialogPrivate(*this))
{
  Q_D(qMRMLScreenShotDialog);
  d->setupUi(this);

  this->createConnection();
}

//-----------------------------------------------------------------------------
qMRMLScreenShotDialog::~qMRMLScreenShotDialog()
{
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::setLayoutManager(qMRMLLayoutManager* newlayoutManager)
{
  Q_D(qMRMLScreenShotDialog);
  d->layoutManager = newlayoutManager;
}

//-----------------------------------------------------------------------------
qMRMLLayoutManager* qMRMLScreenShotDialog::layoutManager() const
{
  Q_D(const qMRMLScreenShotDialog);
  return d->layoutManager.data();
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::setNameEdit(const QString& name)
{
  Q_D(qMRMLScreenShotDialog);
  d->name = name;
}

//-----------------------------------------------------------------------------
QString qMRMLScreenShotDialog::nameEdit() const
{
  Q_D(const qMRMLScreenShotDialog);
  return d->name;
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::setDescription(const QString& description)
{
  Q_D(qMRMLScreenShotDialog);
  d->description = description;
}

//-----------------------------------------------------------------------------
QString qMRMLScreenShotDialog::description() const
{
  Q_D(const qMRMLScreenShotDialog);
  return d->description;
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::setID(const QString& id)
{
  Q_D(qMRMLScreenShotDialog);
  d->idName = id;
}

//-----------------------------------------------------------------------------
QString qMRMLScreenShotDialog::id() const
{
  Q_D(const qMRMLScreenShotDialog);
  return d->idName;
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::setScreenshotType(int type)
{
  Q_D(qMRMLScreenShotDialog);
  d->type = type;
}

//-----------------------------------------------------------------------------
int qMRMLScreenShotDialog::screenshotType() const
{
  Q_D(const qMRMLScreenShotDialog);
  return d->type;
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::setScaleFactor(double scaleFactor)
{
  Q_D(qMRMLScreenShotDialog);
  d->scaleFactor = scaleFactor;
}

//-----------------------------------------------------------------------------
double qMRMLScreenShotDialog::scaleFactor() const
{
  Q_D(const qMRMLScreenShotDialog);
  return d->scaleFactor;
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::setImageData(vtkImageData* screenshot)
{
  Q_D(qMRMLScreenShotDialog);
  d->imageData = screenshot;
}

//-----------------------------------------------------------------------------
vtkImageData* qMRMLScreenShotDialog::imageData() const
{
  Q_D(const qMRMLScreenShotDialog);
  return d->imageData;
}

/*//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::setShowResetButton(bool showned)
{

}

//-----------------------------------------------------------------------------
bool showResetButton()
{

}*/

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::setShowScaleFactorSpinBox(bool showed)
{
  Q_D(qMRMLScreenShotDialog);
  d->scaleFactorSpinBox->setVisible(showed);
  d->scaleFactorLabel->setVisible(showed);
}

//-----------------------------------------------------------------------------
bool qMRMLScreenShotDialog::showScaleFactorSpinBox() const
{
  Q_D(const qMRMLScreenShotDialog);
  return d->scaleFactorSpinBox->isVisible();
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::initializeDialog()
{
  Q_D(qMRMLScreenShotDialog);

  // Name
  d->nameEdit->setText(d->name);
  // Description
  d->descriptionTextEdit->setPlainText(d->description);

  // IdName

  // Type
  switch (d->type)
    {
    case 0:
      d->threeDViewRadio->setChecked(true);
      break;
    case 1:
      d->redSliceViewRadio->setChecked(true);
      break;
    case 2:
      d->yellowSliceViewRadio->setChecked(true);
      break;
    case 3:
      d->greenSliceViewRadio->setChecked(true);
      break;
    case 4:
      d->fullLayoutRadio->setChecked(true);
      break;
    default:
      d->fullLayoutRadio->setChecked(true);
      break;
    }

  // scaleFactor
  d->scaleFactorSpinBox->setValue(this->scaleFactor());
  d->scaleFactorSpinBox->setEnabled(false);

  // we want to disable the modification of the screenshot since this snapshot was created earlier
  // to change a screenshot of an old snapshot, there is a workaround by restoring it,
  // deleting it and creating a new one.
  d->threeDViewRadio->setEnabled(false);
  d->redSliceViewRadio->setEnabled(false);
  d->yellowSliceViewRadio->setEnabled(false);
  d->greenSliceViewRadio->setEnabled(false);
  d->fullLayoutRadio->setEnabled(false);

   // convert the image data from vtkImageData to QImage ...
  QImage qimage;
  qMRMLUtils::vtkImageDataToQImage(d->imageData,qimage);

  // set preview (unscaled)
  d->ScreenshotWidget->setPixmap(QPixmap::fromImage(qimage));
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::resetDialog()
{
  Q_D(qMRMLScreenShotDialog);
  d->fullLayoutRadio->setEnabled(true);
  d->threeDViewRadio->setEnabled(true);
  d->redSliceViewRadio->setEnabled(true);
  d->yellowSliceViewRadio->setEnabled(true);
  d->greenSliceViewRadio->setEnabled(true);

  d->fullLayoutRadio->setChecked(true);
  d->threeDViewRadio->setChecked(false);
  d->redSliceViewRadio->setChecked(false);
  d->yellowSliceViewRadio->setChecked(false);
  d->greenSliceViewRadio->setChecked(false);

  d->scaleFactorSpinBox->setEnabled(true);
  d->scaleFactorSpinBox->setValue(1.0);

  this->grabScreenShot("");
  d->descriptionTextEdit->clear();

  // we want a default name which is easily overwritable by just typing

  // We set the name
  d->nameEdit->setText(this->nameEdit());
  d->nameEdit->setFocus();
  d->nameEdit->selectAll();

  // reset the id
  d->idName = QString("");
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::onThreeDViewRadioClicked()
{
  this->setScreenshotType(0);
  this->grabScreenShot("ThreeD");
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::onRedSliceViewRadioClicked()
{
  this->setScreenshotType(1);
  this->grabScreenShot("Red");
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::onYellowSliceViewRadioClicked()
{
  this->setScreenshotType(2);
  this->grabScreenShot("Yellow");
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::onGreenSliceViewRadioClicked()
{
  this->setScreenshotType(3);
  this->grabScreenShot("Green");
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::onFullLayoutRadioClicked()
{
  this->setScreenshotType(4);
  this->grabScreenShot("");
}

void qMRMLScreenShotDialog::onScaleFactorSpinBoxChanged()
{
  Q_D(qMRMLScreenShotDialog);
  // check which screenshot should be grabbed
  if (d->fullLayoutRadio->isChecked())
    {
    // Fill Layout
    this->grabScreenShot("");
    }
  else if (d->threeDViewRadio->isChecked())
    {
    // TreeD
    this->grabScreenShot("ThreeD");
    }
  else if (d->redSliceViewRadio->isChecked())
    {
    // Red
    this->grabScreenShot("Red");
    }
  else if (d->yellowSliceViewRadio->isChecked())
    {
    // Yellow
    this->grabScreenShot("Yellow");
    }
  else if (d->greenSliceViewRadio->isChecked())
    {
    // Green
    this->grabScreenShot("Green");
    }
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::onNameEditChanged(const QString& newName)
{
  this->setNameEdit(newName);
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::onDescriptionChanged()
{
  Q_D(qMRMLScreenShotDialog);
  this->setDescription(d->descriptionTextEdit->toPlainText());
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::createConnection()
{
  Q_D(qMRMLScreenShotDialog);
  // connect the OK and CANCEL button to the individual Slots
  QObject::connect(this, SIGNAL(rejected()), this, SLOT(onDialogRejected()));
  QObject::connect(this, SIGNAL(accepted()), this, SLOT(onDialogAccepted()));

  QObject::connect(d->threeDViewRadio, SIGNAL(clicked()),
                  this, SLOT(onThreeDViewRadioClicked()));
  QObject::connect(d->redSliceViewRadio, SIGNAL(clicked()),
                  this, SLOT(onRedSliceViewRadioClicked()));
  QObject::connect(d->yellowSliceViewRadio, SIGNAL(clicked()),
                  this, SLOT(onYellowSliceViewRadioClicked()));
  QObject::connect(d->greenSliceViewRadio, SIGNAL(clicked()),
                  this, SLOT(onGreenSliceViewRadioClicked()));
  QObject::connect(d->fullLayoutRadio, SIGNAL(clicked()),
                  this, SLOT(onFullLayoutRadioClicked()));
  QObject::connect(d->scaleFactorSpinBox, SIGNAL(valueChanged(double)),
                  this, SLOT(onScaleFactorSpinBoxChanged()));

  // connect the name and the description to variable name and description
  QObject::connect(d->nameEdit, SIGNAL(textChanged(const QString&)),
                   this, SLOT(onNameEditChanged(const QString&)));
  QObject::connect(d->descriptionTextEdit, SIGNAL(textChanged()),
                   this, SLOT(onDescriptionChanged()));
}

//-----------------------------------------------------------------------------
void qMRMLScreenShotDialog::grabScreenShot(QString screenshotWindow)
{
  Q_D(qMRMLScreenShotDialog);
  QWidget* widget = 0;
  if (screenshotWindow.length() > 0)
    {
    if (screenshotWindow == "ThreeD")
      {
      // Create a scrennshot of the first 3DView
      widget = d->layoutManager.data()->threeDWidget(0)->threeDView();
      }
    else
      {
      // Create a screenshot of a specific sliceView
      widget = const_cast<ctkVTKSliceView*>(
          d->layoutManager.data()->sliceWidget(screenshotWindow)->sliceView());
      }
    }
  else
    {
    // Create a screenshot of the full layout
    widget = d->layoutManager.data()->viewport();
    }

  QImage screenShot = ctk::grabVTKWidget(widget);

  // set preview (unscaled)
  d->ScreenshotWidget->setPixmap(QPixmap::fromImage(screenShot));

  // Rescale the image which gets saved
  QImage rescaledScreenShot = screenShot.scaled(screenShot.size().width()
      * d->scaleFactorSpinBox->value(), screenShot.size().height()
      * d->scaleFactorSpinBox->value());

  // convert the screenshot from QPixmap to vtkImageData and store it with this class
  qMRMLUtils::qImageToVtkImageData(rescaledScreenShot, d->imageData);
}

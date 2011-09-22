#include "qSlicerAnnotationModuleSnapShotDialog.h"

#include "Logic/vtkSlicerAnnotationModuleLogic.h"

#include "qSlicerApplication.h"
#include "qSlicerLayoutManager.h"

// QT includes
#include <QDebug>
//-----------------------------------------------------------------------------
qSlicerAnnotationModuleSnapShotDialog::qSlicerAnnotationModuleSnapShotDialog()
{
  this->m_Logic = 0;
  this->setLayoutManager(qSlicerApplication::application()->layoutManager());
  this->setWindowTitle("Annotation Screenshot");
}

//-----------------------------------------------------------------------------
qSlicerAnnotationModuleSnapShotDialog::~qSlicerAnnotationModuleSnapShotDialog()
{
  if (this->m_Logic)
    {
    this->m_Logic = 0;
    }
}

//-----------------------------------------------------------------------------
void qSlicerAnnotationModuleSnapShotDialog::setLogic(vtkSlicerAnnotationModuleLogic* logic)
{
  if (!logic)
    {
    qErrnoWarning("setLogic: We need the Annotation module logic here!");
    return;
    }

  this->m_Logic = logic;
}

//-----------------------------------------------------------------------------
void qSlicerAnnotationModuleSnapShotDialog::initialize(const char* nodeId)
{
  if (!this->m_Logic)
    {
    qErrnoWarning("initialize: We need the Annotation module logic here!");
    return;
    }

  this->setLayoutManager(qSlicerApplication::application()->layoutManager());

  this->setID(QString(nodeId));
  // get the name..
  vtkStdString name = this->m_Logic->GetAnnotationName(this->id().toLatin1());

  // ..and set it in the GUI
  this->setNameEdit(QString(name));

  // get the description..
  vtkStdString description =
      this->m_Logic->GetSnapShotDescription(this->id().toLatin1());
  // ..and set it in the GUI
  this->setDescription(QString(description));

  // get the screenshot type..
  int screenshotType =
      this->m_Logic->GetSnapShotScreenshotType(this->id().toLatin1());

  // ..and set it in the GUI
  this->setScreenshotType(screenshotType);

  double scaleFactor =
      this->m_Logic->GetSnapShotScaleFactor(this->id().toLatin1());
  this->setScaleFactor(scaleFactor);

  this->initializeDialog();
}

//-----------------------------------------------------------------------------
void qSlicerAnnotationModuleSnapShotDialog::reset()
{
  QString name("Screenshot");
  // check to see if it's an already used name for a node (redrawing the
  // dialog causes it to reset and calling GetUniqueNameByString increments
  // the number each time).
  QByteArray nameBytes = name.toLatin1();
  vtkCollection *col = this->m_Logic->GetMRMLScene()->GetNodesByName(nameBytes.data());
  if (col->GetNumberOfItems() > 0)
    {
    // get a new unique name
    name = this->m_Logic->GetMRMLScene()->GetUniqueNameByString(name.toLatin1());
    qDebug() << name;
    }
  this->setNameEdit(name);
  this->resetDialog();
}

//-----------------------------------------------------------------------------
void qSlicerAnnotationModuleSnapShotDialog::onDialogRejected()
{
  // emit an event which gets caught by main GUI window
  emit dialogRejected();
}

//-----------------------------------------------------------------------------
void qSlicerAnnotationModuleSnapShotDialog::onDialogAccepted()
{
  // name
  QString name = this->nameEdit();
  QByteArray nameBytes = name.toLatin1();

  // description
  QString description = this->description();
  QByteArray descriptionBytes = description.toLatin1();

  // we need to know of which type the screenshot is
  int screenshotType = this->screenshotType();

  if (!strcmp(this->id().toLatin1(), ""))
    {
    // this is a new snapshot
    this->m_Logic->CreateSnapShot(nameBytes.data(),
                                  descriptionBytes.data(),
                                  screenshotType,
                                  this->scaleFactor(),
                                  this->imageData());
    }
  else
    {
    // this snapshot already exists
    this->m_Logic->ModifySnapShot(vtkStdString(this->id().toLatin1()),
                                  nameBytes.data(),
                                  descriptionBytes.data(),
                                  screenshotType,
                                  this->scaleFactor(),
                                  this->imageData());
    }

  // emit an event which gets caught by main GUI window
  emit dialogAccepted();
}


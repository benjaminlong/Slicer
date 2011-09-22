#include "qSlicerSceneViewsModuleDialog.h"

#include <vtkSlicerSceneViewsModuleLogic.h>

#include "qSlicerApplication.h"
#include "qSlicerLayoutManager.h"

// QT includes
#include <QDebug>

//-----------------------------------------------------------------------------
qSlicerSceneViewsModuleDialog::qSlicerSceneViewsModuleDialog()
{
  this->m_Logic = 0;
  this->setLayoutManager(qSlicerApplication::application()->layoutManager());
  this->setShowScaleFactorSpinBox(false);
  this->setWindowTitle("3D Slicer SceneView");
}

//-----------------------------------------------------------------------------
qSlicerSceneViewsModuleDialog::~qSlicerSceneViewsModuleDialog()
{

  if(this->m_Logic)
    {
    this->m_Logic = 0;
    }
}

//-----------------------------------------------------------------------------
void qSlicerSceneViewsModuleDialog::setLogic(vtkSlicerSceneViewsModuleLogic* logic)
{
  if (!logic)
    {
    qErrnoWarning("setLogic: We need the SceneViews module logic here!");
    return;
    }
  this->m_Logic = logic;
}

//-----------------------------------------------------------------------------
void qSlicerSceneViewsModuleDialog::initialize(const QString& nodeId)
{
  if (!this->m_Logic)
    {
    qErrnoWarning("initialize: We need the SceneViews module logic here!");
    return;
    }
  this->setLayoutManager(qSlicerApplication::application()->layoutManager());
  this->setID(nodeId);

  // get the name..
  vtkStdString name = this->m_Logic->GetSceneViewName(this->id().toLatin1());

  qDebug() << name;

  // ..and set it in the GUI
  this->setNameEdit(QString(name));

  // get the description..
  vtkStdString description = this->m_Logic->GetSceneViewDescription(this->id().toLatin1());

  // ..and set it in the GUI
  this->setDescription(QString(description));

  // get the screenshot type..
  int screenshotType = this->m_Logic->GetSceneViewScreenshotType(this->id().toLatin1());

  // ..and set it in the GUI
  this->setScreenshotType(screenshotType);

  this->setShowScaleFactorSpinBox(false);

  this->initializeDialog();
}

//-----------------------------------------------------------------------------
void qSlicerSceneViewsModuleDialog::reset()
{
  QString name("SceneView");
  // check to see if it's an already used name for a node (redrawing the
  // dialog causes it to reset and calling GetUniqueNameByString increments
  // the number each time).
  QByteArray nameBytes = name.toLatin1();
  vtkCollection *col = this->m_Logic->GetMRMLScene()->GetNodesByName(nameBytes.data());
  if (col->GetNumberOfItems() > 0)
    {
    // get a new unique name
    name = this->m_Logic->GetMRMLScene()->GetUniqueNameByString(name.toLatin1());
    }
  this->setNameEdit(name);
  this->resetDialog();
}


//-----------------------------------------------------------------------------
void qSlicerSceneViewsModuleDialog::onDialogRejected()
{
  emit dialogRejected();
}

//-----------------------------------------------------------------------------
void qSlicerSceneViewsModuleDialog::onDialogAccepted()
{
  // name
  QString name = this->nameEdit();
  QByteArray nameBytes = name.toLatin1();

  // description
  QString description = this->description();
  QByteArray descriptionBytes = description.toLatin1();

  // we need to know of which type the screenshot is
  int screenshotType = this->screenshotType();

  if (!strcmp(this->id().toLatin1(),""))
    {
    // this is a new SceneView
    this->m_Logic->CreateSceneView(nameBytes.data(),descriptionBytes.data(),
                                   screenshotType,this->imageData());
    //QMessageBox::information(this, "3D Slicer SceneView created",
    //             "A new SceneView was created and the current scene was attached.");
    }
  else
    {
    // this SceneView already exists
    this->m_Logic->ModifySceneView(vtkStdString(this->id().toLatin1()),nameBytes.data(),descriptionBytes.data()
                                   ,screenshotType,this->imageData());
    //QMessageBox::information(this, "3D Slicer SceneView updated",
    //             The SceneView was updated without changing the attached scene.");
    }
}

//-----------------------------------------------------------------------------
void qSlicerSceneViewsModuleDialog::onRestoreButtonClicked()
{
  this->m_Logic->RestoreSceneView(this->id().toLatin1());

  //QMessageBox::information(this, "3D Slicer SceneView updated",
  //                               "The SceneView was restored including the attached scene.");
  this->accept();
}

#include "NOXConstants.h"
#include "SMT.h"
#include "WindowEnum.h"
#include "VMUtils.h"
#include <QAction>
#include <QFileDialog>
#include <QTimer>

//
// Static definitions
//
SMT* SMT::TheInstance = 0;

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  Constructor
//
////////////////////////////////////////////////////////////////////////////////////////
SMT::SMT(QWidget *parent, void* authCB, void* crcCB, void* slaveCB) : 
  QMainWindow(parent),
  AuthenticateCB(authCB),
  Crc32CB(crcCB),
  SlaveCB(slaveCB)
{
  TheInstance = this;

  // Setup interface
  ui.setupUi(this);

  QAction* searchAction = new QAction();
  searchAction->setToolTip("Attach to all Nox Players");
  searchAction->setIcon(QIcon(":/SMT/Resources/Search-icon.png"));
  connect(searchAction, &QAction::triggered, this, &SMT::OnSearchTriggered);
  ui.ToolBar->addAction(searchAction);

  StatusAction = new QAction();
  StatusAction->setCheckable(true);
  StatusAction->setProperty("state", 0);
  StatusAction->setToolTip("Display status");
  StatusAction->setIcon(QIcon(":/SMT/Resources/Status-icon.png"));
  connect(StatusAction, &QAction::triggered, this, &SMT::OnStatusTriggered);
  ui.ToolBar->addAction(StatusAction);

  ui.ToolBar->addSeparator();

  QAction* loadAction = new QAction();
  loadAction->setToolTip("Load script");
  loadAction->setIcon(QIcon(":/SMT/Resources/script-icon.png"));
  connect(loadAction, &QAction::triggered, this, &SMT::OnOpenScriptTriggered);
  ui.ToolBar->addAction(loadAction);

  QAction* saveAction = new QAction();
  saveAction->setToolTip("Save script");
  saveAction->setIcon(QIcon(":/SMT/Resources/save-icon.png"));
  connect(saveAction, &QAction::triggered, this, &SMT::OnSaveScriptTriggered);
  ui.ToolBar->addAction(saveAction);

  RecordAction = new QAction();
  RecordAction->setCheckable(true);
  RecordAction->setProperty("state", 0);
  RecordAction->setToolTip("Record Macro");
  RecordAction->setIcon(QIcon(":/SMT/Resources/record-icon.png"));
  connect(RecordAction, &QAction::triggered, this, &SMT::OnRecordTriggered);
  ui.ToolBar->addAction(RecordAction);

  SleepAction = new QAction();
  SleepAction->setCheckable(true);
  SleepAction->setProperty("state", 0);
  SleepAction->setToolTip("Enable/Disable PC Sleep");
  SleepAction->setIcon(QIcon(":/SMT/Resources/zzz-icon.png"));
  connect(SleepAction, &QAction::triggered, this, &SMT::OnSleepTriggered);
  ui.ToolBar->addAction(SleepAction);

  ui.ToolBar->addSeparator();

  QAction* calibrateAction = new QAction();
  calibrateAction->setToolTip("Calibration Troubleshooting");
  calibrateAction->setIcon(QIcon(":/SMT/Resources/Utilities-icon.png"));
  connect(calibrateAction, &QAction::triggered, this, &SMT::OnCalibrateTriggered);
  ui.ToolBar->addAction(calibrateAction);

  // Setup custom signals->slots
  connect(this, &SMT::SigNotifyXYRGB, this, &SMT::OnNotifyXYRGB);

  // Initialize VM 
#ifndef _DEBUG
  VCPU_INIT();
  VCPU_START();
  VMUtils::InitializeQueues();
  VCPU_VALIDATE();

  // Setup Timer for slave heart-beat
  QTimer* timer = new QTimer();
  connect(timer, &QTimer::timeout, this, &SMT::OnSlaveQuery);
  timer->start(500);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  OnSearchTriggered
//
////////////////////////////////////////////////////////////////////////////////////////
void SMT::OnSearchTriggered()
{
  reinterpret_cast<void(*)()>(AuthenticateCB)();

  // Delete all existing threads.
  if (0 < ui.BotTabs->count())
  {
    StatusAction->trigger();

    // Kill the running thread for each controller.
    for (unsigned int i = 0; i < ui.BotTabs->count(); ++i)
    {
      BotWidget* bot = dynamic_cast<BotWidget*>(ui.BotTabs->widget(i));
      bot->KillController();
    }

    // Remove all tabs
    ui.BotTabs->clear();
  }

  // Create a tab for all existing NoxPlayer instances.
  std::vector<WndHierarchy> windows = WindowEnum::GetTargets("NoxPlayer");
  if (0 < windows.size())
  {
    bool hookupDebug = true;
    for (WndHierarchy wnd : windows)
    {
      // The widget will be in charge of adding itself to the tab widget
      // for simplicity.
      BotWidget* widget = new BotWidget(ui.BotTabs, &wnd, NotifyXYRGB);
      widget->setProperty("index", QVariant(ui.BotTabs->count() - 1).toUInt());
    }

    ui.BotTabs->setCurrentIndex(0);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  OnStatusTriggered
//
////////////////////////////////////////////////////////////////////////////////////////
void SMT::OnStatusTriggered()
{
  reinterpret_cast<void(*)()>(AuthenticateCB)();
  reinterpret_cast<void(*)()>(Crc32CB)();

  if (0 < ui.BotTabs->count())
  {
    unsigned int state = StatusAction->property("state").toUInt();
    StatusAction->setProperty("state", 1 == state ? 0 : 1);

    // Update all tabs
    for (unsigned int i = 0; i < ui.BotTabs->count(); ++i)
    {
      BotWidget* bot = dynamic_cast<BotWidget*>(ui.BotTabs->widget(i));
      bot->SetStatusVisible(0 == state ? true : false);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  OnOpenScriptTriggered
//
////////////////////////////////////////////////////////////////////////////////////////
void SMT::OnOpenScriptTriggered()
{
  reinterpret_cast<void(*)()>(AuthenticateCB)();

  if (0 < ui.BotTabs->count())
  {
    QString path = QFileDialog::getOpenFileName
      (this, "Load Script", QDir::currentPath(), "*.smts");
    if (0 != path.length())
    {
      dynamic_cast<BotWidget*>(ui.BotTabs->currentWidget())->LoadScript
        (path.toStdString().c_str());
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  OnSaveScriptTriggered
//
////////////////////////////////////////////////////////////////////////////////////////
void SMT::OnSaveScriptTriggered()
{
  reinterpret_cast<void(*)()>(AuthenticateCB)();

  if (0 < ui.BotTabs->count())
  {
    QString path = QFileDialog::getSaveFileName
                   (this, "Save Script", QDir::currentPath(), "*.smts");
    if (0 != path.length())
    {
      dynamic_cast<BotWidget*>(ui.BotTabs->currentWidget())->SaveScript
        (path.toStdString().c_str());
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  OnRecordTriggered
//
////////////////////////////////////////////////////////////////////////////////////////
void SMT::OnRecordTriggered()
{
  unsigned int state = RecordAction->property("state").toUInt();
  RecordAction->setProperty("state", 1 == state ? 0 : 1);

  if (1 == RecordAction->property("state").toUInt())
  {
    MessageBoxA(0, "Click OK when ready to record mouse clicks", "Macro Recorder", MB_OK);
    dynamic_cast<BotWidget*>(ui.BotTabs->currentWidget())->StartRecording();
  }
  else
  {
    dynamic_cast<BotWidget*>(ui.BotTabs->currentWidget())->StopRecording();
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  OnCalibrateTriggered
//
////////////////////////////////////////////////////////////////////////////////////////
void SMT::OnCalibrateTriggered()
{
  int response = MessageBoxA(0, "This function will calibrate pixel data for common "
                             "issues such as inability to login. Continue?",
                             "Warning",
                             MB_YESNO);
  if (IDYES == response)
  {
    dynamic_cast<BotWidget*>(ui.BotTabs->currentWidget())->Calibrate();
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  OnSleepTriggered
//
////////////////////////////////////////////////////////////////////////////////////////
void SMT::OnSleepTriggered()
{
  unsigned int state = SleepAction->property("state").toUInt();
  SleepAction->setProperty("state", 1 == state ? 0 : 1);
  
  if (0 == SleepAction->property("state").toUInt())
  {
    SetThreadExecutionState(ES_CONTINUOUS);
  }
  else
  {
    SetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  NotifyXYRGB
//
////////////////////////////////////////////////////////////////////////////////////////
void SMT::OnNotifyXYRGB(int x, int y,
                        unsigned char r, unsigned char g, unsigned char b)
{
  char debugPrint[50] = { 0 };
  sprintf_s(debugPrint, "(%d, %d, %d, %d, %d)", x, y, r, g, b);
  TheInstance->ui.StatusBar->showMessage(debugPrint);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  OnSlaveQuery
//
////////////////////////////////////////////////////////////////////////////////////////
void SMT::OnSlaveQuery()
{
  reinterpret_cast<void (*)()>(SlaveCB)();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  closeEvent
//
////////////////////////////////////////////////////////////////////////////////////////
void SMT::closeEvent(QCloseEvent* e)
{
  // Destroy all controllers
  if (0 < ui.BotTabs->count())
  {
    // Kill the running thread for each controller.
    for (unsigned int i = 0; i < ui.BotTabs->count(); ++i)
    {
      BotWidget* bot = dynamic_cast<BotWidget*>(ui.BotTabs->widget(i));
      bot->KillController();
    }

    // Remove all tabs
    ui.BotTabs->clear();
  }

  e->accept();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  NotifyXYRGB
//
////////////////////////////////////////////////////////////////////////////////////////
void SMT::NotifyXYRGB(int x, int y,
                      unsigned char r, unsigned char g, unsigned char b)
{
  emit TheInstance->SigNotifyXYRGB(x, y, r, g, b);
}
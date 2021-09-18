#include "BotWidget.h"
#include "MSMManager.h"
#include <fstream>
#include <QCheckBox>
#include <QMainWindow>
#include <QMessageBox>
#include <QLayout>
#include <QSplitter>
#include <QtWinExtras\qwinextrasglobal.h>
#include <QtWinExtras\qwinfunctions.h>

//
// Static definitions
//
BotWidget* BotWidget::TheInstance = 0;

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  SendXYRGB
//
////////////////////////////////////////////////////////////////////////////////////////
void BotWidget::SendXYRGB(int x, int y,
                          unsigned char r, unsigned char g, unsigned char b)
{
  QTabWidget* parent = dynamic_cast<QTabWidget*>(TheInstance->ParentWidget);
  if (0 != parent)
  {
    unsigned int index = TheInstance->property("index").toUInt();
    if ((index == parent->currentIndex()) &&
      (0 != TheInstance->CallbackXYRGB))
    {
      TheInstance->CallbackXYRGB(x, y, r, g, b);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  SetStatusVisible
//
////////////////////////////////////////////////////////////////////////////////////////
void BotWidget::SetStatusVisible(bool vis)
{
  DebugEdit->setVisible(vis);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  Constructor
//
////////////////////////////////////////////////////////////////////////////////////////
BotController* BotWidget::GetController() const
{
  return Controller;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  KillController
//
////////////////////////////////////////////////////////////////////////////////////////
void BotWidget::KillController()
{
  Controller->ExitThread();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  LoadScript
//
////////////////////////////////////////////////////////////////////////////////////////
void BotWidget::LoadScript(const char* path)
{
  std::ifstream inFile(path, std::ios::binary);
  if (false == inFile.is_open())
  {
    QMessageBox::warning(this, "Error", "Unable to load script");
    return;
  }

  unsigned int dataLength = 0;
  unsigned int dataBegin = inFile.tellg();
  inFile.seekg(0, std::ios::end);
  dataLength = static_cast<unsigned int>(inFile.tellg()) - dataBegin;
  inFile.seekg(0, std::ios::beg);

  // Read the file into a string buffer.
  std::string data;
  data.resize(dataLength);
  inFile.read(&data[0], dataLength);

  // Close the file handle
  inFile.close();

  // Update script
  BotScript->setText(QString::fromStdString(data));
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  SaveScript
//
////////////////////////////////////////////////////////////////////////////////////////
void BotWidget::SaveScript(const char* path)
{
  std::string filename = path;
  
  // Append the extension if it isn't already there.
  if (std::string::npos == filename.find(".smts"))
  {
    filename += ".smts";
  }

  std::ofstream outFile(filename.c_str(), std::ios::binary);
  if (false == outFile.is_open())
  {
    QMessageBox::warning(this, "Error", "Unable to save script");
    return;
  }

  std::string buffer = BotScript->toPlainText().toStdString();
  outFile.write(buffer.c_str(), buffer.length());
  outFile.close();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  Calibrate
//
////////////////////////////////////////////////////////////////////////////////////////
void BotWidget::Calibrate()
{
  SetForegroundWindow(Windows.handle); // Bring to front
  Controller->Calibrate();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  StartRecording
//
////////////////////////////////////////////////////////////////////////////////////////
void BotWidget::StartRecording()
{
  SetForegroundWindow(Windows.handle); // Bring to front
  Controller->StartRecording();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  StopRecording
//
////////////////////////////////////////////////////////////////////////////////////////
void BotWidget::StopRecording()
{
  BotScript->setText(Controller->StopRecording().c_str());
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  Constructor
//
////////////////////////////////////////////////////////////////////////////////////////
BotWidget::BotWidget(QWidget* parent, WndHierarchy* wnd, void* callback) :
  QWidget(parent),
  Controller(0),
  DebugEdit(0),
  ParentWidget(dynamic_cast<QTabWidget*>(parent)),
  CallbackXYRGB(reinterpret_cast<NotifyXYRGB>(callback))
{
  TheInstance = this;

  // Add this widget to the parent tab widget to adjust sizing.
  ParentWidget->addTab(this, wnd->name.c_str());

  // Setup a layout for this tab
  QVBoxLayout* mainLayout = new QVBoxLayout();
  mainLayout->setContentsMargins(QMargins(2, 2, 2, 2));
  mainLayout->setSpacing(2);
  setLayout(mainLayout);

  // Setup Script component
  BotScript = new QTextEdit(this);
  BotScript->setMinimumWidth(NOXConstants::SnapWindowWidth);
  BotScript->setMinimumHeight(NOXConstants::SnapWindowHeight);
  BotScript->setVisible(true);

  // Setup frame and components for screen
  QHBoxLayout* screenLayout = new QHBoxLayout();
  screenLayout->setContentsMargins(QMargins(0, 0, 0, 0));
  screenLayout->setSpacing(0);
  ScreenFrame = new QFrame();
  ScreenFrame->setLayout(screenLayout);
  Screen = new QLabel(this);
  Screen->setFrameShape(QFrame::Box);
  Screen->setMinimumWidth(NOXConstants::SnapWindowWidth);
  Screen->setMaximumWidth(NOXConstants::SnapWindowWidth);
  Screen->setMinimumHeight(NOXConstants::SnapWindowHeight);
  Screen->setMaximumHeight(NOXConstants::SnapWindowHeight);
  screenLayout->addWidget(Screen);

  // Setup a frame for state and button (initialize components)
  QFrame* controlFrame = new QFrame();
  QHBoxLayout* controlLayout = new QHBoxLayout();
  controlLayout->setContentsMargins(QMargins(0, 0, 0, 0));
  controlLayout->setSpacing(2); 
  controlFrame->setMaximumHeight(20);
  controlFrame->setLayout(controlLayout);
  StartButton = new QPushButton("Run Script", this);
  StartButton->setMinimumWidth(80);
  StartButton->setMaximumWidth(80);
  StartButton->setVisible(true);
  MinimalCheckbox = new QCheckBox("Snapshot View", 0);
  MinimalCheckbox->setChecked(true);
  connect(MinimalCheckbox, &QCheckBox::stateChanged,
          this, &BotWidget::OnMinimalViewStateChanged);
  controlLayout->addWidget(MinimalCheckbox, Qt::AlignLeft);
  controlLayout->addWidget(StartButton, Qt::AlignRight);

  // Setup final components of widget
  DebugEdit = new QTextEdit(this);
  DebugEdit->setMinimumWidth(NOXConstants::SnapWindowWidth);
  DebugEdit->setMinimumHeight(NOXConstants::SnapWindowHeight);
  DebugEdit->setVisible(false);
  StateLabel = new QLabel("Ready", this);
  StateLabel->setMinimumWidth(NOXConstants::SnapWindowWidth);
  StateLabel->setVisible(true);
  CommandEdit = new QLineEdit(this);
  connect(CommandEdit, &QLineEdit::returnPressed, this, &BotWidget::OnCommandEntered);

  // Add rest of components to the main layout
  mainLayout->addWidget(ScreenFrame);
  mainLayout->addWidget(BotScript);
  mainLayout->addWidget(controlFrame);
  mainLayout->addWidget(DebugEdit);
  mainLayout->addWidget(CommandEdit);
  mainLayout->addWidget(StateLabel);

  // Connect components to slots
  connect(StartButton, &QPushButton::clicked, this, &BotWidget::OnStartClick);
  connect(this, &BotWidget::NotifyStatusSignal, this, &BotWidget::OnNotifyStatus);
  connect(this, &BotWidget::NotifyDoneSignal, this, &BotWidget::OnNotifyDone);

  // Initialize syntax highlighter
  Syntax = new SyntaxHighlighter(BotScript->document());
  std::vector<QRegularExpression> commands =
  {
    QRegularExpression(QStringLiteral("\\bloginchar\\b")),
    QRegularExpression(QStringLiteral("\\blogout\\b")),
    QRegularExpression(QStringLiteral("\\bchangechannel\\b")),
    QRegularExpression(QStringLiteral("\\bguildattendance\\b")),
    QRegularExpression(QStringLiteral("\\bachievements\\b")),
    QRegularExpression(QStringLiteral("\\bcollectmail\\b")),
    QRegularExpression(QStringLiteral("\\bdailydungeon\\b")),
    QRegularExpression(QStringLiteral("\\bchaosdailydungeon\\b")),
    QRegularExpression(QStringLiteral("\\belitedungeon\\b")),
    QRegularExpression(QStringLiteral("\\bmulung\\b")),
    QRegularExpression(QStringLiteral("\\bmove\\b")),
    QRegularExpression(QStringLiteral("\\bskill\\b")),
    QRegularExpression(QStringLiteral("\\bquickslot\\b")),
    QRegularExpression(QStringLiteral("\\bchangepreset\\b")),
    QRegularExpression(QStringLiteral("\\bquest\\b")),
    QRegularExpression(QStringLiteral("\\bclick\\b")),
    QRegularExpression(QStringLiteral("\\bdrag\\b")),
    QRegularExpression(QStringLiteral("\\bswipe\\b")),
    QRegularExpression(QStringLiteral("\\bmousedown\\b")),
    QRegularExpression(QStringLiteral("\\bmouseup\\b")),
    QRegularExpression(QStringLiteral("\\bdelay\\b")),
    QRegularExpression(QStringLiteral("\\brepeat\\b")),
    QRegularExpression(QStringLiteral("\\bifpixel\\b")),
    QRegularExpression(QStringLiteral("\\bendif\\b")),
    QRegularExpression(QStringLiteral("\\bextenddelays\\b")),
    QRegularExpression(QStringLiteral("\\brunscript\\b")),
    QRegularExpression(QStringLiteral("\\bsnipeall\\b"))
  };
  Syntax->RegisterFunctions(commands);

  std::vector<QRegularExpression> constants =
  {
    QRegularExpression(QStringLiteral("\\bhell\\b")),
    QRegularExpression(QStringLiteral("\\bveryhard\\b")),
    QRegularExpression(QStringLiteral("\\bhard\\b")),
    QRegularExpression(QStringLiteral("\\bnormal\\b")),
    QRegularExpression(QStringLiteral("\\beasy\\b")),
    QRegularExpression(QStringLiteral("\\bconsume\\b")),
    QRegularExpression(QStringLiteral("\\bitem\\b")),
    QRegularExpression(QStringLiteral("\\bforge\\b")),
    QRegularExpression(QStringLiteral("\\bhunt\\b")),
    QRegularExpression(QStringLiteral("\\bdaily\\b")),
    QRegularExpression(QStringLiteral("\\belite\\b")),
    QRegularExpression(QStringLiteral("\\bml\\b")),
    QRegularExpression(QStringLiteral("\\bup\\b")),
    QRegularExpression(QStringLiteral("\\bdown\\b")),
    QRegularExpression(QStringLiteral("\\bleft\\b")),
    QRegularExpression(QStringLiteral("\\bright\\b")),
    QRegularExpression(QStringLiteral("\\bjump\\b")),
    QRegularExpression(QStringLiteral("\\battack\\b")),
    QRegularExpression(QStringLiteral("\\bdefault\\b")),
    QRegularExpression(QStringLiteral("\\bpersonal\\b"))
  };
  Syntax->RegisterParameters(constants);

  // Manually copy contents of wnd
  Windows.name = wnd->name;
  Windows.handle = wnd->handle;
  
  for (Wnd child : wnd->children)
  {
    Windows.children.push_back(child);
  }

  // Initialize bot. The window which takes input has different
  // names depending on the version of NOX.
  HWND macroWindow = WindowEnum::GetHandle(NOXConstants::MacroWindowName_0, &Windows);
  if (0 == macroWindow)
  {
    macroWindow = WindowEnum::GetHandle(NOXConstants::MacroWindowName_1, &Windows);
  }

  HWND graphicWindow = WindowEnum::GetHandle(NOXConstants::OpenGLPxWindowName, &Windows);
  if (0 == graphicWindow) // OpenGL not found
  {
    graphicWindow = WindowEnum::GetHandle(NOXConstants::DirectXPxWindowName,&Windows);
    
    if (0 == graphicWindow) // DirectX not found
    {
      QMessageBox::warning(this, "Error", "Unable to find NoxPlayer");
      return;
    }

    DebugEdit->append("DirectX Detected");
  }
  else
  {
    DebugEdit->append("OpenGL Detected");
  }

  Controller = new BotController(this, macroWindow, graphicWindow);
  Controller->RegisterNotifyXYRGB(SendXYRGB);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  OnStartClick
//
////////////////////////////////////////////////////////////////////////////////////////
void BotWidget::OnStartClick()
{
  if (0 == StartButton->text().compare("Run Script"))
  {
    // Split the text into separate strings per line
    QStringList qLines = BotScript->toPlainText().split("\n");

    // Fill a string array with the lines
    std::vector<std::string> lines;
    for (QString str : qLines)
    {
      // Ignore lines that are too short.
      if (2 >= str.length())
      {
        continue;
      }

      // Do not add commented lines.
      if (("/" != str[0]) && ("/" != str[1]))
      {
        lines.push_back(str.toStdString());
      }
    }

    // Pass the string array to the processor.
    std::vector<FunctionSet> functions;
    std::string error_str = MSMManager::BuildScript(lines, functions);
    if (0 != error_str.length())
    {
      QMessageBox::warning(this, "Error", QString::fromStdString(error_str));
      return;
    }

    // Do not update state if function set is empty.
    if (0 == functions.size())
    {
      return;
    }

    Controller->RegisterFunctionSet(functions);
    StartButton->setText("Stop");
  }
  else
  {
    Controller->ReleaseFunctionSet();
    StartButton->setText("Run Script");
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  Destructor
//
////////////////////////////////////////////////////////////////////////////////////////
BotWidget::~BotWidget()
{

}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  OnNotifyStatus
//
////////////////////////////////////////////////////////////////////////////////////////
void BotWidget::OnNotifyStatus(const char* status)
{
  if (0 != Controller)
  {
    QPixmap screenSnap = FromWinHBITMAP(Controller->GetPixelBitmap());
    Screen->setPixmap(screenSnap.scaled(NOXConstants::SnapWindowWidth,
                                        NOXConstants::SnapWindowHeight,
                                        Qt::KeepAspectRatio));
  }
    
  if (0 != strlen(status))
  {
    StateLabel->setText(status);
    DebugEdit->append(status);
  }

  delete[] status;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  OnNotifyDone
//
////////////////////////////////////////////////////////////////////////////////////////
void BotWidget::OnNotifyDone()
{
  if (0 == StartButton->text().compare("Stop"))
  {
    Controller->ReleaseFunctionSet();
    StartButton->setText("Run Script");
    StateLabel->setText("Done");
    DebugEdit->append("Done");
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  OnMinimalViewStateChanged
//
////////////////////////////////////////////////////////////////////////////////////////
void BotWidget::OnMinimalViewStateChanged(int state)
{
  switch (state)
  {
  case Qt::Checked:
    ScreenFrame->show();
    break;
  case Qt::Unchecked:
    ScreenFrame->hide();
    break;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  OnCommandEntered
//
////////////////////////////////////////////////////////////////////////////////////////
void BotWidget::OnCommandEntered()
{
  std::string command = CommandEdit->text().toStdString();
  CommandEdit->clear();

  std::vector<std::string> tokens = MSMManager::SplitSyntax(command, " ");
  if (0 < tokens.size())
  {
    Controller->CommandedFunction.params.clear();
    Controller->CommandedFunction.syntax = command;

    if (0 == tokens[0].compare("getpixel"))
    {
      // GETPIXEL requires two parameters
      if (2 < tokens.size())
      {
        Controller->CommandedFunction.params.push_back(atoi(tokens[1].c_str()));
        Controller->CommandedFunction.params.push_back(atoi(tokens[2].c_str()));
        Controller->CommandedFunction.function = FunctionIdEnum::GETPIXEL;
      }
    }
    else if (0 == tokens[0].compare("skipfunction"))
    {
      Controller->CommandedFunction.function = FunctionIdEnum::SKIP_FUNCITON;
    }
    else if (0 == tokens[0].compare("setfunction"))
    {
      // GETPIXEL requires two parameters
      if (1 < tokens.size())
      {
        Controller->CommandedFunction.params.push_back(atoi(tokens[1].c_str()) - 1);
        Controller->CommandedFunction.function = FunctionIdEnum::SET_FUNCTION;
      }
    }
    else if (0 == tokens[0].compare("experimental"))
    {
      // GETPIXEL requires two parameters
      if (1 < tokens.size())
      {
        Controller->CommandedFunction.params.push_back(atoi(tokens[1].c_str()));
        Controller->CommandedFunction.function = FunctionIdEnum::EXPERIMENTAL;
      }
    }
    else
    {
      DebugEdit->append(QString::fromStdString(command + " > Invalid Command"));
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  NotifyStatus
//
////////////////////////////////////////////////////////////////////////////////////////
void BotWidget::NotifyStatus(const char* status)
{
  emit NotifyStatusSignal(status);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  NotifyDone
//
////////////////////////////////////////////////////////////////////////////////////////
void BotWidget::NotifyDone()
{
  emit NotifyDoneSignal();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  FromWinHBITMAP
//
////////////////////////////////////////////////////////////////////////////////////////
QPixmap BotWidget::FromWinHBITMAP(HBITMAP bitmap)
{
  // Verify size
  BITMAP bitmap_info;
  memset(&bitmap_info, 0, sizeof(BITMAP));
  if (0 == GetObject(bitmap, sizeof(BITMAP), &bitmap_info))
  {
    qErrnoWarning("QPixmap::fromWinHBITMAP(), failed to get bitmap info");
    return QPixmap();
  }

  unsigned int width = bitmap_info.bmWidth;
  unsigned int height = bitmap_info.bmHeight;

  BITMAPINFO bmi;
  memset(&bmi, 0, sizeof(bmi));
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = -height;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage = width * height * sizeof(unsigned int);

  QImage result;
  unsigned char* data = new unsigned char[bmi.bmiHeader.biSizeImage];

  HDC display_dc = GetDC(0);
  if (0 != GetDIBits(display_dc, bitmap, 0, height, data, &bmi, DIB_RGB_COLORS))
  {
    // Create image and copy data into image.
    QImage image(width, height, QImage::Format_RGB32);
    if (false == image.isNull())
    {
      unsigned int bytes_per_line = width * sizeof(QRgb);
      for (unsigned int y = 0; y < height; ++y)
      {
        QRgb* dest = reinterpret_cast<QRgb*>(image.scanLine(y));
        QRgb* src = reinterpret_cast<QRgb*>(data + y * bytes_per_line);
        for (unsigned int x = 0; x < width; ++x)
        {
          dest[x] = src[x] | 0xff000000;
        }
      }
    }

    result = image;
  }

  delete[] data;
  ReleaseDC(0, display_dc);
  return QPixmap::fromImage(result);
}
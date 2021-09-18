#include "BotController.h"
#include "BotWidget.h"
#include "MSMManager.h"
#include <stdio.h>
#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  Calibrate
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::Calibrate()
{
  bool calibrated = false;

  int response = MessageBoxA(0, "Are you having issues logging in?", "Login", MB_YESNO);
  if (IDYES == response)
  {
    Manager->CalibrateLogin();
    calibrated = true;
  }

  response = MessageBoxA(0, "Are you having issues auto qesting?", "Quest", MB_YESNO);
  if (IDYES == response)
  {
    Manager->CalibrateQuest();
    calibrated = true;
  }

  if (true == calibrated)
  {
    Manager->GeneratePixelData(WindowWidth, WindowHeight);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  StartRecording
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::StartRecording()
{
  Recording = true;
  RecordScript.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  StopRecording
//
////////////////////////////////////////////////////////////////////////////////////////
std::string BotController::StopRecording()
{
  Recording = false;
  return RecordScript;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  MovePosition
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::MovePosition(PxXY xy) const
{
  PostMessage(MacroHandle, WM_MOUSEMOVE, 0, MAKELPARAM(xy.X, xy.Y));
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  MovePosition
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::MovePosition(int x, int y) const
{
  PostMessage(MacroHandle, WM_MOUSEMOVE, 0, MAKELPARAM(x, y));
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  LeftDown
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::LeftDown(PxXY xy) const
{
  PostMessage(MacroHandle, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(xy.X, xy.Y));
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  LeftDown
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::LeftDown(int x, int y) const
{
  PostMessage(MacroHandle, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  LeftUp
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::LeftUp(PxXY xy) const
{
  PostMessage(MacroHandle, WM_LBUTTONUP, 0, MAKELPARAM(xy.X, xy.Y));
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  LeftUp
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::LeftUp(int x, int y) const
{
  PostMessage(MacroHandle, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  LeftClick
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::LeftClick(PxXY xy, unsigned int count) const
{
  LeftClick(xy.X, xy.Y, count);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  LeftClick
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::LeftClick(int x, int y, unsigned int count) const
{
  for (unsigned int i = 0; i < count; ++i)
  {
    LeftDown(x, y);
    Delay(100);
    MovePosition(x, y);
    Sleep(100);
    LeftUp(x, y);
    Sleep(100);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  Drag
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::Drag(PxXY sXY, PxXY eXY) const
{
  Drag(sXY.X, sXY.Y, eXY.X, eXY.Y);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  Drag
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::Drag(int sX, int sY, int eX, int eY) const
{
  int currentX = sX;
  int currentY = sY;
  
  int deltaX = 0;
  if (sX != eX)
  {
    deltaX = (sX > eX ? -1 : 1);
  }

  int deltaY = sY;
  if (sY != eY)
  {
    deltaY = (sY > eY ? -1 : 1);
  }
 
  while ((currentX != eX) || (currentY != eY))
  {
    if (currentX != eX)
    {
      currentX += deltaX;
    }

    if (currentY != eY)
    {
      currentY += deltaY;
    }

    LeftDown(currentX, currentY);
    Delay(2);
  }

  Delay(500);

  LeftUp(currentX, currentY);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  Swipe
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::Swipe(PxXY xy, unsigned int direction) const
{
  Swipe(xy.X, xy.Y, direction);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  Swipe
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::Swipe(int x, int y, unsigned int direction) const
{
  int currentX = x;
  int currentY = y;
  
  // Get the change in X/Y depending on direction
  int deltaX = 0;
  int deltaY = 0;
  switch (direction)
  {
  case MSMConstants::MOVE_UP:
    deltaY = -1;
    break;
  case MSMConstants::MOVE_DOWN:
    deltaY = 1;
    break;
  case MSMConstants::MOVE_LEFT:
    deltaX = -1;
    break;
  case MSMConstants::MOVE_RIGHT:
    deltaX = 1;
    break;
  }

  for (unsigned int msg = 10; msg < 100; msg += 10)
  {
    currentX += deltaX * msg;
    currentY += deltaY * msg;
    LeftDown(currentX, currentY);
    Delay(10);
  }

  LeftUp(currentX, currentY);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  Delay
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::Delay(unsigned int ms) const
{
  std::this_thread::sleep_for(std::chrono::milliseconds(ms + ExtendedDelay));
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  ExitThread
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::ExitThread()
{
  Exit = true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  SendStatus
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::SendStatus(std::string status)
{
  char* message = new char[status.size() + 1];
  memset(message, 0, status.size() + 1);

  if (std::string::npos == LastStatus.find(status))
  {
    LastStatus = status; // Keep track of status updates..
    memcpy(message, status.data(), status.size());
  }

  reinterpret_cast<BotWidget*>(ParentWidget)->NotifyStatus(message);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  SendStatus
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::SendStatus(MSMConstants::InterfaceEnum substate, std::string syntax)
{
  std::string status(256, 0);
  sprintf(&status[0], "[%s] -> %s : %s",
          syntax.c_str(),
          Manager->GetCurrentStateString(),
          MSMConstants::InterfaceStrings[substate].c_str());
  SendStatus(status);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  GetPX
//
////////////////////////////////////////////////////////////////////////////////////////
PxXY BotController::GetPX(PxXY xy, bool refresh)
{
  unsigned int itr = (((WindowHeight * ScalingFactor) - xy.Y - 1) * 
                      (WindowWidth * ScalingFactor)) + xy.X;
  unsigned int index = itr * sizeof(unsigned int);

  // If refresh is specified, refresh our pixel data.
  if (true == refresh)
  {
    RefreshPixels();
  }

  return PxXY(xy.X, xy.Y, PixelPtr[index+2], PixelPtr[index + 1], PixelPtr[index]);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  GetPX
//
////////////////////////////////////////////////////////////////////////////////////////
bool BotController::LClickUntilChange(PxXY* pixels, 
                                      PxXY* pos, 
                                      unsigned int delay, 
                                      bool match)
{
  bool safeExit = true;
  PxXY pixelRef = *pixels;

  unsigned int try_count = 0;
  while (match == (pixelRef == GetPX(pixelRef)))
  {
    LeftClick(*pos);
    Delay(delay);

    ++try_count;
    if (try_count > 5)
    {
      safeExit = false;
      break;
    }
  }

  return safeExit;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  Random
//
////////////////////////////////////////////////////////////////////////////////////////
unsigned int BotController::Random(unsigned int min, unsigned int max)
{
  static bool initialized = false;
  if (false == initialized)
  {
    initialized = true;
    srand(time(0));
  }

  return min + (rand() % (max - min));
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  RegisterNotifyXYRGB
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::RegisterNotifyXYRGB(void* cbFunc)
{
  CallbackXYRGB = reinterpret_cast<NotifyXYRGB>(cbFunc);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  RegisterFunctionSet
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::RegisterFunctionSet(std::vector<FunctionSet> functions)
{
  // Reset the state and sub state
  if (0 != Manager)
  {
    SubState = MSMConstants::INTERFACE_MAX;
    Manager->ResetCurrentState();
  }

  Functions = functions;
  FunctionCount = Functions.size();
  Counter = 0;

  // Reset all function counters
  for (unsigned int i = 0; i < Functions.size(); ++i)
  {
    Functions[i].counter = 0;
    Functions[i].errors = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  ReleaseFunctionSet
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::ReleaseFunctionSet()
{
  FunctionCount = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  Constructor
//
////////////////////////////////////////////////////////////////////////////////////////
HBITMAP BotController::GetPixelBitmap() const
{
  return PixelBitmap;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  Constructor
//
////////////////////////////////////////////////////////////////////////////////////////
BotController::BotController(void* parent, HWND macroHandle, HWND graphicHandle) :
  ParentWidget(parent),
  MacroHandle(macroHandle),
  GraphicsWnd(graphicHandle),
  GraphicsHandle(GetDC(GraphicsWnd)),
  Frequency(250),
  ExtendedDelay(0),
  ScalingFactor(1.0f),
  Exit(false),
  Recording(false),
  Experimental(false),
  LastStatus(""),
  Counter(0),
  FunctionCount(0),
  Manager(0),
  SubState(MSMConstants::INTERFACE_MAX)
{
  // Launch the Bot thread
  ThreadHandle = std::async(std::launch::async, 
                            &BotController::BotMain, 
                            static_cast<void*>(this));
                            
  // Initialize all other parameters
  GetWindowRect(MacroHandle, &WindowRect);
  WindowWidth = WindowRect.right - WindowRect.left;
  WindowHeight = WindowRect.bottom - WindowRect.top;

  // Check for odd window sizing based on our acceptible resolutions
  unsigned int errorWidth = 
    abs(static_cast<int>(NOXConstants::ReqWindowWidth) -
        static_cast<int>(WindowWidth));
  unsigned int errorHeight = 
    abs(static_cast<int>(NOXConstants::ReqWindowHeight) -
        static_cast<int>(WindowHeight));
  if ((0 != errorWidth) || (0 != errorHeight))
  {
    if (50 >= (errorWidth + errorHeight))
    {
      while (WindowWidth != NOXConstants::ReqWindowWidth)
      {
        char message[215] = { 0 };
        sprintf_s(message, 
                  "Adjust NOXPlayer Window width, "
                  "current width: %d, expected width: %d, Click OK when ready",
                  WindowRect.right - WindowRect.left, NOXConstants::ReqWindowWidth);
        MessageBoxA(0, message, "Resolution Error", MB_OK);

        // Update window size
        GetWindowRect(MacroHandle, &WindowRect);
        WindowWidth = WindowRect.right - WindowRect.left;
        WindowHeight = WindowRect.bottom - WindowRect.top;
      }

      WindowWidth = NOXConstants::ReqWindowWidth;
      WindowHeight = NOXConstants::ReqWindowHeight;
    }
    else
    {
      errorWidth = 
        abs(static_cast<int>(NOXConstants::ReqWindowWidthOpt) -
            static_cast<int>(WindowWidth));
      errorHeight = 
        abs(static_cast<int>(NOXConstants::ReqWindowHeightOpt) -
            static_cast<int>(WindowHeight));
      if ((0 != errorWidth) || (0 != errorHeight))
      {
        if (50 >= (errorWidth + errorHeight))
        {
          while (WindowWidth != NOXConstants::ReqWindowWidthOpt)
          {
            char message[215] = { 0 };
            sprintf_s(message,
                      "Adjust NOXPlayer Window width, "
                      "current width: %d, expected width: %d, Click OK when ready",
                      WindowRect.right - WindowRect.left, NOXConstants::ReqWindowWidthOpt);
            MessageBoxA(0, message, "Resolution Error", MB_OK);

            // Update window size
            GetWindowRect(MacroHandle, &WindowRect);
            WindowWidth = WindowRect.right - WindowRect.left;
            WindowHeight = WindowRect.bottom - WindowRect.top;
          }

          WindowWidth = NOXConstants::ReqWindowWidthOpt;
          WindowHeight = NOXConstants::ReqWindowHeightOpt;
        }
        else
        {
          SendStatus("[Error] Experiment resolution set (may not work..)");
          ScalingFactor = (WindowWidth > NOXConstants::ReqWindowWidth ? 1.25f : 0.75f);
          WindowWidth = NOXConstants::ReqWindowWidth;
          WindowHeight = NOXConstants::ReqWindowHeight;
        }
      }
    }
  }

  // Initialize MSM Manager and correct pixel data if applicable.
  Manager = new MSMManager(this, WindowWidth, WindowHeight);

  // Check to see if the corrections file is in the same directory,
  // and if so, correct pixel data.
  char path[MAX_PATH] = { 0 };
  GetCurrentDirectoryA(MAX_PATH, path);
  sprintf_s(path, "%s\\corrections%dx%d.cpx", path, WindowWidth, WindowHeight);

  if (true == Manager->CorrectPixelData(path))
  {
    SendStatus("[!] Data correction applied");
  }

  // Initialize pixel detection device context
  PixelHandle = CreateCompatibleDC(GraphicsHandle);

  // Notify UI of dimensions
  char status[256];
  sprintf_s(status, "Dimensions: %dx%d X=%d Y=%d",
            WindowWidth, WindowHeight, WindowRect.left, WindowRect.top);
  SendStatus(status);

  // Create device independent bitmap for pixel detection.
  BITMAPINFO bmi;
  bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
  bmi.bmiHeader.biWidth = WindowWidth;
  bmi.bmiHeader.biHeight = WindowHeight;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage = bmi.bmiHeader.biWidth *
                              bmi.bmiHeader.biHeight *
                              sizeof(unsigned int);
  bmi.bmiHeader.biClrUsed = 0;
  bmi.bmiHeader.biClrImportant = 0;

  PixelBitmap = CreateDIBSection(PixelHandle, &bmi, DIB_RGB_COLORS,
                                 reinterpret_cast<void**>(&PixelPtr), 0, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  Destructor
//
////////////////////////////////////////////////////////////////////////////////////////
BotController::~BotController()
{
  DeleteObject(SelectObject(PixelHandle, OldObject));
  ReleaseDC(GraphicsWnd, GraphicsHandle);
  ReleaseDC(GraphicsWnd, PixelHandle);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  GetXYRGB
//
////////////////////////////////////////////////////////////////////////////////////////
PxXY BotController::GetXYRGB()
{
  POINT pos;
  GetCursorPos(&pos);

  PxXY xyRGB(0, 0, 0);
  if (TRUE == ScreenToClient(GraphicsWnd, &pos))
  {
    if ((0 <= pos.x) &&
        (0 <= pos.y) &&
        (WindowWidth >= pos.x) &&
        (WindowHeight >= pos.y))
    {
      xyRGB.X = pos.x;
      xyRGB.Y = pos.y;
      xyRGB = GetPX(xyRGB);
    }
  }

  return xyRGB;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  RefreshPixels
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::RefreshPixels()
{
  static bool firstFail = true;

  // Retrieve pixel data for bot
  OldObject = SelectObject(PixelHandle, PixelBitmap);
  if (0 == BitBlt(PixelHandle, 
                  0, 
                  0,
                  WindowWidth, 
                  WindowHeight,
                  GraphicsHandle, 0, 0, SRCCOPY))
  {
    if (false == firstFail)
    {
      SendStatus("[!] Failed retrieving data");
      return;
    }

    firstFail = true;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  BotMain (Thread)
//
////////////////////////////////////////////////////////////////////////////////////////
void BotController::BotMain(void* instance)
{
  PxXY xyRGB;
  char status[256] = { 0 };

  BotController* controller = reinterpret_cast<BotController*>(instance);
  if (0 != controller)
  {
    // Subscript function set
    std::string subPath;
    unsigned int subCounter = 0;
    std::vector<FunctionSet> subFunctions;
    std::vector<std::string> subFuncSyntax;

    // Recording members
    unsigned int totalDelay = 0;

    // Begin main bot loop
    while (false == controller->Exit)
    {
      // Take a snapshot of the window
      controller->RefreshPixels();

      if (0 != controller->Functions.size())
      {
        if (0 == controller->FunctionCount)
        {
          // Clear functions here so we don't have an exception
          controller->Counter = 0;
          controller->Functions.clear();
          subFunctions.clear();
          subCounter = 0;
          continue;
        }

        if (controller->Counter < controller->Functions.size())
        {
          bool skipStatus = false;

          FunctionSet* func = &controller->Functions[controller->Counter];
          switch (func->function)
          {
          case FunctionIdEnum::RUN_SCRIPT:
            skipStatus = true;

            // Handle subscript processing...
            if (0 == subFunctions.size())
            {
              subPath = MSMManager::SplitSyntax(func->syntax, "(,)")[1];
              if (false == MSMManager::ReadScriptFile(subPath, subFuncSyntax))
              {
                ++controller->Counter;
                controller->SendStatus("[" + func->syntax + "] Failed loading");
              }
              else
              {
                if (0 == MSMManager::BuildScript(subFuncSyntax, subFunctions).length())
                {
                  controller->SendStatus("[" + func->syntax + "] Running script");
                }
                else
                {
                  ++controller->Counter;
                  controller->SendStatus("[" + func->syntax + "] Error in script");
                  subFunctions.clear();
                }
              }
            }
            else // Script built, process it
            {
              if (true == controller->ProcessSubFunctions(subFunctions, subCounter))
              {
                ++controller->Counter;
                controller->SendStatus("[" + func->syntax + "] Complete ");
                subFunctions.clear();
                subCounter = 0;
              }
            }
            
            break;
          // Functions that are experimental
          case FunctionIdEnum::SNIPE_ALL:
            if (false == controller->Experimental)
            {
              ++controller->Counter;
              break;
            }

          // Functions enabled by default
          case FunctionIdEnum::LOGIN_CHAR:
          case FunctionIdEnum::LOGOUT:
          case FunctionIdEnum::CHANGE_PRESET:
          case FunctionIdEnum::CHANGE_CHANNEL:
          case FunctionIdEnum::GUILD_ATTENDANCE:
          case FunctionIdEnum::COLLECT_ACHIEVEMENTS:
          case FunctionIdEnum::COLLECT_MAIL:
          case FunctionIdEnum::CHAOS_DAILY_DUNGEON:
          case FunctionIdEnum::DAILY_DUNGEON:
          case FunctionIdEnum::ELITE_DUNGEON:
          case FunctionIdEnum::MU_LUNG:
          case FunctionIdEnum::MOVE:
          case FunctionIdEnum::SKILL:
          case FunctionIdEnum::QUICKSLOT:
          case FunctionIdEnum::QUEST:
            skipStatus = true;
            if (true == controller->Manager->ProcessMSMFunction(func, 
                                                                controller->SubState))
            {
              ++controller->Counter;
            }
            break;
          case FunctionIdEnum::CLICK:
            controller->LeftClick(func->params[0], func->params[1], 
                                  (2 < func->params.size() ? func->params[2] : 1));
            ++controller->Counter;
            break;
          case FunctionIdEnum::MOUSE_DOWN:
            controller->LeftDown(func->params[0], func->params[1]);
            ++controller->Counter;
            break;
          case FunctionIdEnum::MOUSE_UP:
            controller->LeftUp(func->params[0], func->params[1]);
            ++controller->Counter;
            break;
          case FunctionIdEnum::SWIPE:
            controller->Swipe(func->params[0], func->params[1], func->params[2]);
            ++controller->Counter;
            break;
          case FunctionIdEnum::DRAG:
            controller->Drag(func->params[0], func->params[1], func->params[2], func->params[3]);
            ++controller->Counter;
            break;
          case FunctionIdEnum::DELAY:
            controller->Delay(func->params[0]);
            ++controller->Counter;
            break;
          case FunctionIdEnum::REPEAT:
            skipStatus = true;

            ++func->counter;
            if (func->counter >= func->params[1])
            {
              func->counter = 0;
              func->errors = 0;
              ++controller->Counter;
            }
            else
            {
              // Set controller counter to the loop function 
              controller->Counter = func->params[0];
              
              // Reset the upcoming functions counter
              controller->Functions[func->params[0]].counter = 0;

              // Update status
              controller->SendStatus("[" + func->syntax + "] " +
                                     std::to_string(func->params[1] - func->counter) +
                                     " remaining");
            }
            break;
          case FunctionIdEnum::IFPIXEL:
          // Validate conditional statement.
          {
            PxXY px(func->params[0],  // x
                    func->params[1],  // y
                    func->params[2],  // r
                    func->params[3],  // g
                    func->params[4]); // b

            // If the pixels match, update counter by 1.
            if (px == controller->GetPX(px))
            {
              ++controller->Counter;
            }
            else
            {
              // The pixels didn't match, keep increasing counter
              // until a non-reliant function is found.
              for (unsigned int idx = controller->Counter; 
                   idx < controller->FunctionCount; 
                   ++idx)
              {
                ++controller->Counter;
                if (-1 == controller->Functions[idx].reliantFunction)
                {
                  // Step back one count
                  --controller->Counter;
                  break;
                }
              }
            }
          }
          break;
          case FunctionIdEnum::EXTEND_DELAYS:
            controller->ExtendedDelay = func->params[0];
            break;
          default:
            abort(); // Invalid function
          }

          // Update user interface state
          if (false == skipStatus)
          {
            sprintf_s(status, "[%s]", func->syntax.c_str());
            controller->SendStatus(status);
          }
        }
        else
        {
          // Alert the interface that the script is complete.
          reinterpret_cast<BotWidget*>(controller->ParentWidget)->NotifyDone();
        }
      }

      // Process all command functions
      switch (controller->CommandedFunction.function)
      {
      case FunctionIdEnum::GETPIXEL:
        controller->CommandedFunction.function = FunctionIdEnum::NO_FUNCTION;
        xyRGB.X = controller->CommandedFunction.params[0];
        xyRGB.Y = controller->CommandedFunction.params[1];
        xyRGB = controller->GetPX(xyRGB);
        sprintf_s(status, "%s> [%d, %d, %d]",
                  controller->CommandedFunction.syntax.c_str(), 
                  xyRGB.R, xyRGB.G, xyRGB.B);
        controller->SendStatus(status);
        break;
      case FunctionIdEnum::SKIP_FUNCITON:
        controller->CommandedFunction.function = FunctionIdEnum::NO_FUNCTION;
        sprintf_s(status, "%s> Skipping function = %s",
                  controller->CommandedFunction.syntax.c_str(),
                  controller->Functions[controller->Counter].syntax.c_str());
        controller->SendStatus(status);
        ++controller->Counter;
        break;
      case FunctionIdEnum::SET_FUNCTION:
        controller->CommandedFunction.function = FunctionIdEnum::NO_FUNCTION;
        controller->Counter = controller->CommandedFunction.params[0];
        sprintf_s(status, "%s> Jumping to function = %s",
                  controller->CommandedFunction.syntax.c_str(),
                  controller->Functions[controller->Counter].syntax.c_str());
        controller->SendStatus(status);
        break;
      case FunctionIdEnum::EXPERIMENTAL:
        controller->CommandedFunction.function = FunctionIdEnum::NO_FUNCTION;
        controller->Experimental = (1 == controller->CommandedFunction.params[0]);
        sprintf_s(status, "%s> Experimental Functions %s",
                  controller->CommandedFunction.syntax.c_str(),
                  true == controller->Experimental ? "Enabled" : "Disabled");
        controller->SendStatus(status);
        break;
      }

      // Retrieve the XY Coordinates and Pixel. Notify the Interface
      // of the values.
      if (0 != GetAsyncKeyState(VK_RBUTTON))
      {
        xyRGB = controller->GetXYRGB();
        if (0 != controller->CallbackXYRGB)
        {
          controller->CallbackXYRGB(xyRGB.X, xyRGB.Y, xyRGB.R, xyRGB.G, xyRGB.B);
        }
      }

      // If we are recording, look for the left mouse click
      if (true == controller->Recording)
      {
        if (0 != GetAsyncKeyState(VK_LBUTTON))
        {
          xyRGB = controller->GetXYRGB();
          if (0 != totalDelay)
          {
            controller->RecordScript += "delay(" + std::to_string(totalDelay) + ")\n";
            totalDelay = 0;
          }

          controller->RecordScript += "click(" + std::to_string(xyRGB.X) + "," + 
                                                 std::to_string(xyRGB.Y) + ")\n";
        }

        totalDelay += controller->Frequency;
      }

      // Sleep
      controller->Delay(controller->Frequency);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function:  ProcessSubFunctions
//
////////////////////////////////////////////////////////////////////////////////////////
bool BotController::ProcessSubFunctions(std::vector<FunctionSet>& functions,
                                        unsigned int& counter)
{
  bool skipStatus = false;

  FunctionSet* func = &functions[counter];
  switch (func->function)
  {
  case FunctionIdEnum::LOGIN_CHAR:
  case FunctionIdEnum::LOGOUT:
  case FunctionIdEnum::CHANGE_PRESET:
  case FunctionIdEnum::CHANGE_CHANNEL:
  case FunctionIdEnum::GUILD_ATTENDANCE:
  case FunctionIdEnum::COLLECT_ACHIEVEMENTS:
  case FunctionIdEnum::COLLECT_MAIL:
  case FunctionIdEnum::CHAOS_DAILY_DUNGEON:
  case FunctionIdEnum::DAILY_DUNGEON:
  case FunctionIdEnum::ELITE_DUNGEON:
  case FunctionIdEnum::MU_LUNG:
  case FunctionIdEnum::MOVE:
  case FunctionIdEnum::SKILL:
  case FunctionIdEnum::QUICKSLOT:
  case FunctionIdEnum::QUEST:
    skipStatus = true;
    if (true == Manager->ProcessMSMFunction(func, SubState))
    {
      ++counter;
    }
    break;
  case FunctionIdEnum::CLICK:
    LeftClick(func->params[0], func->params[1], 
              (2 < func->params.size() ? func->params[2] : 1));
    ++counter;
    break;
  case FunctionIdEnum::MOUSE_DOWN:
    LeftDown(func->params[0], func->params[1]);
    ++counter;
    break;
  case FunctionIdEnum::MOUSE_UP:
    LeftUp(func->params[0], func->params[1]);
    ++counter;
    break;
  case FunctionIdEnum::SWIPE:
    Swipe(func->params[0], func->params[1], func->params[2]);
    ++counter;
    break;
  case FunctionIdEnum::DRAG:
    Drag(func->params[0], func->params[1], func->params[2], func->params[3]);
    ++counter;
    break;
  case FunctionIdEnum::DELAY:
    Delay(func->params[0]);
    ++counter;
    break;
  case FunctionIdEnum::REPEAT:
    skipStatus = true;

    ++func->counter;
    if (func->counter >= func->params[1])
    {
      func->counter = 0;
      func->errors = 0;
      ++counter;
    }
    else
    {
      // Set controller counter to the loop function 
      counter = func->params[0];

      // Reset the upcoming functions counter
      functions[func->params[0]].counter = 0;

      // Update status
      SendStatus("[" + func->syntax + "] " +
                 std::to_string(func->params[1] - func->counter) +
                 " remaining");
    }
    break;
  case FunctionIdEnum::IFPIXEL:
    // Validate conditional statement.
  {
    PxXY px(func->params[0],  // x
            func->params[1],  // y
            func->params[2],  // r
            func->params[3],  // g
            func->params[4]); // b

    // If the pixels match, update counter by 1.
    if (px == GetPX(px))
    {
      ++counter;
    }
    else
    {
      // The pixels didn't match, keep increasing counter
      // until a non-reliant function is found.
      for (unsigned int idx = counter; idx < functions.size(); ++idx)
      {
        ++counter;
        if (-1 == functions[idx].reliantFunction)
        {
          // Step back one count
          --counter;
          break;
        }
      }
    }
  }
  break;
  }

  // Update user interface state
  if (false == skipStatus)
  {
    char status[256] = { 0 };
    sprintf_s(status, "[%s]", func->syntax.c_str());
    SendStatus(status);
  }

  // Return true if the counter meets or exceeds our count
  if (counter >= functions.size())
  {
    return true;
  }

  // Subscript not done
  return false;
}
#pragma once
#include "FunctionDefines.h"
#include "MSMManager.h"
#include "NOXConstants.h"
#include "PxXY.h"
#include <future>
#include <thread>
#include <Windows.h>

class BotController
{
public:
  void Calibrate();
  void StartRecording();
  std::string StopRecording();
  void MovePosition(PxXY xy) const;
  void MovePosition(int x, int y) const;
  void LeftDown(PxXY xy) const;
  void LeftDown(int x, int y) const;
  void LeftUp(PxXY xy) const;
  void LeftUp(int x, int y) const;
  void LeftClick(PxXY xy, unsigned int count = 1) const;
  void LeftClick(int x, int y, unsigned int count = 1) const;
  void Drag(PxXY sXY, PxXY eXY) const;
  void Drag(int sX, int sY, int eX, int eY) const;
  void Swipe(PxXY xy, unsigned int direction) const;
  void Swipe(int x, int y, unsigned int direction) const;
  void Delay(unsigned int ms) const;
  void ExitThread();

  void SendStatus(std::string status);
  void SendStatus(MSMConstants::InterfaceEnum state,  std::string syntax);
  PxXY GetPX(PxXY xy, bool refresh = true);
  bool LClickUntilChange(PxXY* pixels, PxXY* pos, unsigned int delay, bool match = true);
  static unsigned int Random(unsigned int min, unsigned int max);
  void RegisterNotifyXYRGB(void* cbFunc);
  void RegisterFunctionSet(std::vector<FunctionSet> functions);
  void ReleaseFunctionSet();
  void RefreshPixels();
  HBITMAP GetPixelBitmap() const;

  BotController(void* parent, HWND macroHandle, HWND graphicHandle);
  ~BotController();

  FunctionSet CommandedFunction;

private:
  PxXY GetXYRGB();
  static void BotMain(void* instance);
  bool ProcessSubFunctions(std::vector<FunctionSet>& functions, 
                           unsigned int& counter);

  // Callback functions
  typedef void (*NotifyXYRGB)(int x, int y, unsigned char, unsigned char, unsigned char);
  NotifyXYRGB CallbackXYRGB;

  // Members
  RECT WindowRect;
  void* ParentWidget;
  unsigned int WindowWidth;
  unsigned int WindowHeight;
  HWND MacroHandle;
  HWND GraphicsWnd;
  HDC GraphicsHandle;
  HDC PixelHandle;  
  HBITMAP PixelBitmap;
  HGDIOBJ OldObject;
  unsigned char* PixelPtr;
  float ScalingFactor;

  // Processing members
  bool Exit;
  bool Recording;
  bool Experimental;
  std::string RecordScript;
  std::string LastStatus;
  unsigned int Counter;
  unsigned int FunctionCount;
  MSMManager* Manager;
  MSMConstants::InterfaceEnum SubState;
  std::vector<FunctionSet> Functions;
  unsigned int Frequency;
  unsigned int ExtendedDelay;
  std::future<void> ThreadHandle;
};


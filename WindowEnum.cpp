#include "WindowEnum.h"

//
// Static definitions
//
unsigned int WindowEnum::WndLevel;
std::string WindowEnum::WindowName;
std::vector<WndHierarchy> WindowEnum::Handles;

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: GetTargets
//
////////////////////////////////////////////////////////////////////////////////////////
std::vector<WndHierarchy> WindowEnum::GetTargets(const char* name)
{
  // Clear handles and state
  Handles.clear();
  WndLevel = 0;

  // Store target name(s) for callback function.
  WindowName = std::string(name);

  // Enumerate top level windows
  EnumWindows(WindowCallback, 0);

  // Search for all children of each parent window.
  for (WndHierarchy wkv : Handles)
  {
    ++WndLevel;
    EnumChildWindows(wkv.handle, WindowCallback, 0);
  }

  return Handles;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: GetHandle
//
////////////////////////////////////////////////////////////////////////////////////////
HWND WindowEnum::GetHandle(std::string name, WndHierarchy* wnds)
{
  HWND handle = 0;

  // Check the parent name first
  if (0 == wnds->name.compare(name))
  {
    handle = wnds->handle;
  }
  else // Check children
  {
    for (Wnd wnd : wnds->children)
    {
      if (std::string::npos != wnd.name.find(name))
      {
        handle = wnd.handle;
        break;
      }
    }
  }

  return handle;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Function: WindowCallback
//
////////////////////////////////////////////////////////////////////////////////////////
BOOL __stdcall WindowEnum::WindowCallback(HWND handle, LPARAM param)
{
  if (TRUE == IsWindowVisible(handle))
  {
    // Retrieve the length of the window name, < 0 means failure.
    int nameLen = GetWindowTextLength(handle);
    if (0 < nameLen)
    {
      // Retrieve the window name.
      std::string windowName;
      windowName.resize(nameLen + 1);
      GetWindowTextA(handle, &windowName[0], nameLen + 1);

      if (0 == WndLevel)
      {
        // Compare the window name to target window name and add
        // matches (non-exact) to the container.
        if (std::string::npos != windowName.find(WindowName))
        {
          WndHierarchy wnd;
          wnd.handle = handle;
          wnd.name = windowName;
          Handles.push_back(wnd);

          std::string className(25, 0);
          GetClassNameA(handle, &className[0], 25);
        }
      }
      else // Child call, find the parent and add it.
      {
        Wnd wnd;
        wnd.name = windowName;
        wnd.name.resize(windowName.size());
        wnd.handle = handle;
        Handles[WndLevel - 1].children.push_back(wnd);
      }
    }
  }

  return TRUE;
}
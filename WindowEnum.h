#pragma once
#include <string>
#include <vector>
#include <Windows.h>

// Structure definition for finding parents/child windows
struct Wnd
{
  HWND handle;
  std::string name;
};

struct WndHierarchy
{
  HWND handle;
  std::string name;
  std::vector<Wnd> children;
};

// Class definition
class WindowEnum
{
public:
  static std::vector<WndHierarchy> GetTargets(const char* name);
  static HWND GetHandle(std::string name, WndHierarchy* wnds);

private:
  static BOOL __stdcall WindowCallback(HWND handle, LPARAM param);

  static unsigned int WndLevel;
  static std::string WindowName;
  static std::string ClassName;
  static std::vector<WndHierarchy> Handles;
};


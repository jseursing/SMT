#include "SMT.h"
#include "VMUtils.h"
#include <QtWidgets/QApplication>

// Forward declarations
extern void InitializeVM();
extern void ValidateUIDCallback();
extern void VMLCRC32Callback();
extern void VMLHBCallback();

//
// Entrypoint
//
int main(int argc, char *argv[])
{
  InitializeVM();
  ValidateUIDCallback();
  VMLCRC32Callback();

  QApplication a(argc, argv);
  SMT w(0, ValidateUIDCallback, VMLCRC32Callback, VMLHBCallback);
  w.show();
  return a.exec();
}

#include "SMT.h"
#include <QtWidgets/QApplication>

//
// Entrypoint
//
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  SMT w(0, ValidateUIDCallback, VMLCRC32Callback, VMLHBCallback);
  w.show();
  return a.exec();
}

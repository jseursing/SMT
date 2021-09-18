#pragma once
#include "BotWidget.h"
#include <vector>

#include <QtWidgets/QMainWindow>
#include "ui_SMT.h"

class SMT : public QMainWindow
{
  Q_OBJECT

public:
  SMT(QWidget *parent = Q_NULLPTR, void* authCB = 0, void* crcCB = 0, void* slaveCB = 0);

signals:
  void SigNotifyXYRGB(int x, int y,
                      unsigned char r, unsigned char g, unsigned char b);

private slots:
  void OnSearchTriggered();
  void OnStatusTriggered();
  void OnOpenScriptTriggered();
  void OnSaveScriptTriggered();
  void OnRecordTriggered();
  void OnCalibrateTriggered();
  void OnSleepTriggered();
  void OnNotifyXYRGB(int x, int y,
                     unsigned char r, unsigned char g, unsigned char b);
  void OnSlaveQuery();

private:
  void closeEvent(QCloseEvent* e);

  // Debug callback
  static void NotifyXYRGB(int x, int y,
                          unsigned char r, unsigned char g, unsigned char b);

  Ui::SMTClass ui;
  void* AuthenticateCB;
  void* Crc32CB;
  void* SlaveCB;
  QAction* StatusAction;
  QAction* SleepAction;
  QAction* RecordAction;
  std::vector<BotWidget*> Bots;
  static SMT* TheInstance;
};

#pragma once
#include "BotController.h"
#include "SyntaxHighlighter.h"
#include "WindowEnum.h"
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QTextEdit>

class BotWidget : public QWidget
{
  Q_OBJECT

public:
  static void SendXYRGB(int x, int y, unsigned char r, unsigned char g, unsigned char b);
  void NotifyStatus(const char* status);
  void NotifyDone();

  void SetStatusVisible(bool vis);
  BotController* GetController() const;
  void KillController();
  void LoadScript(const char* path);
  void SaveScript(const char* path);
  void Calibrate();
  void StartRecording();
  void StopRecording();
 
  BotWidget(QWidget* parent, WndHierarchy* wnd, void* callback);
  ~BotWidget();

  typedef void (*NotifyXYRGB)(int x, int y, unsigned char, unsigned char, unsigned char);

signals:
  void NotifyStatusSignal(const char*);
  void NotifyDoneSignal();

private slots:
  void OnStartClick();
  void OnNotifyStatus(const char* status);
  void OnNotifyDone();
  void OnMinimalViewStateChanged(int state);
  void OnCommandEntered();

private:
  QPixmap FromWinHBITMAP(HBITMAP bitmap);

  // Components
  QTextEdit* BotScript;
  QFrame* ScreenFrame;
  QLabel* Screen;
  QLabel* StateLabel;
  QCheckBox* MinimalCheckbox;
  QPushButton* StartButton;
  QTextEdit* DebugEdit;
  QLineEdit* CommandEdit;
  QTabWidget* ParentWidget;
  static BotWidget* TheInstance;

  // Members
  WndHierarchy Windows;
  BotController* Controller;
  SyntaxHighlighter* Syntax;
  NotifyXYRGB CallbackXYRGB;
};


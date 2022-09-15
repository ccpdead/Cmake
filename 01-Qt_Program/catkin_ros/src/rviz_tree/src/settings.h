#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include<QSettings>
#include<QMessageBox>

namespace Ui
{
  class settings;
}

class settings : public QWidget
{
  Q_OBJECT

public:
  explicit settings(QWidget *parent = nullptr);
  ~settings();

public:
  void slot_ok_btn_click();
  void slot_cancel_btn_click();

private:
  Ui::settings *ui;
};

#endif // SETTINGS_H

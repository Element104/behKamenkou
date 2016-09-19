#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_actionT_triggered();
  void on_tlacitko_start_clicked();
  void on_tlacitko_tak_to_ukoncime_clicked();
  void tik_tak();

  void on_zvladam_cist_popisky_toggled(bool checked);

private:
  Ui::MainWindow *ui;
  QTimer _casovac_sekunda_po_sekunde;
  QTime _cas_zacatek;
  QTime _cas_relativni;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QStandardItemModel>
#include <QHttpEngine/QHttpServer>
#include <QHttpEngine/QFilesystemHandler>
#include <QResource>
#include "APIcko.h"

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
  void on_tlacitko_stopni_to_pressed();

  void uloz();
  void nahraj();
  void uloz_jako_csv();

  void on_tlacitko_vyexportuj_to_clicked();

private:
  Ui::MainWindow *ui;
  QTimer _casovac_sekunda_po_sekunde;
  QDateTime _cas_zacatek;
  QDateTime _cas_relativni;
  QStandardItemModel *_model;
  int _posledni_casovy_index = -1;
  APIcko _apicko;
  QFilesystemHandler _server_fshandler;
  QHttpServer _server;
  QResource _html;
};

#endif // MAINWINDOW_H

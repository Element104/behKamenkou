#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QStandardItemModel>
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

public slots:
   void napis_status(const QString& status);

private slots:
  void on_actionT_triggered();
  void tik_tak();
  void on_policko_cislo_returnPressed();
  void on_tlacitko_synchronizovat_clicked();
  void on_zmena_tabulky(const QModelIndex& topLeft, const QModelIndex& bottomRight);

private:
  Ui::MainWindow *ui;
  QTimer _casovac_sekunda_po_sekunde;
  QTime _cas_zacatek;
  QStandardItemModel *_model;
  APIcko _apicko;
};

#endif // MAINWINDOW_H

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->obalovac->setEnabled(false);
  _casovac_sekunda_po_sekunde.setInterval(1000);
  connect(&_casovac_sekunda_po_sekunde, SIGNAL(timeout()),
          this, SLOT(tik_tak()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_actionT_triggered()
{
  QApplication::quit();
}

void MainWindow::on_tlacitko_start_clicked()
{
  ui->obalovac->setEnabled(true);
  ui->tlacitko_start->setEnabled(false);
  _casovac_sekunda_po_sekunde.start();
  _cas_zacatek.start();
  _cas_relativni.start();
  ui->zvladam_cist_popisky->setChecked(false);
}

void MainWindow::on_tlacitko_tak_to_ukoncime_clicked()
{
  ui->obalovac->setEnabled(false);
  ui->tlacitko_start->setEnabled(true);
  _casovac_sekunda_po_sekunde.stop();
}

void MainWindow::tik_tak()
{
  QTime cas_od_zacatku = QTime(0, 0).addMSecs(_cas_zacatek.elapsed());
  QTime cas_relativni = QTime(0, 0).addMSecs(_cas_relativni.elapsed());

  ui->casoukazovac_celkovy->setText(cas_od_zacatku.toString("HH:mm:ss"));
  ui->casoukazovac_relativni->setText(cas_relativni.toString("HH:mm:ss"));
}

void MainWindow::on_zvladam_cist_popisky_toggled(bool checked)
{
  ui->tlacitko_tak_to_ukoncime->setEnabled(checked);
}

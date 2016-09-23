#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  _server_fshandler(":/static"),
  _server(&_server_fshandler)
{
  ui->setupUi(this);
  ui->obalovac->setEnabled(false);
  _casovac_sekunda_po_sekunde.setInterval(1000);
  connect(&_casovac_sekunda_po_sekunde, SIGNAL(timeout()),
          this, SLOT(tik_tak()));
  _model = nullptr;
  _server_fshandler.addSubHandler(QRegExp("api/"), &_apicko);
  _server.listen(QHostAddress::Any, 1234);
}

MainWindow::~MainWindow()
{
  if (_model) {
    delete _model;
  }
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

  if (_model) {
    delete _model;
  }
  _model = new QStandardItemModel();
  _model->setHorizontalHeaderItem(0, new QStandardItem("Cas"));
  _model->setHorizontalHeaderItem(1, new QStandardItem("Cislo"));
  ui->tabulkohled->setModel(_model);
  _apicko.setModel(_model);
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

void MainWindow::on_tlacitko_stopni_to_pressed()
{
  QTime cas = QTime(0, 0).addMSecs(_cas_zacatek.elapsed());
  _cas_relativni.restart();

  auto item = new QStandardItem();
  item->setText(cas.toString("HH:mm:ss.zzz"));
  item->setData(cas);

  if ((_posledni_casovy_index + 1) < _model->rowCount()) {
    _model->setItem(_posledni_casovy_index + 1, 0, item);
  }
  else {
    _model->appendRow(item);
  }
  _posledni_casovy_index++;

  ui->tabulkohled->scrollToBottom();
}

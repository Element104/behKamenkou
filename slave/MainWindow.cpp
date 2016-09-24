#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  _apicko(*this)
{
  ui->setupUi(this);
  _casovac_sekunda_po_sekunde.setInterval(1000);
  connect(&_casovac_sekunda_po_sekunde, SIGNAL(timeout()),
          this, SLOT(tik_tak()));
  _model = nullptr;
  _model = new QStandardItemModel();
  connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
          this, SLOT(on_zmena_tabulky(QModelIndex,QModelIndex)));
  ui->tabulkohled->setModel(_model);
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

void MainWindow::tik_tak()
{
  QTime cas_od_zacatku = QTime(0, 0).addMSecs(_cas_zacatek.elapsed());

  ui->casoukazovac_celkovy->setText(cas_od_zacatku.toString("HH:mm:ss"));
}

void MainWindow::on_policko_cislo_returnPressed()
{
  if (_model == nullptr) {
    return;
  }

  int cislo = ui->policko_cislo->text().toInt();

  QStandardItem* item = new QStandardItem(ui->policko_cislo->text());
  item->setData(cislo);
  _model->appendRow(item);
  ui->policko_cislo->clear();
  ui->tabulkohled->scrollToBottom();
  _apicko.pridejCislo(cislo);
}

void MainWindow::on_tlacitko_synchronizovat_clicked()
{
  _apicko.setURL(QUrl());
  _model->clear();
  _apicko.setURL(QUrl(QString("http://%1/api").arg(ui->policko_server->text())));
  for (auto cislo : _apicko.vylistujCisla()) {
      QStandardItem* item = new QStandardItem(QString("%1").arg(cislo));
      item->setData(cislo);
      _model->appendRow(item);
  }
}

void MainWindow::on_zmena_tabulky(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
  for (int r = topLeft.row(); r <= bottomRight.row(); ++r) {
    _apicko.zmenCislo(r, _model->data(_model->index(r, 0)).toInt());
  }
}

void MainWindow::napis_status(const QString &status)
{
    ui->statusBar->showMessage(status);
}

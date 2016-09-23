#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
//  ui->obalovac->setEnabled(false);
  _casovac_sekunda_po_sekunde.setInterval(1000);
  connect(&_casovac_sekunda_po_sekunde, SIGNAL(timeout()),
          this, SLOT(tik_tak()));
  _model = nullptr;
  _model = new QStandardItemModel();
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
  QTime cas_relativni = QTime(0, 0).addMSecs(_cas_relativni.elapsed());

  ui->casoukazovac_celkovy->setText(cas_od_zacatku.toString("HH:mm:ss"));
  ui->casoukazovac_relativni->setText(cas_relativni.toString("HH:mm:ss"));
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
  _apicko.setURL(QUrl(QString("http://%1/api").arg(ui->policko_server->text())));
}

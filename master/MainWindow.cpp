#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFile>
#include <QIODevice>

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
  ui->tlacitko_tak_to_ukoncime->setEnabled(true);
  ui->zvladam_cist_popisky->setChecked(false);

  _casovac_sekunda_po_sekunde.start();
  _cas_zacatek.start();
  _cas_relativni.start();

  if (_model) {
    delete _model;
  }
  _model = new QStandardItemModel();
  _model->setColumnCount(2);
  _model->setHorizontalHeaderItem(0, new QStandardItem("Cas"));
  _model->setHorizontalHeaderItem(1, new QStandardItem("Cislo"));
  connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(uloz()));

  nahraj();

  ui->tabulkohled->setModel(_model);
  _apicko.setModel(_model);
}

void MainWindow::on_tlacitko_tak_to_ukoncime_clicked()
{
  _casovac_sekunda_po_sekunde.stop();
  ui->obalovac->setEnabled(false);
  ui->tlacitko_start->setEnabled(true);
  _casovac_sekunda_po_sekunde.stop();
  ui->zvladam_cist_popisky->setChecked(false);
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
  uloz();
}

void MainWindow::uloz()
{
  QFile file("tabulka.bin");
  if (file.open(QIODevice::WriteOnly)) {
    QDataStream stream(&file);

    stream << _cas_zacatek.elapsed();
    stream << _cas_relativni.elapsed();

    qint32 n = _model->rowCount();
    qint32 m = _model->columnCount();
    qint32 t = _posledni_casovy_index;
    stream << n << m << t;

    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        if (!_model->item(i, j)) {
          _model->setItem(i, j, new QStandardItem(""));
        }
        _model->item(i, j)->write(stream);
      }
    }
    file.flush();
    file.copy(QString("tabulka-%1.bin").arg(QDateTime::currentDateTime().toString(Qt::ISODate)));
    file.close();
  }
}

void MainWindow::nahraj()
{
  QFile file("tabulka.bin");
  if (file.open(QIODevice::ReadOnly)) {
    QDataStream stream(&file);

    int cas_zacatek;
    int cas_relativni;

    stream >> cas_zacatek;
    stream >> cas_relativni;

    _cas_zacatek.start();
    _cas_relativni.start();
    _cas_zacatek = _cas_zacatek.addMSecs(-cas_zacatek);
    _cas_relativni = _cas_relativni.addMSecs(-cas_relativni);

    qint32 n, m, t;
    stream >> n >> m >> t;

    _model->setRowCount(n);
    _model->setColumnCount(m);
    _posledni_casovy_index = t;

    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        _model->setItem(i, j, new QStandardItem());
        _model->item(i, j)->read(stream);
      }
    }
    file.close();
  }
}

void MainWindow::uloz_jako_csv()
{
  QFile file("tabulka.csv");
  if (file.open(QIODevice::WriteOnly)) {
    QTextStream stream(&file);

    for (int i = 0; i < _model->rowCount(); ++i) {
      for (int j = 0; j < _model->columnCount(); ++j) {
        if (!_model->item(i, j)) {
          _model->setItem(i, j, new QStandardItem(""));
        }
        stream << _model->item(i, j)->text();
        stream << ",";
      }
      stream << "\n";
    }
    file.flush();
    file.close();
  }
}

void MainWindow::on_tlacitko_vyexportuj_to_clicked()
{
  uloz_jako_csv();
}

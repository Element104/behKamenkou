#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFile>
#include <QIODevice>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  _server_fshandler("./"),
  _server(&_server_fshandler)
{
  ui->setupUi(this);
  ui->obalovac->setEnabled(false);
  _casovac_sekunda_po_sekunde.setInterval(5000);
  connect(&_casovac_sekunda_po_sekunde, SIGNAL(timeout()),
          this, SLOT(tik_tak()));
  _model = nullptr;
  _server_fshandler.addSubHandler(QRegExp("api/"), &_apicko);
  _server_fshandler.addRedirect(QRegExp("^$"), "index.html");
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
  ui->tlacitko_tak_to_ukoncime->setEnabled(false);
  ui->zvladam_cist_popisky->setChecked(false);

  _casovac_sekunda_po_sekunde.start();
  _cas_zacatek = QDateTime::currentDateTime();
  _cas_relativni = QDateTime::currentDateTime();

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
  if (ui->zvladam_cist_popisky->isChecked()) {
    _casovac_sekunda_po_sekunde.stop();
    ui->obalovac->setEnabled(false);
    ui->tlacitko_start->setEnabled(true);
    ui->zvladam_cist_popisky->setChecked(false);
    ui->tlacitko_tak_to_ukoncime->setEnabled(false);
  }
}

void MainWindow::tik_tak()
{
  QTime cas_od_zacatku = QTime(0,0).addMSecs(QDateTime::currentMSecsSinceEpoch() - _cas_zacatek.toMSecsSinceEpoch());
  QTime cas_relativni = QTime(0,0).addMSecs(QDateTime::currentMSecsSinceEpoch() - _cas_relativni.toMSecsSinceEpoch());

  ui->casoukazovac_celkovy->setText(cas_od_zacatku.toString("HH:mm:ss"));
  ui->casoukazovac_relativni->setText(cas_relativni.toString("HH:mm:ss"));

  QFile index("index.html");
  if (index.open(QIODevice::WriteOnly)) {
    QTextStream stream(&index);
    stream << \
      QString("<html>"
              "<head>"
	      "<script>lapTime = %1; </script>"
	      "<script src=\"index.js\"></script>"
              "<meta http-equiv=\"refresh\" content=\"5\">"
              "</head>"
              "<body onload=\"lapTime = %1; show(); start(%1);\">"
              "<center>"
	      // "<div>Time: <span id=\"time\"></span></div>"
              "<p>"
              "<span id=\"time\" style=\"font-size: 100px; font-weight: bold; position: relative; top: 100px;\">%1</span>"
              "</p>").arg(QTime(0,0).msecsTo(cas_od_zacatku));

     stream << "<span style=\"font-weight: bold; position: relative; top: 100px;\">";
     stream << "<table border=\"1\" cellpadding=\"5\">";
     stream << "<tr>";
     stream << "<th>Pozice</th>";
     stream << "<th>Cas</th>";
     stream << "<th>Bachuv identifikator</th>";
     stream << "</tr>";
     for (int r = _model->rowCount(), c = 0; r > 0 && c < 10; --r, ++c) {
       if (!_model->item(r - 1, 0) || !_model->item(r - 1, 1)) {
          ++c;
       }
       stream << "<tr>";
       stream << QString("<td align=\"center\">%1</td><td align=\"center\">%2</td><td align=\"center\">%3</td>")
                 .arg(r)
                 .arg(_model->item(r - 1, 0)->text())
                 .arg(_model->item(r - 1, 1)->text());
       stream << "</tr>";
     }
     stream << "</table>";
     stream << "</span>";
     stream << \
              "</center>"
              "</body>"
              "</html>";
    index.close();
  }
}

void MainWindow::on_zvladam_cist_popisky_toggled(bool checked)
{
  ui->tlacitko_tak_to_ukoncime->setEnabled(checked);
}

void MainWindow::on_tlacitko_stopni_to_pressed()
{
  QTime cas = QTime(0,0).addMSecs(QDateTime::currentMSecsSinceEpoch() - _cas_zacatek.toMSecsSinceEpoch());
  _cas_relativni = QDateTime::currentDateTime();

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

    stream << _cas_zacatek.toMSecsSinceEpoch();
    stream << _cas_relativni.toMSecsSinceEpoch();

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

    qint64 cas_zacatek;
    qint64 cas_relativni;

    stream >> cas_zacatek;
    stream >> cas_relativni;

    _cas_zacatek.setMSecsSinceEpoch(cas_zacatek);
    _cas_relativni.setMSecsSinceEpoch(cas_relativni);

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
      stream << QString("%1").arg(i + 1);
      stream << ";";
      for (int j = 0; j < _model->columnCount(); ++j) {
        if (!_model->item(i, j)) {
          _model->setItem(i, j, new QStandardItem(""));
        }
        stream << _model->item(i, j)->text();
        stream << ";";
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

#include <QHttpEngine/QObjectHandler>
#include "APIcko.h"
#include <QDebug>
#include <QVariantMap>

QVariantMap APIcko::vylistujCisla(const QVariantMap &parametry)
{
  QVariantMap vysledek;

  try {
    QVariantList cisla;

    const int pocet_radku = model().rowCount();

    for (int r = 0; r < pocet_radku; ++r) {
      cisla.append(model().data(model().index(r, 1)).toInt());
    }

    vysledek.insert("cisla", cisla);
  }
  catch(...) {
    qDebug() << "model neni nastaveny asi";
  }

  return vysledek;
}

QVariantMap APIcko::pridejCislo(const QVariantMap &parametry)
{
  QVariantMap vysledek;

  try {
    int cislo = parametry["cislo"].toInt();
    int r = 0;
    bool exists = false;

    for (; r < model().rowCount(); ++r) {
      auto model_index = model().index(r, 1);
      if (model().data(model_index).toInt() == 0) {
        model().setData(model_index, cislo, Qt::DisplayRole);
        exists = true;
        break;
      }
    }

    if (!exists) {
        QList<QStandardItem*> row;
        row.append(new QStandardItem(""));
        row.append(new QStandardItem(QString("%1").arg(cislo)));
        model().appendRow(row);
        r = model().rowCount() - 1;
    }

    vysledek.insert("index", r);
  }
  catch(...) {
    qDebug() << "pridejCislo se nepovedlo";
  }

  return vysledek;
}

QVariantMap APIcko::zmenCislo(const QVariantMap &parametry)
{
  QVariantMap vysledek;

  try {
    int index = parametry["index"].toInt();
    int cislo = parametry["cislo"].toInt();
    if (index < 0 || index >= model().rowCount()) {
      throw std::out_of_range("index out of range");
    }
    model().setData(model().index(index, 1), cislo, Qt::DisplayRole);
  }
  catch(...) {
  }

  return vysledek;
}

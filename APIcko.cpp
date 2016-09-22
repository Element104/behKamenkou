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
  qDebug() << "pridejCislo";
  return QVariantMap();
}

QVariantMap APIcko::zmenCislo(const QVariantMap &parametry)
{
  qDebug() << "zmenCislo" << parametry.keys();

  QVariantMap m;
  m.insert("test", QString("foo"));
  return m;
}

#include <QHttpEngine/QObjectHandler>
#include "APIcko.h"
#include <QDebug>
#include <QVariantMap>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHttpMultiPart>

QList<int> APIcko::vylistujCisla()
{
  QVariantMap parametry;
  auto vysledek = call("vylistujCisla", parametry);
  auto klic_cisla = vysledek["cisla"].toList();
  QList<int> cisla;
  for (auto cislo : klic_cisla) {
    cisla.append(cislo.toInt());
  }
  return cisla;
}

int APIcko::pridejCislo(int cislo)
{
  QVariantMap parametry;
  parametry.insert("cislo", cislo);
  auto vysledek = call("pridejCislo", parametry);
  return vysledek["index"].toInt();
}

void APIcko::zmenCislo(int index, int nove_cislo)
{
  QVariantMap parametry;
  parametry.insert("index", index);
  parametry.insert("cislo", nove_cislo);
  auto vysledek = call("zmenCislo", parametry);
  return;
}

QVariantMap APIcko::call(const QString &metoda, const QVariantMap &parametry)
{
  const QJsonDocument request_json(QJsonObject::fromVariantMap(parametry));
  const QUrl request_url(_url.toString() + "/" + metoda);
  QNetworkRequest request(request_url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  QNetworkReply *reply = _manager.post(request, request_json.toJson());

  QEventLoop loop;
  connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();

  const QJsonDocument reply_json = QJsonDocument::fromJson(reply->readAll());
  return reply_json.object().toVariantMap();
}

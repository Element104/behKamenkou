#ifndef APICKO_H
#define APICKO_H

#include <QObject>
#include <QHttpEngine/QObjectHandler>
#include <QList>
#include <QUrl>
#include <QNetworkAccessManager>

class APIcko : public QObjectHandler
{
  Q_OBJECT
public:
  APIcko(const QUrl& api_url = QUrl())
  {
    setURL(api_url);
  }

public Q_SLOTS:
  QList<int> vylistujCisla();
  int pridejCislo(int cislo);
  void zmenCislo(int index, int nove_cislo);

  void setURL(const QUrl& api_url)
  {
    _url = api_url;
  }

private:
  const QUrl& url() const {
    return _url;
  }

  QVariantMap call(const QString& metoda, const QVariantMap& parametry);

  QNetworkAccessManager _manager;
  QUrl _url;
};

#endif // APICKO_H

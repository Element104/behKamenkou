#ifndef APICKO_H
#define APICKO_H

#include <QObject>
#include <qhttpengine/qobjecthandler.h>
#include <QList>
#include <QUrl>
#include <QNetworkAccessManager>

class MainWindow;
class APIcko : public QHttpEngine::QObjectHandler
{
  Q_OBJECT
public:
  APIcko(MainWindow& main, const QUrl& api_url = QUrl())
      : _mainwindow(main)
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

  MainWindow& _mainwindow;
  QNetworkAccessManager _manager;
  QUrl _url;
};

#endif // APICKO_H

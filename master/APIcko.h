#ifndef APICKO_H
#define APICKO_H

#include <QObject>
#include <qhttpengine/qobjecthandler.h>
#include <QStandardItemModel>

class APIcko : public QHttpEngine::QObjectHandler
{
  Q_OBJECT
public:
  APIcko(QStandardItemModel* model_ptr = nullptr)
  {
    setModel(model_ptr);
  }

public Q_SLOTS:
  QVariantMap vylistujCisla(const QVariantMap& parametry);
  QVariantMap pridejCislo(const QVariantMap& parametry);
  QVariantMap zmenCislo(const QVariantMap& parametry);

  void setModel(QStandardItemModel* model_ptr)
  {
    _model = model_ptr;
  }

private:
  QStandardItemModel& model()
  {
    if (_model != nullptr) {
      return *_model;
    }
    throw std::runtime_error("Model v APIcku neni nastaveny!");
  }

  QStandardItemModel* _model;
};

#endif // APICKO_H

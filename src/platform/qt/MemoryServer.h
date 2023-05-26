#ifndef MEMORYSERVER_H
#define MEMORYSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>

class MemoryServer : public QObject
{
  Q_OBJECT
  public:
    explicit MemoryServer(QObject *parent = 0);

  signals:

    public slots:
      void newConnection();

  private:
    QHash<qintptr, QTcpSocket*> sockets;
    QTcpServer *server;
    void readyRead(qintptr descriptor);
};

#endif // MEMORYSERVER_H

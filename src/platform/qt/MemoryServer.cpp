#include "MemoryServer.h"
#include <vector>

MemoryServer::MemoryServer(QObject *parent) : QObject(parent) {
  server = new QTcpServer(this);
  sockets = {};

  connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
  if (!server->listen(QHostAddress::Any, 9090)) {
    qDebug() << "Memory server could not be started";
  } else {
    qInfo() << "Memory server started";
  }
}

void MemoryServer::newConnection() {
  QTcpSocket* socket = server->nextPendingConnection();
  sockets[socket->socketDescriptor()] = socket;

  connect(socket, &QTcpSocket::readyRead, this, [this, socket]() { readyRead(socket->socketDescriptor()); });

  socket->write("Hello client\r\n");
  socket->flush();
  socket->waitForBytesWritten();
}

void MemoryServer::readyRead(qintptr descriptor) {
  QTcpSocket* socket = sockets[descriptor];
  QByteArray data = socket->readAll();
  if (data.isEmpty()) return;

  char command = data[0];

  if (data.size() < 4) return;

  unsigned short address = ((short)data[1]) << 8 | data[2];
  uint8_t bytesToRead = data[3];

  if (command == 0x47) {
    qInfo() << "Getting" << bytesToRead << "bytes from address" << Qt::hex << address;
  }
  if (command == 0x53) {
    qInfo() << "Setting" << bytesToRead << "bytes from address" << Qt::hex << address;
  }

  qInfo() << data.toHex();
  socket->close(); // Remove this to keep open connection
}

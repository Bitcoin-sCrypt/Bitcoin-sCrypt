#ifndef CHATTERBOXPAGE_H
#define CHATTERBOXPAGE_H

#include "bitcoingui.h"

#include <QWidget>
#include <QMainWindow>
#include <QObject>

#include <QTcpSocket>


namespace Ui
{
  class ChatterboxPage;
}

class ChatterboxPage : public QWidget
{
  Q_OBJECT

public:
  ChatterboxPage(QWidget *parent = 0);

private slots:
  // This function gets called when a user clicks on the
  // loginButton on the front page (which you placed there
  // with Designer)
  void on_loginButton_clicked();
  void on_logoutButton_clicked();

  // This gets called when you click the sayButton on
  // the chat page.
  void on_sayButton_clicked();

  // This is a function we'll connect to a socket's readyRead()
  // signal, which tells us there's text to be read from the chat
  // server.
  void readyRead();

  // This function gets called when the socket tells us it's connected.
  void connected();
  void disconnected();

protected:

private:
  Ui::ChatterboxPage *ui;
  // This is the socket that will let us communitate with the server.
  QTcpSocket *socket;

  void loadSettings();
  void saveSettings();
  QString chatserver,username;


signals:
};

#endif

#include "chatterboxpage.h"
#include "ui_chatterboxpage.h"
#include "util.h"
#include "bitcoingui.h"

#include <QSettings>
#include <string>
#include <QMainWindow>
#include "ui_interface.h"

#include <QMessageBox>
#include <QRegExp>

extern BitcoinGUI *guiref;


#include <QDebug>
using namespace std;



ChatterboxPage::ChatterboxPage(QWidget *parent) : QWidget(parent), ui(new Ui::ChatterboxPage)
{
  
  ui->setupUi(this);

  // Instantiate our socket (but don't actually connect to anything
  // yet until the user clicks the loginButton:
  socket = new QTcpSocket(this);

  // This is how we tell Qt to call our readyRead() and connected()
  // functions when the socket has text ready to be read, and is done
  // connecting to the server (respectively):
  connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
  connect(socket, SIGNAL(connected()), this, SLOT(connected()));

  QSettings settings("Bitcoin-sCrypt", "settings");
  username=settings.value("username", "").toString();
  chatserver=settings.value("chatserver", "").toString();

  ui->serverLineEdit->setText(chatserver);
  ui->userLineEdit->setText(username);
}


// This gets called when the loginButton gets clicked:
// We didn't have to use connect() to set this up because
// Qt recognizes the name of this function and knows to set
// up the signal/slot connection for us.
void ChatterboxPage::on_loginButton_clicked()
{
  int pass=0;
  if(ui->serverLineEdit->text().length() < 1)
  {
    QMessageBox::warning(this, tr("Need input"), tr("Please fill in the server information"));
    return;
  }
  if(ui->userLineEdit->text().length() < 1)
  {
    QMessageBox::warning(this, tr("Need input"), tr("Please fill in your user name"));
    return;
  }

  chatserver=ui->serverLineEdit->text();
  username=ui->userLineEdit->text();
  saveSettings();

    // Start connecting to the chat server (on port 4200).
    // This returns immediately and then works on connecting
    // to the server in the background. When it's done, we'll
    // get a connected() function call (below). If it fails,
    // we won't get any error message because we didn't connect()
    // to the error() signal from this socket.
  socket->connectToHost(ui->serverLineEdit->text(), 4200);
}

void ChatterboxPage::on_logoutButton_clicked()
{
  socket->disconnectFromHost();
  ui->roomTextEdit->append("waiting to disconnect");
}

// This gets called when the user clicks the sayButton (next to where
// they type text to send to the chat room):
void ChatterboxPage::on_sayButton_clicked()
{
  // What did they want to say (minus white space around the string):
  QString message = ui->sayLineEdit->text().trimmed();

  // Only send the text to the chat server if it's not empty:
  if(!message.isEmpty())
  {
    socket->write(QString(message + "\n").toUtf8());
  }

  // Clear out the input box so they can type something else:
  ui->sayLineEdit->clear();

  // Put the focus back into the input box so they can type again:
  ui->sayLineEdit->setFocus();
}

// This function gets called whenever the chat server has sent us some text:
void ChatterboxPage::readyRead()
{
  // We'll loop over every (complete) line of text that the server has sent us:
  while(socket->canReadLine())
  {
    // Here's the line the of text the server sent us (we use UTF-8 so
    // that non-English speakers can chat in their native language)
    QString line = QString::fromUtf8(socket->readLine()).trimmed();

    // These two regular expressions describe the kinds of messages
    // the server can send us:

    //  Normal messges look like this: "username:The message"
    QRegExp messageRegex("^([^:]+):(.*)$");

    // Any message that starts with "/users:" is the server sending us a
    // list of users so we can show that list in our GUI:
    QRegExp usersRegex("^/users:(.*)$");

    // Is this a users message:
    if(usersRegex.indexIn(line) != -1)
    {
      // If so, udpate our users list on the right:
      QStringList users = usersRegex.cap(1).split(",");
      ui->userListWidget->clear();
      foreach(QString user, users)
        new QListWidgetItem(QPixmap(":/user.png"), user, ui->userListWidget);
    }
    // Is this a normal chat message:
    else if(messageRegex.indexIn(line) != -1)
    {
      QString user = messageRegex.cap(1);  // mess from
      QString message = messageRegex.cap(2);

      QRegExp joinRegex("has joined.");
      if(joinRegex.indexIn(message) != -1)
        guiref->postMessage(message);

      QRegExp leftRegex("has left.");
      if(leftRegex.indexIn(message) != -1)
        guiref->postMessage(message);

      // append this message to our chat box:
      ui->roomTextEdit->append("<b>" + user + "</b>: " + message);
    }
  }
}

// This function gets called when our socket has successfully connected to the chat
// server. (see the connect() call in the MainWindow constructor).
void ChatterboxPage::connected()
{
  connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

  // Flip over to the chat page:
  ui->stackedWidget->setCurrentWidget(ui->chatPage);

  // And send our username to the chat server.
  socket->write(QString("/me:" + ui->userLineEdit->text() + "\n").toUtf8());
}

void ChatterboxPage::disconnected()
{
  // Flip over to the chat page:
  ui->stackedWidget->setCurrentWidget(ui->loginPage);
}

void ChatterboxPage::saveSettings()
{
  QSettings settings("Bitcoin-sCrypt", "settings");
  settings.setValue("username", username);
  settings.setValue("chatserver", chatserver);  //temp to be removed when using peers
}

void ChatterboxPage::loadSettings()
{
  QSettings settings("Bitcoin-sCrypt", "settings");
  username=settings.value("username", "").toString();
  chatserver=settings.value("username", "").toString();
}


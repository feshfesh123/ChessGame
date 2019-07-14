#include "game.h"
#include <QApplication>
#include <QFile>

QByteArray readTextFile(const QString &file_path) {
  QFile input_file(file_path);
  QByteArray input_data;

  if (input_file.open(QIODevice::Text | QIODevice::Unbuffered | QIODevice::ReadOnly)) {
    input_data = input_file.readAll();
    input_file.close();
    return input_data;
  }
  else {
    return QByteArray();
  }
}

game *Game;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString style_sheet = readTextFile(":/stylesheets/material-blue.qss");
    a.setStyleSheet(style_sheet);

    Game = new game();
    //Game->writeHistory();
    Game->show();

    return a.exec();
}

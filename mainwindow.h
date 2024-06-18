#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "spacecarver.h"
#include <QMainWindow>
#include <QList>
#include <QVector>
#include <QString>
#include "structs.h"
#include "spacecarver.h"
#include "scene.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


namespace Ui {
class MainWindow;
}

struct Metadata {
    QString position;
    QString lookVector;
    QString upVector;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSelectImages();

private:
    SpaceCarver* carver;
    void setupSpaceCarver();
    Ui::MainWindow *ui;
    QList<QString> selectImages();
    glm::vec3 stringToVec(QString str);
    QMap<QString, Metadata> createMetadataForms(const QList<QString> &imageFiles);
    void loadImage(const QString &filePath, std::vector<RGBA>* pixelArray);
};

#endif // MAINWINDOW_H

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
    glm::vec3 position;
    glm::vec3 lookVector;
    glm::vec3 upVector;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSelectImages();
    void onSelectMetadata();
    void onParse();
    void onStart();

private:
    SpaceCarver* carver;
    //QMap<QString, Metadata> imgNameToMeta;
    void setupSpaceCarver();
    Ui::MainWindow *ui;

    QList<QString> selectImages();
    QList<QString> imageFiles;
    bool imagesSelected;

    QList<QString> selectMetadata();
    QList<QString> metadataFile;
    bool metadataSelected;

    void mapMetadata();
    void parse();

    // helpers
    glm::vec3 stringToVec(QString str);
    void loadImage(const QString &filePath, std::vector<RGBA>* pixelArray);
    QString readFile(QString fileName);
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDebug>
#include <iostream>

/* MainWindow constructor */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug() << "Setting up UI";
    ui->setupUi(this); // Setup the UI
    // connect click signals to corresponding slots
    connect(ui->selectImagesButton, &QPushButton::clicked, this, &MainWindow::onSelectImages);
    connect(ui->selectMetadataButton, &QPushButton::clicked, this, &MainWindow::onSelectMetadata);
    connect(ui->parseButton, &QPushButton::clicked, this, &MainWindow::onParse);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStart);
    qDebug() << "UI setup complete";

    qDebug() << "Initializing carver";
    carver = new SpaceCarver();
    imagesSelected = false; metadataSelected = false;
    qDebug() << "Carver setup complete";
}

/* MainWindow Destructor */
MainWindow::~MainWindow() {
    delete ui;
    for (Camera* cam : carver->scene.cameras) {
        delete cam->photoData;
        delete cam;
    }
    delete carver;
}

void MainWindow::onSelectImages() {
    imageFiles = selectImages();
    imagesSelected = !imageFiles.isEmpty();
}

void MainWindow::onSelectMetadata() {
    metadataFile = selectMetadata();
    metadataSelected= !metadataFile.isEmpty();
}

void MainWindow::onParse() {
    if (imagesSelected && metadataSelected) {
        parse();
    } else {
        qDebug() << "Please selected valid image(s) and metadata.";
    }
}

void MainWindow::onStart() {
    parse();
    carver->multiSweep();
}

QList<QString> MainWindow::selectImages() {
    qDebug() << "Selecting images";
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Images (*.png *.jpg *.jpeg)"));
    dialog.setViewMode(QFileDialog::Detail);

    // if user selects file(s), return them in a list
    if (dialog.exec()) {
        qDebug() << "Image selected";
        return dialog.selectedFiles();
    }
    return QList<QString>(); // else return empty list
}

QList<QString> MainWindow::selectMetadata() {
    qDebug() << "Selecting metadata";
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Text Files (*.txt)"));
    dialog.setViewMode(QFileDialog::Detail);

    // if user selects file(s), return them in a list
    if (dialog.exec()) {
        qDebug() << "Metadata selected";
        return dialog.selectedFiles();
    }
    return QList<QString>(); // else return empty list
}

/* convert user inputted QStrings into their vector representations */
glm::vec3 MainWindow::stringToVec(QString str) {
    QStringList list = str.split(',');
    float x = list[0].toFloat();
    float y = list[1].toFloat();
    float z = list[2].toFloat();

    return glm::vec3(x, y, z);
}

/* convert image to RGBA struct */
glm::vec2 MainWindow::loadImage(const QString &file, std::vector<RGBA>* pixelArray) {
    QString path = "data/";
    QString filePath = path + file;
    QImage myImage;
    if (!myImage.load(filePath)) {
        qDebug() << "Failed to load in image";
    }
    myImage = myImage.convertToFormat(QImage::Format_RGBX8888);
    int width = myImage.width();
    int height = myImage.height();
    QByteArray arr = QByteArray::fromRawData((const char*) myImage.bits(), myImage.sizeInBytes());

    pixelArray->clear();
    pixelArray->reserve(width * height);
    for (int i = 0; i < arr.size() / 4.f; i++){
        pixelArray->push_back(RGBA{(std::uint8_t) arr[4*i], (std::uint8_t) arr[4*i+1], (std::uint8_t) arr[4*i+2], (std::uint8_t) arr[4*i+3]});
    }
    return glm::vec2{width, height};
}


QString MainWindow::readFile(QString fileName) {
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for reading:" << fileName;
        return QString(); // return empty QString if file can't be opened
    }
    QTextStream in(&file); // make stream
    QString fileContent = in.readAll(); // read file contents into QString
    file.close();
    return fileContent;
}

/* fill in scene with cameras and their associated image and metadata */
void MainWindow::parse() {
    qDebug() << "Parsing";

    // each line of metadata represents one image and is formatted as follows:
    // imgfilename.png posx,posy,posz lookx,looky,lookz, upx,upy,upz
    QString metadata = readFile(metadataFile.first());
    QList<QString> cameraList = metadata.split('\n');
    int numCams = cameraList.size();

    for (int i = 0; i < numCams; i++) {
        QString data = cameraList[i];
        QList<QString> items = data.split(' ');

        QString imgName = items[0];
        glm::vec4 position = {stringToVec(items[1]), 1.f};
        glm::vec4 lookVector = {normalize(stringToVec(items[2])), 0.f};
        glm::vec4 upVector = {normalize(stringToVec(items[3])), 0.f};

        std::vector<RGBA>* pixelArray = new std::vector<RGBA>;
        glm::vec2 dims = loadImage(imgName, pixelArray);

        Camera* cam = new Camera(pixelArray, position, lookVector, upVector, dims[0], dims[1]);
        carver->scene.cameras.push_back(cam);
    }

    qDebug() << "Parsing complete";
}

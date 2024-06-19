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
    // Connect the selectImagesButton click signal to the onSelectImages slot
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
    delete ui; // Clean up the UI
    for (Camera* cam : carver->scene.cameras) {
        delete cam->photoData;
        delete cam;
    }
    delete carver;
}

void MainWindow::onSelectImages() {
    imageFiles = selectImages(); // Open file dialog and get selected images
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
    QFileDialog dialog(this); // Create a file dialog
    dialog.setFileMode(QFileDialog::ExistingFiles); // Allow selection of existing files only
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg *.jpeg)")); // Set the file filter for images
    dialog.setViewMode(QFileDialog::Detail); // Set the view mode to detailed

    // If the user selects files, return the list of selected files
    if (dialog.exec()) {
        qDebug() << "Image selection complete";
        return dialog.selectedFiles();
    }
    return QList<QString>(); // else return empty list
}

QList<QString> MainWindow::selectMetadata() {
    qDebug() << "Selecting metadata";
    QFileDialog dialog(this); // Create a file dialog
    dialog.setFileMode(QFileDialog::ExistingFile); // Allow selection of existing files only
    dialog.setNameFilter(tr("Text Files (*.txt)")); // Set the file filter for images
    dialog.setViewMode(QFileDialog::Detail); // Set the view mode to detailed

    // If the user selects a file, return the selected file
    if (dialog.exec()) {
        qDebug() << "Metadata selection complete";
        return dialog.selectedFiles();
    }
    return QList<QString>(); // Return an empty list if no file is selected
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
    QImage myImage;
    if (!myImage.load(file)) {
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
    QFile file(fileName); // Create a QFile object with the given file name

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for reading:" << fileName;
        return QString(); // Return an empty QString if the file cannot be opened
    }
    QTextStream in(&file); // Create a QTextStream to read the file
    QString fileContent = in.readAll(); // Read all the content of the file into a QString
    file.close(); // Close the file
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
        glm::vec4 lookVector = {stringToVec(items[2]), 0.f};
        glm::vec4 upVector = {stringToVec(items[3]), 0.f};

        std::vector<RGBA>* pixelArray = new std::vector<RGBA>;
        glm::vec2 dims = loadImage(imgName, pixelArray);

        Camera* cam = new Camera(pixelArray, position, lookVector, upVector, dims[0], dims[1]);
        carver->scene.cameras.push_back(cam);
    }

    qDebug() << "Parsing complete";
}

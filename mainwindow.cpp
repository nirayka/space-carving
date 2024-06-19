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
    qDebug() << "UI setup complete";

    qDebug() << "Initializing carver";
    carver = new SpaceCarver(); // setup spacecarver
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


/* convert image to RGBA struct */
void MainWindow::loadImage(const QString &file, std::vector<RGBA>* pixelArray) {
    QImage myImage;
    if (!myImage.load(file)) {
        std::cout<<"Failed to load in image"<<std::endl;
    }
    myImage = myImage.convertToFormat(QImage::Format_RGBX8888);
    QByteArray arr = QByteArray::fromRawData((const char*) myImage.bits(), myImage.sizeInBytes());

    for (int i = 0; i < arr.size() / 4.f; i++){
        pixelArray->push_back(RGBA{(std::uint8_t) arr[4*i], (std::uint8_t) arr[4*i+1], (std::uint8_t) arr[4*i+2], (std::uint8_t) arr[4*i+3]});
    }
}

void MainWindow::onSelectImages() {
    imageFiles = selectImages(); // Open file dialog and get selected images
    imagesSelected = !imageFiles.isEmpty();
    if (imagesSelected && metadataSelected) { parse(); }
}

void MainWindow::onSelectMetadata() {
    metadataFile = selectMetadata();
    metadataSelected= !metadataFile.isEmpty();
    if (imagesSelected && metadataSelected) { parse(); }
}

/* convert user inputted QStrings into their vector representations */
glm::vec3 MainWindow::stringToVec(QString str) {
    QStringList list = str.split(',');

    float x = list[0].toFloat();
    float y = list[1].toFloat();
    float z = list[2].toFloat();

    return glm::vec3(x, y, z);
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

///* create a map from image filename to metadata */
//void MainWindow::mapMetadata() {
//    qDebug() << "Mapping metadata";
//    // each line of metadata represents an image and is formatted as follows:
//    // imgfilename.png posx,posy,posz lookx,looky,lookz, upx,upy,upz
//    // first set is filename, second is position, third is lookvector, fourth is upvector
//    QString metadata = readFile(metadataFile.first());
//    QList<QString> dataList = metadata.split('\n');
//    for (const QString &imgObj : dataList) { // TOCHECK
//        QList<QString> items = imgObj.split(' ');
//        QString imgName = items[0];
//        imgNameToMeta[imgName].position = stringToVec(items[1]);
//        imgNameToMeta[imgName].lookVector = stringToVec(items[2]);
//        imgNameToMeta[imgName].upVector = stringToVec(items[3]);
//    }
//    qDebug() << "Metadata mapping complete";
//}


// OLD START
//    for (const QString &imgObj : dataList) {
//        QList<QString> items = imgObj.split(' ');
//        QString imgName = items[0];

//        Metadata* meta;

//        qDebug() << "hi " << stringToVec(items[1]).x << stringToVec(items[1]).y << stringToVec(items[1]).z;

//        meta->position = stringToVec(items[1]);
//        meta->lookVector = stringToVec(items[2]);
//        meta->upVector = stringToVec(items[3]);
//        map.insert(imgName, meta);
//    }
//    qDebug() << "Metadata mapping complete";

//    // Iterate over each selected image file
//    for (const QString &file : imageFiles) {
//        std::vector<RGBA>* pixelArray = new std::vector<RGBA>;
//        loadImage(file, pixelArray);
//        Metadata perFileMeta = map[file];
//        glm::vec4 pos = glm::vec4{perFileMeta.position, 1.f};
//        glm::vec4 look = glm::vec4{perFileMeta.lookVector, 0.f};
//        glm::vec4 up = glm::vec4{perFileMeta.upVector, 0.f};
//        Camera* cam = new Camera(pixelArray, pos, look, up);
//        carver->scene.cameras.push_back(cam);
//        qDebug() << "added camera";
//    }
// OLD END

/* fill in scene with cameras and their associated image and metadata */
void MainWindow::parse() {
    qDebug() << "Parsing";

    // each line of metadata represents an image and is formatted as follows:
    // imgfilename.png posx,posy,posz lookx,looky,lookz, upx,upy,upz
    // first set is filename, second is position, third is lookvector, fourth is upvector
    QString metadata = readFile(metadataFile.first());
    QList<QString> cameraList = metadata.split('\n');
    int numCams = cameraList.size();
    carver->scene.cameras.resize(numCams);

    qDebug() << "fidsdfsdf";
    for (int i = 0; i < numCams; i++) {
        qDebug() << "sdfafdf";
        QString data = cameraList[i];

        QList<QString> items = data.split(' ');
        QString imgName = items[0];

        glm::vec4 position = {stringToVec(items[1]), 1.f};
        glm::vec4 lookVector = {stringToVec(items[2]), 0.f};
        glm::vec4 upVector = {stringToVec(items[3]), 0.f};

        carver->scene.cameras[i]->pos = position;
        carver->scene.cameras[i]->look = lookVector;
        carver->scene.cameras[i]->up = upVector;

        std::vector<RGBA>* pixelArray = new std::vector<RGBA>;
        loadImage(imgName, pixelArray);
        carver->scene.cameras[i]->photoData = pixelArray;
    }

    qDebug() << "Parsing complete";
}


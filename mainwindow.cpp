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
    qDebug() << "Setting up UI.";
    ui->setupUi(this); // Setup the UI
    carver = new SpaceCarver(); // setup spacecarver
    qDebug() << "UI setup complete.";

    // Connect the selectImagesButton click signal to the onSelectImages slot
    connect(ui->selectImagesButton, &QPushButton::clicked, this, &MainWindow::onSelectImages);
}

/* MainWindow Destructor */
MainWindow::~MainWindow() {
    delete ui; // Clean up the UI
}

/* open a file dialog and select images */
QList<QString> MainWindow::selectImages() {
    QFileDialog dialog(this); // Create a file dialog
    dialog.setFileMode(QFileDialog::ExistingFiles); // Allow selection of existing files only
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg *.jpeg)")); // Set the file filter for images
    dialog.setViewMode(QFileDialog::Detail); // Set the view mode to detailed

    // If the user selects files, return the list of selected files
    if (dialog.exec()) {
        return dialog.selectedFiles();
    }
    return QList<QString>(); // Return an empty list if no files are selected
}

/* create forms for user inputted metadata */
QMap<QString, Metadata> MainWindow::createMetadataForms(const QList<QString> &imageFiles) {
    QMap<QString, Metadata> metadataMap; // Map to hold metadata for each image
    QVBoxLayout *layout = new QVBoxLayout(this); // Create a vertical layout

    // Iterate over each selected image file
    for (const QString &file : imageFiles) {
        QFormLayout *formLayout = new QFormLayout; // Create a form layout for metadata

        // Create line edits for position, look vector, and up vector
        QLineEdit *positionEdit = new QLineEdit;
        QLineEdit *lookVectorEdit = new QLineEdit;
        QLineEdit *upVectorEdit = new QLineEdit;

        // Add rows to the form layout with labels and line edits
        formLayout->addRow(tr("Position for %1").arg(file), positionEdit);
        formLayout->addRow(tr("Look Vector for %1").arg(file), lookVectorEdit);
        formLayout->addRow(tr("Up Vector for %1").arg(file), upVectorEdit);

        // Add the form layout to the main layout
        layout->addLayout(formLayout);

        // Create a Metadata struct and populate it with the entered values
        Metadata metadata;
        connect(positionEdit, &QLineEdit::textChanged, [=, &metadataMap](const QString &text) {
            metadataMap[file].position = text;
        });
        connect(lookVectorEdit, &QLineEdit::textChanged, [=, &metadataMap](const QString &text) {
            metadataMap[file].lookVector = text;
        });
        connect(upVectorEdit, &QLineEdit::textChanged, [=, &metadataMap](const QString &text) {
            metadataMap[file].upVector = text;
        });

        // Append the metadata to the map
        metadataMap.insert(file, metadata);
    }

    //setLayout(layout); // Set the layout to the main window

    //NEW2
    // Set the layout to the main window (only if it does not have one already)
    if (!this->layout()) {
        this->setLayout(layout);
    } else {
        qWarning() << "MainWindow already has a layout.";
    }


    //NEW
    // Check if central widget already has a layout
    if (this->centralWidget()->layout() == nullptr) {
        this->centralWidget()->setLayout(layout);
    } else {
        qWarning() << "Central widget already has a layout.";
        // Optionally: clear existing layout and set a new one
        QLayout* oldLayout = this->centralWidget()->layout();
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
        this->centralWidget()->setLayout(layout);
    }



    return metadataMap; // Return the map of metadata
}

/* load image and convert to RGBA struct */
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

/* convert user inputted QStrings into their vector representations */
glm::vec3 MainWindow::stringToVec(QString str) {
    QStringList list = str.split(',');

    float x = list[0].toFloat();
    float y = list[1].toFloat();
    float z = list[2].toFloat();

    return glm::vec3(x, y, z);
}

/* main function for image selection */
void MainWindow::onSelectImages() {
    QList<QString> imageFiles = selectImages(); // Open file dialog and get selected images

    // If there are selected images, create metadata forms and process the images
    if (!imageFiles.isEmpty()) {
        QMap<QString, Metadata> metadataMap = createMetadataForms(imageFiles); // Create metadata forms

        // Iterate over each selected image file
        for (const QString &file : imageFiles) {
            std::vector<RGBA>* pixelArray = new std::vector<RGBA>;
            loadImage(file, pixelArray);

            Metadata metadata = metadataMap.value(file);
            glm::vec4 pos = glm::vec4{stringToVec(metadata.position), 1.f};
            glm::vec4 look = glm::vec4{stringToVec(metadata.lookVector), 0.f};
            glm::vec4 up = glm::vec4{stringToVec(metadata.upVector), 0.f};
            Camera cam = *new Camera(pixelArray, pos, look, up);
            carver->scene.cameras.push_back(cam);
        }
    }
}

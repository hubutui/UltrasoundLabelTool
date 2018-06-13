#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    setDirName(QFileDialog::getExistingDirectory(this,
                                                 tr("打开目录"),
                                                 QDir::currentPath(),
                                                 QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog));
    QDir workDir = getDirName();
    QStringList filters;
    // 支持的图像格式，暂时只用这三个，应该足够了
    filters << "*.png" << "*.jpg" << "*.bmp";
    workDir.setNameFilters(filters);
    workDir.setSorting(QDir::Name);
    setImageFileNameList(workDir.entryList());

    if (getImageFileNameList().isEmpty()) {
        QMessageBox::information(this,
                                 tr("警告"),
                                 tr("该目录下无任何图像文件！请选择正确的目录．"));
        return;
    } else {
        setCurrentIdx(0);
    }
    updateScene();
}

void MainWindow::setDirName(const QString &dirName)
{
    this->dirName = dirName;
}

void MainWindow::setImageFileNameList(const QStringList &fileNameList)
{
    this->imageFileNameList = fileNameList;
}

QStringList MainWindow::getImageFileNameList() const
{
    return this->imageFileNameList;
}

QString MainWindow::getCsvFileName() const
{
    return this->csvFileName;
}

int MainWindow::getCurrentIdx() const
{
    return this->currentIdx;
}

QMap<QString, int> MainWindow::getLabel() const
{
    return this->label;
}

int MainWindow::getCurrentValue() const
{
    return this->currentValue;
}

QMap<QString, int> MainWindow::readCsvFile(const QString &csvFileName)
{
    QFile file;
    file.setFileName(csvFileName);
    file.open(QIODevice::ReadOnly);
    QTextStream io(&file);
    QMap<QString, int> map;
    QString s, key, value;
    while(!io.atEnd()) {
        s = io.readLine();
        key = s.split(",").first();
        value = s.split(",").last();
        map.insert(key, value.toInt());
    }

    return map;
}

// 将存储在变量 map 中的标签保存到文件 csvFileName 中
void MainWindow::writeCsvFile(const QString &csvFileName, const QMap<QString, int> &map)
{
    // 检查输入参数 csvFileName，如果该字符串为空，则从用户获取一个保存的文件名
    QString fileName;
    if (!csvFileName.isEmpty()) {
        fileName = getCsvFileName();
    } else {
        fileName = QFileDialog::getSaveFileName(this,
                                                tr("保存标签文件"),
                                                QDir::currentPath());
    }

    QFile file;
    file.setFileName(fileName);
    file.open(QIODevice::WriteOnly);
    QTextStream io(&file);
    for (QMap<QString, int>::const_iterator i = map.constBegin(); i != map.constEnd(); ++i) {
        io << i.key() << "," << i.value() << endl;
    }
    file.close();
}

void MainWindow::updateScene()
{
    QDir workDir = QDir(getDirName());
    // create a QImage object
    QImage image(workDir.absoluteFilePath(imageFileNameList.at(getCurrentIdx())));

    // create a QPixmap object from QImage
    QPixmap pixmap = QPixmap::fromImage(image);
    scene = new QGraphicsScene(this);
    scene->clear();
    // add image to scene
    scene->addPixmap(pixmap);
    // set visible rectangle
    scene->setSceneRect(pixmap.rect());
    // important attach QGraphicsView with QGraphicsScene
    ui->graphicsView->setScene(scene);
    // 显示当前图像文件名
    ui->labelImageFileName->setText(workDir.absoluteFilePath(getImageFileNameList().at(getCurrentIdx())));
}

void MainWindow::setCsvFileName(const QString &csvFileName)
{
    this->csvFileName = csvFileName;
}

void MainWindow::setCurrentIdx(const int &idx)
{
    this->currentIdx = idx;
}

void MainWindow::setLabel(const QMap<QString, int> &map)
{
    this->label = map;
}

void MainWindow::setCurrentValue(const int &value)
{
    this->currentValue = value;
}

QString MainWindow::getDirName() const
{
    return this->dirName;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_pushButtonPrev_clicked()
{
    // 更新标签信息
    QString key = imageFileNameList.at(getCurrentIdx());
    int value= getCurrentValue();
    label.insert(key, value);

    if (getCurrentIdx() == 0) {
        QMessageBox::information(this,
                                 tr("警告"),
                                 tr("已经是最前一张图像了！"));
        return;
    } else {
        setCurrentIdx(getCurrentIdx() - 1);
    }
    updateScene();
    // 如果已经标注，则读取标注信息
    if (label.contains(imageFileNameList.at(getCurrentIdx()))) {
        if (label[imageFileNameList.at(getCurrentIdx())] == 0) {
            ui->radioButtonNo->setChecked(true);
        } else if (label[imageFileNameList.at(getCurrentIdx())] == 1) {
            ui->radioButtonYes->setChecked(true);
        }
    }
}

void MainWindow::on_pushButtonNext_clicked()
{
    // 更新标签信息
    QString key = imageFileNameList.at(getCurrentIdx());
    int value= getCurrentValue();
    label.insert(key, value);

    if (getCurrentIdx() == imageFileNameList.size() - 1) {
        QMessageBox::information(this,
                                 tr("警告"),
                                 tr("已经是最后一张图像了！"));
        return;
    } else {
        setCurrentIdx(getCurrentIdx() + 1);
    }
    updateScene();
    // 如果已经标注，则读取标注信息
    if (label.contains(imageFileNameList.at(getCurrentIdx()))) {
        if (label[imageFileNameList.at(getCurrentIdx())] == 0) {
            ui->radioButtonNo->setChecked(true);
        } else if (label[imageFileNameList.at(getCurrentIdx())] == 1) {
            ui->radioButtonYes->setChecked(true);
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (getCsvFileName().isEmpty()) {
        setCsvFileName(QFileDialog::getSaveFileName(this,
                                                    tr("保存文件"),
                                                    getDirName()));
    }
    writeCsvFile(getCsvFileName(),
                 getLabel());
    QMessageBox::information(this,
                             tr(""),
                             tr("保存成功！"));
}

void MainWindow::on_pushButtonReadProcess_clicked()
{
    setCsvFileName(QFileDialog::getOpenFileName(this,
                                                tr("打开文件"),
                                                getDirName()));

    setLabel(readCsvFile(getCsvFileName()));
    setCurrentIdx(label.size() - 1);
    updateScene();
}

void MainWindow::on_radioButtonYes_toggled(bool checked)
{
    if (checked) {
        setCurrentValue(1);
    } else {
        setCurrentValue(0);
    }
}

void MainWindow::on_radioButtonNo_toggled(bool checked)
{
    if (checked) {
        setCurrentValue(0);
    } else {
        setCurrentValue(1);
    }
}

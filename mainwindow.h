#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QGraphicsScene>
#include <QMap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionQuit_triggered();

    void on_pushButtonPrev_clicked();

    void on_pushButtonNext_clicked();

    void on_actionSave_triggered();

    void on_pushButtonReadProcess_clicked();

private:
    Ui::MainWindow *ui;

    // 数据所在的目录名称
    QString dirName;
    // 数据目录下全部的图像文件名称
    QStringList imageFileNameList;
    // 当前图像文件的索引
    QStringList::iterator currentIter;
    int currentIdx;
    // 标签文件名称
    QString csvFileName;
    // 存储所有标签的字典
    QMap<QString, int> label;

    QGraphicsScene *scene;

    void setDirName(const QString &dirName);
    void setImageFileNameList(const QStringList &fileNameList);
    void setCsvFileName(const QString &csvFileName);
    void setCurrentIdx(const int &idx);
    void setLabel(const QMap<QString, int> &map);

    QString getDirName(void) const;
    QStringList getImageFileNameList(void) const;
    QString getCsvFileName(void) const;
    int getCurrentIdx(void) const;
    QMap<QString, int> getLabel(void) const;

    QMap<QString, int> readCsvFile(const QString &csvFileName);
    void writeCsvFile(const QString &csvFileName, const QMap<QString, int> &map);
};

#endif // MAINWINDOW_H

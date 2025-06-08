#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>                 // for showing warning/critical dialogs

#include "StorageManager.h"
#include "DuplicateItemException.h"   // thrown by addItem
#include "ItemNotFoundException.h"    // thrown by removeItem

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();    // Add Item button
    void on_deleteButton_clicked();  // Delete button
    void on_exitButton_clicked();  //exit porgram
    void on_searchButton_clicked();  //search item

private:
    Ui::MainWindow *ui;

    // ── BACKEND STORAGE MANAGER ──
    StorageManager manager;

    // Helper to repopulate listWidget from manager’s contents
    void refreshItemList();
};

#endif

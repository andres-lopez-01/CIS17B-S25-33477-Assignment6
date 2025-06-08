#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QListWidgetItem>
#include <QMessageBox>     // for QMessageBox::warning / critical
#include <memory>          // for std::make_shared
#include "StoredItem.h"    // to construct new StoredItem in addItem

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // On startup, populate the QListWidget (will be empty initially)
    refreshItemList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ──────────────────────────────────────────────────────────────────────────────
// Helper: Clear the QListWidget, get all items from manager, and re-add them
void MainWindow::refreshItemList()
{
    ui->listWidget->clear();

    // Ask the manager for all items sorted by description
    auto items = manager.getAllItemsByDescription();
    for (auto const& itemPtr : items) {
        // Format each line as "ID -- Description -- Location"
        QString line = QString::fromStdString(
            itemPtr->getId() + " -- " +
            itemPtr->getDescription() + " -- " +
            itemPtr->getLocation()
            );
        ui->listWidget->addItem(line);
    }
}

// ──────────────────────────────────────────────────────────────────────────────
// Slot: “Add Item” button was clicked (objectName = pushButton)
void MainWindow::on_addButton_clicked()
{
    // 1) Read the three QLineEdits
    QString idText   = ui->txtId->text().trimmed();
    QString descText = ui->txtDescription->text().trimmed();
    QString locText  = ui->txtLocation->text().trimmed();

    // 2) Basic validation: none should be empty
    if (idText.isEmpty() || descText.isEmpty() || locText.isEmpty()) {
        QMessageBox::warning(
            this,
            "Input Error",
            "Please fill in ID, Description, and Location before adding."
            );
        return;
    }

    // 3) Convert to string
    string idStr   = idText.toStdString();
    string descStr = descText.toStdString();
    string locStr  = locText.toStdString();

    // 4) Attempt to add via backend manager
    try {
        auto newItem = make_shared<StoredItem>(idStr, descStr, locStr);
        manager.addItem(newItem);
    }
    catch (const DuplicateItemException& ex) {
        // If ID already exists, show an error dialog
        QMessageBox::critical(this, "Cannot Add Item", ex.what());
        return;
    }

    // 5) Clear inputs, set focus back to txtId, and refresh the list
    ui->txtId->clear();
    ui->txtDescription->clear();
    ui->txtLocation->clear();
    ui->txtId->setFocus();
    refreshItemList();
}

// ──────────────────────────────────────────────────────────────────────────────
void MainWindow::on_deleteButton_clicked()
{
    // 1) Read the ID from the same QLineEdit used by search:
    QString idText = ui->txtFindId->text().trimmed();

    // 2) Validate that it's not empty:
    if (idText.isEmpty()) {
        QMessageBox::warning(
            this,
            "Delete Item",
            "Please enter an ID to delete."
            );
        return;
    }

    // 3) Convert to string and attempt removal:
    std::string idStr = idText.toStdString();
    try {
        manager.removeItem(idStr);
    }
    catch (const ItemNotFoundException& ex) {
        QMessageBox::critical(this, "Delete Error", ex.what());
        return;
    }

    // 4) On success: refresh the list and clear the ID field:
    refreshItemList();
    ui->txtFindId->clear();
}


//Exit Program when button is clicked
void MainWindow::on_exitButton_clicked() {
    close();
}

//search item
void MainWindow::on_searchButton_clicked()
{
    // 1) Read the ID text from the search QLineEdit
    QString idText = ui->txtFindId->text().trimmed();

    // 2) Validate: ensure it isn’t empty
    if (idText.isEmpty()) {
        QMessageBox::warning(
            this,
            "Input Error",
            "Please enter an ID to search for."
            );
        return;
    }

    // 3) Convert to std::string and attempt to find via manager
    string idStr = idText.toStdString();
    try {
        auto itemPtr = manager.findById(idStr);
        // If found, show a dialog with its details
        QString info = QString::fromStdString(
            "ID: " + itemPtr->getId() + "\n"
                                        "Description: " + itemPtr->getDescription() + "\n"
                                          "Location: " + itemPtr->getLocation()
            );
        QMessageBox::information(this, "Item Found", info);
    }
    catch (const ItemNotFoundException& ex) {
        // If not found, show an error dialog
        QMessageBox::critical(this, "Item Not Found", ex.what());
    }
}


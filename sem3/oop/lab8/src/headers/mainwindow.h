#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include "contactstorage.h"
#include "contact.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addContact();
    void editContact();
    void deleteContact();
    void searchContacts();
    void clearSearch();
    void onTableSelectionChanged();
    void onTableSortChanged(int column);
    void loadContacts();
    void saveContacts();

private:
    void setupUI();
    void setupTable();
    void setupForm();
    void setupSearch();
    void populateTable();
    void clearForm();
    void fillForm(const Contact& contact);
    Contact getContactFromForm() const;
    bool validateForm(QString& errorMessage);
    void showError(const QString& message);
    void showInfo(const QString& message);
    int getSelectedRow() const;

    // UI Components
    QWidget* m_centralWidget;
    QVBoxLayout* m_mainLayout;
    
    // Table
    QTableWidget* m_table;
    
    // Form
    QGroupBox* m_formGroup;
    QLineEdit* m_firstNameEdit;
    QLineEdit* m_lastNameEdit;
    QLineEdit* m_middleNameEdit;
    QTextEdit* m_addressEdit;
    QDateEdit* m_birthDateEdit;
    QLineEdit* m_emailEdit;
    QTextEdit* m_phoneNumbersEdit;
    
    // Buttons
    QPushButton* m_addButton;
    QPushButton* m_editButton;
    QPushButton* m_deleteButton;
    QPushButton* m_saveButton;
    QPushButton* m_cancelButton;
    
    // Search
    QGroupBox* m_searchGroup;
    QLineEdit* m_searchEdit;
    QComboBox* m_searchFieldCombo;
    QPushButton* m_searchButton;
    QPushButton* m_clearSearchButton;
    
    // Data
    ContactStorage* m_storage;
    int m_editingIndex;
    bool m_isEditing;
};

#endif // MAINWINDOW_H


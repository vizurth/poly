#include "mainwindow.h"
#include "contactvalidator.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_editingIndex(-1)
    , m_isEditing(false)
{
    m_storage = new ContactStorage("phonebook.json");
    m_storage->load();
    
    setupUI();
    populateTable();
    
    // Автосохранение при закрытии
    connect(qApp, &QApplication::aboutToQuit, this, &MainWindow::saveContacts);
}

MainWindow::~MainWindow()
{
    saveContacts();
    delete m_storage;
}

void MainWindow::setupUI()
{
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    m_mainLayout = new QVBoxLayout(m_centralWidget);
    
    setupTable();
    setupForm();
    setupSearch();
    
    setWindowTitle("Телефонный справочник");
    resize(1000, 700);
}

void MainWindow::setupTable()
{
    m_table = new QTableWidget(this);
    m_table->setColumnCount(7);
    m_table->setHorizontalHeaderLabels({"Фамилия", "Имя", "Отчество", "Адрес", "Дата рождения", "Email", "Телефоны"});
    
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setSortingEnabled(true);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setAlternatingRowColors(true);
    
    connect(m_table, &QTableWidget::itemSelectionChanged, this, &MainWindow::onTableSelectionChanged);
    connect(m_table->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, &MainWindow::onTableSortChanged);
    
    m_mainLayout->addWidget(m_table);
}

void MainWindow::setupForm()
{
    m_formGroup = new QGroupBox("Данные контакта", this);
    QVBoxLayout* formLayout = new QVBoxLayout(m_formGroup);
    
    QHBoxLayout* nameLayout = new QHBoxLayout();
    m_lastNameEdit = new QLineEdit(this);
    m_lastNameEdit->setPlaceholderText("Фамилия");
    m_firstNameEdit = new QLineEdit(this);
    m_firstNameEdit->setPlaceholderText("Имя");
    m_middleNameEdit = new QLineEdit(this);
    m_middleNameEdit->setPlaceholderText("Отчество");
    nameLayout->addWidget(new QLabel("Фамилия:", this));
    nameLayout->addWidget(m_lastNameEdit);
    nameLayout->addWidget(new QLabel("Имя:", this));
    nameLayout->addWidget(m_firstNameEdit);
    nameLayout->addWidget(new QLabel("Отчество:", this));
    nameLayout->addWidget(m_middleNameEdit);
    formLayout->addLayout(nameLayout);
    
    QHBoxLayout* addressLayout = new QHBoxLayout();
    m_addressEdit = new QTextEdit(this);
    m_addressEdit->setMaximumHeight(60);
    m_addressEdit->setPlaceholderText("Адрес");
    addressLayout->addWidget(new QLabel("Адрес:", this));
    addressLayout->addWidget(m_addressEdit);
    formLayout->addLayout(addressLayout);
    
    QHBoxLayout* dateLayout = new QHBoxLayout();
    m_birthDateEdit = new QDateEdit(this);
    m_birthDateEdit->setCalendarPopup(true);
    m_birthDateEdit->setDate(QDate::currentDate().addYears(-20));
    m_birthDateEdit->setMaximumDate(QDate::currentDate().addDays(-1));
    dateLayout->addWidget(new QLabel("Дата рождения:", this));
    dateLayout->addWidget(m_birthDateEdit);
    formLayout->addLayout(dateLayout);
    
    QHBoxLayout* emailLayout = new QHBoxLayout();
    m_emailEdit = new QLineEdit(this);
    m_emailEdit->setPlaceholderText("email@example.com");
    emailLayout->addWidget(new QLabel("Email:", this));
    emailLayout->addWidget(m_emailEdit);
    formLayout->addLayout(emailLayout);
    
    QHBoxLayout* phoneLayout = new QHBoxLayout();
    m_phoneNumbersEdit = new QTextEdit(this);
    m_phoneNumbersEdit->setMaximumHeight(60);
    m_phoneNumbersEdit->setPlaceholderText("Телефоны (каждый с новой строки):\n+7 812 1234567\n8(800)123-1212");
    phoneLayout->addWidget(new QLabel("Телефоны:", this));
    phoneLayout->addWidget(m_phoneNumbersEdit);
    formLayout->addLayout(phoneLayout);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_addButton = new QPushButton("Добавить", this);
    m_editButton = new QPushButton("Редактировать", this);
    m_deleteButton = new QPushButton("Удалить", this);
    m_saveButton = new QPushButton("Сохранить", this);
    m_cancelButton = new QPushButton("Отмена", this);
    
    m_editButton->setEnabled(false);
    m_deleteButton->setEnabled(false);
    m_saveButton->setEnabled(false);
    m_cancelButton->setEnabled(false);
    
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_editButton);
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addStretch();
    
    formLayout->addLayout(buttonLayout);
    m_mainLayout->addWidget(m_formGroup);
    
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::addContact);
    connect(m_editButton, &QPushButton::clicked, this, &MainWindow::editContact);
    connect(m_deleteButton, &QPushButton::clicked, this, &MainWindow::deleteContact);
    connect(m_saveButton, &QPushButton::clicked, this, &MainWindow::addContact);
    connect(m_cancelButton, &QPushButton::clicked, this, [this]() {
        clearForm();
        m_isEditing = false;
        m_editingIndex = -1;
        m_saveButton->setEnabled(false);
        m_cancelButton->setEnabled(false);
        m_addButton->setEnabled(true);
    });
}

void MainWindow::setupSearch()
{
    m_searchGroup = new QGroupBox("Поиск", this);
    QHBoxLayout* searchLayout = new QHBoxLayout(m_searchGroup);
    
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Введите текст для поиска...");
    
    m_searchFieldCombo = new QComboBox(this);
    m_searchFieldCombo->addItems({"Все поля", "Фамилия", "Имя", "Отчество", "Адрес", "Email", "Телефон"});
    
    m_searchButton = new QPushButton("Найти", this);
    m_clearSearchButton = new QPushButton("Очистить", this);
    
    searchLayout->addWidget(new QLabel("Поиск:", this));
    searchLayout->addWidget(m_searchEdit);
    searchLayout->addWidget(new QLabel("Поле:", this));
    searchLayout->addWidget(m_searchFieldCombo);
    searchLayout->addWidget(m_searchButton);
    searchLayout->addWidget(m_clearSearchButton);
    
    m_mainLayout->addWidget(m_searchGroup);
    
    connect(m_searchButton, &QPushButton::clicked, this, &MainWindow::searchContacts);
    connect(m_clearSearchButton, &QPushButton::clicked, this, &MainWindow::clearSearch);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &MainWindow::searchContacts);
}

void MainWindow::populateTable()
{
    m_table->setSortingEnabled(false);
    m_table->setRowCount(0);
    
    const QList<Contact>& contacts = m_storage->contacts();
    for (int i = 0; i < contacts.size(); ++i) {
        const Contact& contact = contacts[i];
        int row = m_table->rowCount();
        m_table->insertRow(row);
        
        m_table->setItem(row, 0, new QTableWidgetItem(contact.lastName()));
        m_table->setItem(row, 1, new QTableWidgetItem(contact.firstName()));
        m_table->setItem(row, 2, new QTableWidgetItem(contact.middleName()));
        m_table->setItem(row, 3, new QTableWidgetItem(contact.address()));
        m_table->setItem(row, 4, new QTableWidgetItem(contact.birthDate().toString("dd.MM.yyyy")));
        m_table->setItem(row, 5, new QTableWidgetItem(contact.email()));
        m_table->setItem(row, 6, new QTableWidgetItem(contact.phoneNumbers().join(", ")));
    }
    
    m_table->setSortingEnabled(true);
    m_table->resizeColumnsToContents();
}

void MainWindow::clearForm()
{
    m_firstNameEdit->clear();
    m_lastNameEdit->clear();
    m_middleNameEdit->clear();
    m_addressEdit->clear();
    m_birthDateEdit->setDate(QDate::currentDate().addYears(-20));
    m_emailEdit->clear();
    m_phoneNumbersEdit->clear();
}

void MainWindow::fillForm(const Contact& contact)
{
    m_firstNameEdit->setText(contact.firstName());
    m_lastNameEdit->setText(contact.lastName());
    m_middleNameEdit->setText(contact.middleName());
    m_addressEdit->setPlainText(contact.address());
    m_birthDateEdit->setDate(contact.birthDate());
    m_emailEdit->setText(contact.email());
    m_phoneNumbersEdit->setPlainText(contact.phoneNumbers().join("\n"));
}

Contact MainWindow::getContactFromForm() const
{
    QString firstName = ContactValidator::normalizeName(m_firstNameEdit->text());
    QString lastName = ContactValidator::normalizeName(m_lastNameEdit->text());
    QString middleName = ContactValidator::normalizeName(m_middleNameEdit->text());
    QString address = m_addressEdit->toPlainText().trimmed();
    QDate birthDate = m_birthDateEdit->date();
    QString email = ContactValidator::normalizeEmail(m_emailEdit->text());
    
    QStringList phoneNumbers;
    QString phoneText = m_phoneNumbersEdit->toPlainText();
    QStringList phoneLines = phoneText.split('\n', Qt::SkipEmptyParts);
    for (const QString& phone : phoneLines) {
        QString normalized = ContactValidator::normalizePhone(phone.trimmed());
        if (!normalized.isEmpty()) {
            phoneNumbers.append(normalized);
        }
    }
    
    return Contact(firstName, lastName, middleName, address, birthDate, email, phoneNumbers);
}

bool MainWindow::validateForm(QString& errorMessage)
{
    QString firstName = ContactValidator::normalizeName(m_firstNameEdit->text());
    QString lastName = ContactValidator::normalizeName(m_lastNameEdit->text());
    QString middleName = ContactValidator::normalizeName(m_middleNameEdit->text());
    QString email = ContactValidator::normalizeEmail(m_emailEdit->text());
    QDate birthDate = m_birthDateEdit->date();
    
    QString msg;
    
    if (!firstName.isEmpty() && !ContactValidator::validateName(firstName, msg)) {
        errorMessage = "Имя: " + msg;
        return false;
    }
    
    if (!lastName.isEmpty() && !ContactValidator::validateName(lastName, msg)) {
        errorMessage = "Фамилия: " + msg;
        return false;
    }
    
    if (!middleName.isEmpty() && !ContactValidator::validateName(middleName, msg)) {
        errorMessage = "Отчество: " + msg;
        return false;
    }
    
    if (!ContactValidator::validateBirthDate(birthDate, msg)) {
        errorMessage = "Дата рождения: " + msg;
        return false;
    }
    
    if (!ContactValidator::validateEmail(email, msg)) {
        errorMessage = "Email: " + msg;
        return false;
    }
    
    QString phoneText = m_phoneNumbersEdit->toPlainText();
    QStringList phoneLines = phoneText.split('\n', Qt::SkipEmptyParts);
    if (phoneLines.isEmpty()) {
        errorMessage = "Необходимо указать хотя бы один телефон";
        return false;
    }
    
    for (const QString& phone : phoneLines) {
        QString normalizedPhone = ContactValidator::normalizePhone(phone.trimmed());
        if (!ContactValidator::validatePhone(normalizedPhone, msg)) {
            errorMessage = "Телефон: " + msg;
            return false;
        }
    }
    
    return true;
}

void MainWindow::showError(const QString& message)
{
    QMessageBox::critical(this, "Ошибка", message);
}

void MainWindow::showInfo(const QString& message)
{
    QMessageBox::information(this, "Информация", message);
}

int MainWindow::getSelectedRow() const
{
    QList<QTableWidgetItem*> selected = m_table->selectedItems();
    if (selected.isEmpty()) {
        return -1;
    }
    return selected.first()->row();
}

void MainWindow::addContact()
{
    QString errorMessage;
    if (!validateForm(errorMessage)) {
        showError(errorMessage);
        return;
    }
    
    Contact contact = getContactFromForm();
    
    if (m_isEditing && m_editingIndex >= 0) {
        // Редактирование существующего контакта
        m_storage->updateContact(m_editingIndex, contact);
        showInfo("Контакт обновлен");
    } else {
        // Добавление нового контакта
        m_storage->addContact(contact);
        showInfo("Контакт добавлен");
    }
    
    clearForm();
    populateTable();
    saveContacts();
    
    m_isEditing = false;
    m_editingIndex = -1;
    m_saveButton->setEnabled(false);
    m_cancelButton->setEnabled(false);
    m_addButton->setEnabled(true);
}

void MainWindow::editContact()
{
    int row = getSelectedRow();
    if (row < 0) {
        showError("Выберите контакт для редактирования");
        return;
    }
    
    // Находим индекс в хранилище (учитывая сортировку таблицы)
    QTableWidgetItem* item = m_table->item(row, 0);
    if (!item) return;
    
    // Ищем контакт по данным из таблицы
    QString lastName = m_table->item(row, 0)->text();
    QString firstName = m_table->item(row, 1)->text();
    QString email = m_table->item(row, 5)->text();
    
    int index = -1;
    const QList<Contact>& contacts = m_storage->contacts();
    for (int i = 0; i < contacts.size(); ++i) {
        if (contacts[i].lastName() == lastName && 
            contacts[i].firstName() == firstName && 
            contacts[i].email() == email) {
            index = i;
            break;
        }
    }
    
    if (index < 0) {
        showError("Контакт не найден");
        return;
    }
    
    m_editingIndex = index;
    m_isEditing = true;
    fillForm(contacts[index]);
    
    m_addButton->setEnabled(false);
    m_editButton->setEnabled(false);
    m_deleteButton->setEnabled(false);
    m_saveButton->setEnabled(true);
    m_cancelButton->setEnabled(true);
}

void MainWindow::deleteContact()
{
    int row = getSelectedRow();
    if (row < 0) {
        showError("Выберите контакт для удаления");
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Подтверждение", "Вы уверены, что хотите удалить этот контакт?",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        // Находим индекс в хранилище
        QString lastName = m_table->item(row, 0)->text();
        QString firstName = m_table->item(row, 1)->text();
        QString email = m_table->item(row, 5)->text();
        
        int index = -1;
        const QList<Contact>& contacts = m_storage->contacts();
        for (int i = 0; i < contacts.size(); ++i) {
            if (contacts[i].lastName() == lastName && 
                contacts[i].firstName() == firstName && 
                contacts[i].email() == email) {
                index = i;
                break;
            }
        }
        
        if (index >= 0) {
            m_storage->removeContact(index);
            populateTable();
            saveContacts();
            showInfo("Контакт удален");
        }
    }
}

void MainWindow::searchContacts()
{
    QString searchText = m_searchEdit->text().trimmed();
    if (searchText.isEmpty()) {
        populateTable();
        return;
    }
    
    int fieldIndex = m_searchFieldCombo->currentIndex();
    QString searchLower = searchText.toLower();
    
    m_table->setSortingEnabled(false);
    m_table->setRowCount(0);
    
    const QList<Contact>& contacts = m_storage->contacts();
    for (int i = 0; i < contacts.size(); ++i) {
        const Contact& contact = contacts[i];
        bool matches = false;
        
        switch (fieldIndex) {
        case 0: // Все поля
            matches = contact.lastName().toLower().contains(searchLower) ||
                      contact.firstName().toLower().contains(searchLower) ||
                      contact.middleName().toLower().contains(searchLower) ||
                      contact.address().toLower().contains(searchLower) ||
                      contact.email().toLower().contains(searchLower) ||
                      contact.phoneNumbers().join(" ").toLower().contains(searchLower);
            break;
        case 1: // Фамилия
            matches = contact.lastName().toLower().contains(searchLower);
            break;
        case 2: // Имя
            matches = contact.firstName().toLower().contains(searchLower);
            break;
        case 3: // Отчество
            matches = contact.middleName().toLower().contains(searchLower);
            break;
        case 4: // Адрес
            matches = contact.address().toLower().contains(searchLower);
            break;
        case 5: // Email
            matches = contact.email().toLower().contains(searchLower);
            break;
        case 6: // Телефон
            matches = contact.phoneNumbers().join(" ").toLower().contains(searchLower);
            break;
        }
        
        if (matches) {
            int row = m_table->rowCount();
            m_table->insertRow(row);
            
            m_table->setItem(row, 0, new QTableWidgetItem(contact.lastName()));
            m_table->setItem(row, 1, new QTableWidgetItem(contact.firstName()));
            m_table->setItem(row, 2, new QTableWidgetItem(contact.middleName()));
            m_table->setItem(row, 3, new QTableWidgetItem(contact.address()));
            m_table->setItem(row, 4, new QTableWidgetItem(contact.birthDate().toString("dd.MM.yyyy")));
            m_table->setItem(row, 5, new QTableWidgetItem(contact.email()));
            m_table->setItem(row, 6, new QTableWidgetItem(contact.phoneNumbers().join(", ")));
        }
    }
    
    m_table->setSortingEnabled(true);
    m_table->resizeColumnsToContents();
}

void MainWindow::clearSearch()
{
    m_searchEdit->clear();
    populateTable();
}

void MainWindow::onTableSelectionChanged()
{
    bool hasSelection = !m_table->selectedItems().isEmpty();
    m_editButton->setEnabled(hasSelection && !m_isEditing);
    m_deleteButton->setEnabled(hasSelection && !m_isEditing);
}

void MainWindow::onTableSortChanged(int column)
{
    // Сортировка обрабатывается автоматически QTableWidget
    Q_UNUSED(column);
}

void MainWindow::loadContacts()
{
    m_storage->load();
    populateTable();
}

void MainWindow::saveContacts()
{
    m_storage->save();
}


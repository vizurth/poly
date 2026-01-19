#include "mainwindow.h"
#include "contactvalidator.h"
#include <QApplication>

// запускается при создании программы
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_editingIndex(-1), m_isEditing(false)
{
    m_storage = new ContactStorage("phonebook.json");
    m_storage->load();
    setupUI();      // делаем интерфейс
    populateTable(); // заполняем таблицу
    connect(qApp, &QApplication::aboutToQuit, this, &MainWindow::saveContacts); // сохраняем перед выходом
}

// деструктор
MainWindow::~MainWindow(){
    saveContacts(); // сохраняем
    delete m_storage; // чистим память
}

// собираем ui
void MainWindow::setupUI(){
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    m_mainLayout = new QVBoxLayout(m_centralWidget);
    
    setupTable(); // таблица
    setupForm();  // форма ввода
    setupSearch(); // поиск
    
    setWindowTitle("Телефонный справочник");
    resize(1000, 700);
}

// создаем таблицу
void MainWindow::setupTable(){
    m_table = new QTableWidget(this);
    m_table->setColumnCount(7);
    m_table->setHorizontalHeaderLabels({"Фамилия", "Имя", "Отчество", "Адрес", "Дата рождения", "Email", "Телефоны"});
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setSortingEnabled(true); // включаем сортировку
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setAlternatingRowColors(true);
    
    // сигналы выбора и сортировки
    connect(m_table, &QTableWidget::itemSelectionChanged, this, &MainWindow::onTableSelectionChanged);
    connect(m_table->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, &MainWindow::onTableSortChanged);
    m_mainLayout->addWidget(m_table);
}

// создаем форму ввода
void MainWindow::setupForm(){
    m_formGroup = new QGroupBox("Данные контакта", this);
    QVBoxLayout* formLayout = new QVBoxLayout(m_formGroup);
    
    // фио
    QHBoxLayout* nameLayout = new QHBoxLayout();
    m_lastNameEdit = new QLineEdit(this);
    m_firstNameEdit = new QLineEdit(this);
    m_middleNameEdit = new QLineEdit(this);
    nameLayout->addWidget(new QLabel("Фамилия:", this));
    nameLayout->addWidget(m_lastNameEdit);
    nameLayout->addWidget(new QLabel("Имя:", this));
    nameLayout->addWidget(m_firstNameEdit);
    nameLayout->addWidget(new QLabel("Отчество:", this));
    nameLayout->addWidget(m_middleNameEdit);
    formLayout->addLayout(nameLayout);
    
    // адрес
    QHBoxLayout* addressLayout = new QHBoxLayout();
    m_addressEdit = new QTextEdit(this);
    m_addressEdit->setMaximumHeight(60);
    addressLayout->addWidget(new QLabel("Адрес:", this));
    addressLayout->addWidget(m_addressEdit);
    formLayout->addLayout(addressLayout);
    
    // дата рождения
    QHBoxLayout* dateLayout = new QHBoxLayout();
    m_birthDateEdit = new QDateEdit(this);
    m_birthDateEdit->setCalendarPopup(true);
    m_birthDateEdit->setDate(QDate::currentDate().addYears(-20));
    m_birthDateEdit->setMaximumDate(QDate::currentDate().addDays(-1));
    dateLayout->addWidget(new QLabel("Дата рождения:", this));
    dateLayout->addWidget(m_birthDateEdit);
    formLayout->addLayout(dateLayout);
    
    // email
    QHBoxLayout* emailLayout = new QHBoxLayout();
    m_emailEdit = new QLineEdit(this);
    emailLayout->addWidget(new QLabel("Email:", this));
    emailLayout->addWidget(m_emailEdit);
    formLayout->addLayout(emailLayout);
    
    // телефоны
    QHBoxLayout* phoneLayout = new QHBoxLayout();
    m_phoneNumbersEdit = new QTextEdit(this);
    m_phoneNumbersEdit->setMaximumHeight(60);
    phoneLayout->addWidget(new QLabel("Телефоны:", this));
    phoneLayout->addWidget(m_phoneNumbersEdit);
    formLayout->addLayout(phoneLayout);
    
    // кнопки
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_addButton = new QPushButton("Добавить", this);
    m_editButton = new QPushButton("Редактировать", this);
    m_deleteButton = new QPushButton("Удалить", this);
    m_saveButton = new QPushButton("Сохранить", this);
    m_cancelButton = new QPushButton("Отмена", this);
    m_undoButton = new QPushButton("Отменить последнее действие", this);
    
    // изначальные состояния кнопок
    m_editButton->setEnabled(false);
    m_deleteButton->setEnabled(false);
    m_saveButton->setEnabled(false);
    m_cancelButton->setEnabled(false);
    m_undoButton->setEnabled(false);
    
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_editButton);
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addWidget(m_undoButton);
    buttonLayout->addStretch();
    
    formLayout->addLayout(buttonLayout);
    m_mainLayout->addWidget(m_formGroup);
    
    // подключаем кнопки
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::addContact);
    connect(m_editButton, &QPushButton::clicked, this, &MainWindow::editContact);
    connect(m_deleteButton, &QPushButton::clicked, this, &MainWindow::deleteContact);
    connect(m_saveButton, &QPushButton::clicked, this, &MainWindow::addContact);
    connect(m_undoButton, &QPushButton::clicked, this, &MainWindow::undoLastAction);
    connect(m_cancelButton, &QPushButton::clicked, this, [this]() {
        clearForm();
        m_isEditing = false;
        m_editingIndex = -1;
        m_saveButton->setEnabled(false);
        m_cancelButton->setEnabled(false);
        m_addButton->setEnabled(true);
    });
}

// создаем поиск
void MainWindow::setupSearch(){
    m_searchGroup = new QGroupBox("Поиск", this);
    QHBoxLayout* searchLayout = new QHBoxLayout(m_searchGroup);
    
    m_searchEdit = new QLineEdit(this);
    m_searchFieldCombo = new QComboBox(this);
    m_searchFieldCombo->addItems({"Все поля", "Фамилия", "Имя", "Отчество", "Адрес", "Email", "Телефон", "Дата рождения"});
    m_searchButton = new QPushButton("Найти", this);
    m_clearSearchButton = new QPushButton("Очистить", this);
    
    // Поле для поиска по дате
	m_searchDateEdit = new QDateEdit(this);
	m_searchDateEdit->setCalendarPopup(true);
	m_searchDateEdit->setDate(QDate::currentDate());
	m_searchDateEdit->setVisible(false);

	// Тип поиска по дате
	m_dateSearchTypeCombo = new QComboBox(this);
	m_dateSearchTypeCombo->addItems({"Точная дата", "Год", "Месяц и год"});
	m_dateSearchTypeCombo->setVisible(false);

	searchLayout->addWidget(new QLabel("Поиск:", this));
	searchLayout->addWidget(m_searchEdit);
	searchLayout->addWidget(m_searchDateEdit);
	searchLayout->addWidget(m_dateSearchTypeCombo);
	searchLayout->addWidget(new QLabel("Поле:", this));
	searchLayout->addWidget(m_searchFieldCombo);
	searchLayout->addWidget(m_searchButton);
	searchLayout->addWidget(m_clearSearchButton);
	m_mainLayout->addWidget(m_searchGroup);

	// Переключение между текстовым полем и датой
	connect(m_searchFieldCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
		bool isDateSearch = (index == 7); // "Дата рождения"
		m_searchEdit->setVisible(!isDateSearch);
		m_searchDateEdit->setVisible(isDateSearch);
		m_dateSearchTypeCombo->setVisible(isDateSearch);
	});
    
    // подключаем поиск
    connect(m_searchButton, &QPushButton::clicked, this, &MainWindow::searchContacts);
    connect(m_clearSearchButton, &QPushButton::clicked, this, &MainWindow::clearSearch);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &MainWindow::searchContacts);
}

// проверка на дубликаты
bool MainWindow::checkForDuplicates(const Contact& contact, int excludeIndex) {
    const QList<Contact>& contacts = m_storage->contacts();
    for (int i = 0; i < contacts.size(); ++i) {
        if (i == excludeIndex) continue; // пропускаем сам контакт при редактировании
        
        const Contact& existing = contacts[i];
        if (existing.firstName() == contact.firstName() &&
            existing.lastName() == contact.lastName() &&
            existing.middleName() == contact.middleName() &&
			existing.phoneNumbers() == contact.phoneNumbers() &&
            existing.email() == contact.email()) {
            return true; // дубликат найден
        }
    }
    return false;
}

// проверка предупреждений о форматировании
QString MainWindow::getFormattingWarnings(const QString& firstName, const QString& lastName, 
                                           const QString& middleName, const QString& phone) {
    QStringList warnings;
    
    // Проверка имён на множественные заглавные буквы
    auto checkMultipleUppercase = [](const QString& name, const QString& fieldName) -> QString {
        if (name.isEmpty()) return "";
        int uppercaseCount = 0;
        for (const QChar& ch : name) {
            if (ch.isUpper()) uppercaseCount++;
        }
        if (uppercaseCount > 1) {
            return fieldName + ": обнаружено несколько заглавных букв, произведем изменение на (первая заглавная, остальные маленькие)";
        }
        return "";
    };
    
    QString firstWarning = checkMultipleUppercase(firstName, "Имя");
    QString lastWarning = checkMultipleUppercase(lastName, "Фамилия");
    QString middleWarning = checkMultipleUppercase(middleName, "Отчество");
    
    if (!firstWarning.isEmpty()) warnings.append(firstWarning);
    if (!lastWarning.isEmpty()) warnings.append(lastWarning);
    if (!middleWarning.isEmpty()) warnings.append(middleWarning);
    
    // Проверка телефона на неправильный формат
    QString phoneText = m_phoneNumbersEdit->toPlainText();
    QStringList phoneLines = phoneText.split('\n', Qt::SkipEmptyParts);
    for (const QString& phoneLine : phoneLines) {
        QString trimmed = phoneLine.trimmed();
        QString normalized = ContactValidator::normalizePhone(trimmed);
        if (trimmed != normalized && !trimmed.isEmpty()) {
            warnings.append("Телефон будет преобразован: " + trimmed + " => " + normalized);
        }
    }
    
    return warnings.join("\n\n");
}

// сохранение действия для отмены
void MainWindow::pushAction(ContactAction::Type type, const Contact& contact, int index) {
    ContactAction action;
    action.type = type;
    action.contact = contact;
    action.index = index;
    m_undoStack.push(action);
    m_undoButton->setEnabled(true);
}

// отмена последнего действия
void MainWindow::undoLastAction() {
	ContactAction action = m_undoStack.pop();

    switch (action.type) { // проверяем события
    case ContactAction::Add:
        if (!m_storage->contacts().isEmpty()) {
            m_storage->removeContact(m_storage->contacts().size() - 1);
            showInfo("Добавление контакта отменено");
        }
        break;
        
    case ContactAction::Edit:
        if (action.index >= 0 && action.index < m_storage->contacts().size()) {
            m_storage->updateContact(action.index, action.contact);
            showInfo("Редактирование контакта отменено");
        }
        break;
        
    case ContactAction::Delete:
        if (action.index >= 0) {
			m_storage->contacts().insert(action.index, action.contact);
            showInfo("Удаление контакта отменено");
        }
        break;
    }
    populateTable();
    saveContacts();
    
    if (m_undoStack.isEmpty()) {
        m_undoButton->setEnabled(false);
    }
}

// заполняем таблицу контактами
void MainWindow::populateTable(){
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

// очищаем форму
void MainWindow::clearForm(){
    m_firstNameEdit->clear();
    m_lastNameEdit->clear();
    m_middleNameEdit->clear();
    m_addressEdit->clear();
    m_birthDateEdit->setDate(QDate::currentDate().addYears(-20));
    m_emailEdit->clear();
    m_phoneNumbersEdit->clear();
}

// заполняем форму данными контакта
void MainWindow::fillForm(const Contact& contact){
    m_firstNameEdit->setText(contact.firstName());
    m_lastNameEdit->setText(contact.lastName());
    m_middleNameEdit->setText(contact.middleName());
    m_addressEdit->setPlainText(contact.address());
    m_birthDateEdit->setDate(contact.birthDate());
    m_emailEdit->setText(contact.email());
    m_phoneNumbersEdit->setPlainText(contact.phoneNumbers().join("\n"));
}

// собираем контакт из формы
Contact MainWindow::getContactFromForm() const{
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
        if (!normalized.isEmpty()) phoneNumbers.append(normalized);
    }
    
    return Contact(firstName, lastName, middleName, address, birthDate, email, phoneNumbers);
}

// проверяем форму
bool MainWindow::validateForm(QString& errorMessage){
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

// окно с ошибкой
void MainWindow::showError(const QString& message){
    QMessageBox::critical(this, "Ошибка", message);
}

// окно с инфой
void MainWindow::showInfo(const QString& message){
    QMessageBox::information(this, "Информация", message);
}

// получить выбранную строку
int MainWindow::getSelectedRow() const{
    QList<QTableWidgetItem*> selected = m_table->selectedItems();
    return selected.isEmpty() ? -1 : selected.first()->row();
}

// добавляем или обновляем контакт
void MainWindow::addContact(){
    QString errorMessage;
    if (!validateForm(errorMessage)) {
        showError(errorMessage);
        return;
    }
    
    // Проверяем предупреждения о форматировании
    QString warnings = getFormattingWarnings(m_firstNameEdit->text(), m_lastNameEdit->text(), m_middleNameEdit->text(), m_phoneNumbersEdit->toPlainText());
    
    if (!warnings.isEmpty()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Предупреждение о форматировании",
            warnings + "\n\nПродолжить?",
            QMessageBox::Yes | QMessageBox::No
        );
        
        if (reply != QMessageBox::Yes) {
            return;
        }
    }
    
    Contact contact = getContactFromForm();
    
    // Проверка на дубликаты
    if (checkForDuplicates(contact, m_isEditing ? m_editingIndex : -1)) {
        showError("Контакт с такими данными (ФИО и Email) уже существует!");
        return;
    }
    
    if (m_isEditing && m_editingIndex >= 0) {
        // Сохраняем старый контакт для отмены
        Contact oldContact = m_storage->contacts()[m_editingIndex];
        pushAction(ContactAction::Edit, oldContact, m_editingIndex);
        
        m_storage->updateContact(m_editingIndex, contact);
        showInfo("Контакт обновлен");
    } else {
        m_storage->addContact(contact);
        pushAction(ContactAction::Add, contact, m_storage->contacts().size() - 1);
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

// переходим в режим редактирования
void MainWчindow::editContact(){
    int row = getSelectedRow();
    if (row < 0) {
        showError("Выберите контакт для редактирования");
        return;
    }
    
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

// удаляем контакт
void MainWindow::deleteContact(){
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
            // Сохраняем контакт для отмены
            Contact deletedContact = contacts[index];
            pushAction(ContactAction::Delete, deletedContact, index);
            
            m_storage->removeContact(index);
            populateTable();
            saveContacts();
            showInfo("Контакт удален");
        }
    }
}

// поиск контактов
void MainWindow::searchContacts(){
    int fieldIndex = m_searchFieldCombo->currentIndex();
    
    // Поиск по дате рождения
	if (fieldIndex == 7) {
		QDate searchDate = m_searchDateEdit->date();
		int dateSearchType = m_dateSearchTypeCombo->currentIndex();
		
		m_table->setSortingEnabled(false);
		m_table->setRowCount(0);
		
		const QList<Contact>& contacts = m_storage->contacts();
		for (int i = 0; i < contacts.size(); ++i) {
			const Contact& contact = contacts[i];
			bool matches = false;
			
			switch (dateSearchType) {
			case 0: // Точная дата
				matches = (contact.birthDate() == searchDate);
				break;
			case 1: // Только год
				matches = (contact.birthDate().year() == searchDate.year());
				break;
			case 2: // Месяц и год
				matches = (contact.birthDate().year() == searchDate.year() &&
						contact.birthDate().month() == searchDate.month());
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
		return;
	}
    
    // Обычный текстовый поиск
    QString searchText = m_searchEdit->text().trimmed();
    if (searchText.isEmpty()) {
        populateTable();
        return;
    }
    
    QString searchLower = searchText.toLower();
    
    m_table->setSortingEnabled(false);
    m_table->setRowCount(0);
    
    const QList<Contact>& contacts = m_storage->contacts();
    for (int i = 0; i < contacts.size(); ++i) {
        const Contact& contact = contacts[i];
        bool matches = false;
        
        switch (fieldIndex) {
        case 0: // все поля
            matches = contact.lastName().toLower().contains(searchLower) ||
                      contact.firstName().toLower().contains(searchLower) ||
                      contact.middleName().toLower().contains(searchLower) ||
                      contact.address().toLower().contains(searchLower) ||
                      contact.email().toLower().contains(searchLower) ||
                      contact.phoneNumbers().join(" ").toLower().contains(searchLower);
            break;
        case 1: matches = contact.lastName().toLower().contains(searchLower); break;
        case 2: matches = contact.firstName().toLower().contains(searchLower); break;
        case 3: matches = contact.middleName().toLower().contains(searchLower); break;
        case 4: matches = contact.address().toLower().contains(searchLower); break;
        case 5: matches = contact.email().toLower().contains(searchLower); break;
        case 6: matches = contact.phoneNumbers().join(" ").toLower().contains(searchLower); break;
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

// сброс поиска
void MainWindow::clearSearch(){
    m_searchEdit->clear();
    populateTable();
}

// реакция на выбор в таблице
void MainWindow::onTableSelectionChanged(){
    bool hasSelection = !m_table->selectedItems().isEmpty();
    m_editButton->setEnabled(hasSelection && !m_isEditing);
    m_deleteButton->setEnabled(hasSelection && !m_isEditing);
}

void MainWindow::onTableSortChanged(int column){
    Q_UNUSED(column);
}

// загрузка из файла
void MainWindow::loadContacts(){
    m_storage->load();
    populateTable();
}

// сохранение в файл
void MainWindow::saveContacts(){
    m_storage->save();
}
// #include "contactvalidator.h"
// #include <QRegularExpression>
// #include <QDate>

// bool ContactValidator::validateName(const QString& name, QString& errorMessage)
// {
//     QString normalized = normalizeName(name);
    
//     if (normalized.isEmpty()) {
//         errorMessage = "Имя не может быть пустым";
//         return false;
//     }
    
//     // Проверка: должно начинаться с заглавной буквы
//     if (!normalized[0].isUpper()) {
//         errorMessage = "Имя должно начинаться с заглавной буквы";
//         return false;
//     }
    
//     // Не может начинаться или заканчиваться на дефис
//     if (normalized.startsWith('-') || normalized.endsWith('-')) {
//         errorMessage = "Имя не может начинаться или заканчиваться на дефис";
//         return false;
//     }
    
//     // Проверка: буквы (любых алфавитов), цифры, дефис, пробел
//     // Начинается с заглавной, может содержать несколько слов через пробел или дефис
//     QRegularExpression regex(R"(^[А-ЯЁA-ZЇІЄҐа-яёa-zїієґ0-9]([А-ЯЁA-ZЇІЄҐа-яёa-zїієґ0-9\s\-]*[А-ЯЁA-ZЇІЄҐа-яёa-zїієґ0-9])?$)");
    
//     if (!regex.match(normalized).hasMatch()) {
//         errorMessage = "Имя может содержать только буквы, цифры, дефис и пробел";
//         return false;
//     }
    
//     // Проверка: не должно быть двойных дефисов или пробелов
//     if (normalized.contains("--") || normalized.contains("  ")) {
//         errorMessage = "Имя не должно содержать двойные дефисы или пробелы";
//         return false;
//     }
    
//     return true;
// }

// bool ContactValidator::validatePhone(const QString& phone, QString& errorMessage)
// {
//     QString normalized = normalizePhone(phone);
    
//     if (normalized.isEmpty()) {
//         errorMessage = "Телефон не может быть пустым";
//         return false;
//     }
    
//     // Извлекаем только цифры для проверки
//     QString digits = extractPhoneDigits(normalized);
    
//     // Международный формат: от 10 до 15 цифр
//     if (digits.length() < 10) {
//         errorMessage = "Телефон должен содержать минимум 10 цифр";
//         return false;
//     }
    
//     if (digits.length() > 15) {
//         errorMessage = "Телефон должен содержать максимум 15 цифр";
//         return false;
//     }
    
//     // Проверяем формат: должен начинаться с + и содержать только цифры
//     QRegularExpression formatRegex(R"(^\+\d+$)");
//     if (!formatRegex.match(normalized).hasMatch()) {
//         errorMessage = "Телефон должен быть в международном формате (+...)";
//         return false;
//     }
    
//     return true;
// }

// bool ContactValidator::validateBirthDate(const QDate& date, QString& errorMessage)
// {
//     if (!date.isValid()) {
//         errorMessage = "Неверная дата";
//         return false;
//     }
    
//     QDate currentDate = QDate::currentDate();
    
//     // Дата рождения должна быть меньше текущей даты
//     if (date >= currentDate) {
//         errorMessage = "Дата рождения должна быть меньше текущей даты";
//         return false;
//     }
    
//     // Проверка разумности (не более 150 лет назад)
//     QDate minDate = currentDate.addYears(-150);
//     if (date < minDate) {
//         errorMessage = "Дата рождения слишком давняя";
//         return false;
//     }
    
//     // Проверка месяца (Qt уже проверяет, но для явности)
//     if (date.month() < 1 || date.month() > 12) {
//         errorMessage = "Месяц должен быть от 1 до 12";
//         return false;
//     }
    
//     // Проверка дня с учетом високосных лет
//     int year = date.year();
//     int month = date.month();
//     int day = date.day();
    
//     // Количество дней в каждом месяце
//     int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
//     // Проверка на високосный год
//     bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
//     if (isLeapYear && month == 2) {
//         daysInMonth[1] = 29;
//     }
    
//     if (day < 1 || day > daysInMonth[month - 1]) {
//         errorMessage = QString("День должен быть от 1 до %1 для выбранного месяца").arg(daysInMonth[month - 1]);
//         return false;
//     }
    
//     return true;
// }

// bool ContactValidator::validateEmail(const QString& email, QString& errorMessage)
// {
//     QString normalized = normalizeEmail(email);
    
//     if (normalized.isEmpty()) {
//         errorMessage = "Email не может быть пустым";
//         return false;
//     }
    
//     // Проверяем наличие ровно одного @
//     if (!normalized.contains('@')) {
//         errorMessage = "Email должен содержать символ @";
//         return false;
//     }
    
//     QStringList parts = normalized.split('@');
//     if (parts.size() != 2) {
//         errorMessage = "Email должен содержать ровно один символ @";
//         return false;
//     }
    
//     QString username = parts[0];
//     QString domain = parts[1];
    
//     if (username.isEmpty()) {
//         errorMessage = "Имя пользователя не может быть пустым";
//         return false;
//     }
    
//     if (domain.isEmpty()) {
//         errorMessage = "Домен не может быть пустым";
//         return false;
//     }
    
//     // Имя пользователя: латинские буквы, цифры, точки, подчеркивания
//     // Не может начинаться или заканчиваться на точку
//     if (username.startsWith('.') || username.endsWith('.')) {
//         errorMessage = "Имя пользователя не может начинаться или заканчиваться на точку";
//         return false;
//     }
    
//     QRegularExpression usernameRegex(R"(^[A-Za-z0-9._%+-]+$)");
//     if (!usernameRegex.match(username).hasMatch()) {
//         errorMessage = "Имя пользователя должно состоять из латинских букв, цифр, точек и подчеркиваний";
//         return false;
//     }
    
//     // Домен: должен содержать минимум одну точку
//     if (!domain.contains('.')) {
//         errorMessage = "Домен должен содержать минимум одну точку (например, example.com)";
//         return false;
//     }
    
//     // Проверка формата домена: латинские буквы, цифры, точки, дефисы
//     QRegularExpression domainRegex(R"(^[A-Za-z0-9]([A-Za-z0-9\-]*[A-Za-z0-9])?(\.[A-Za-z0-9]([A-Za-z0-9\-]*[A-Za-z0-9])?)*\.[A-Za-z]{2,}$)");
//     if (!domainRegex.match(domain).hasMatch()) {
//         errorMessage = "Неверный формат домена";
//         return false;
//     }
    
//     return true;
// }

// QString ContactValidator::normalizeName(const QString& name)
// {
//     QString normalized = name.trimmed();
    
//     // Удаляем множественные пробелы
//     normalized.replace(QRegularExpression(R"(\s+)"), " ");
    
//     // Удаляем множественные дефисы
//     normalized.replace(QRegularExpression(R"(-+)"), "-");
    
//     // Делаем первую букву заглавной
//     if (!normalized.isEmpty()) {
//         normalized[0] = normalized[0].toUpper();
        
//         // Делаем заглавными буквы после пробела и дефиса
//         for (int i = 1; i < normalized.length(); ++i) {
//             if ((normalized[i-1] == ' ' || normalized[i-1] == '-') && normalized[i].isLetter()) {
//                 normalized[i] = normalized[i].toUpper();
//             }
//         }
//     }
    
//     return normalized;
// }

// QString ContactValidator::normalizePhone(const QString& phone)
// {
//     QString normalized = phone.trimmed();
    
//     // Удаляем все пробелы, скобки, дефисы
//     normalized.remove(QRegularExpression(R"([\s\(\)\-])"));
    
//     // Преобразуем в международный формат
//     // Если начинается с 8 и длина 11 цифр (российский формат), заменяем на +7
//     if (normalized.startsWith("8")) {
//         QString digits = extractPhoneDigits(normalized);
//         if (digits.length() == 11) {
//             normalized = "+7" + digits.mid(1);
//         }
//     }
//     // Если начинается с 7 без +, добавляем +
//     else if (normalized.startsWith("7") && !normalized.startsWith("+")) {
//         normalized = "+" + normalized;
//     }
//     // Если не начинается с +, добавляем +
//     else if (!normalized.startsWith("+")) {
//         normalized = "+" + normalized;
//     }
    
//     return normalized;
// }

// QString ContactValidator::normalizeEmail(const QString& email)
// {
//     QString normalized = email.trimmed().toLower();
    
//     // Удаляем пробелы вокруг @
//     normalized.replace(QRegularExpression(R"(\s*@\s*)"), "@");
    
//     // Удаляем все пробелы
//     normalized.remove(' ');
    
//     return normalized;
// }

// QString ContactValidator::extractPhoneDigits(const QString& phone)
// {
//     QString digits;
//     for (const QChar& ch : phone) {
//         if (ch.isDigit()) {
//             digits.append(ch);
//         }
//     }
//     return digits;
// }


#include "contactvalidator.h"
#include <QRegularExpression>
#include <QDate>

bool ContactValidator::validateName(const QString& name, QString& errorMessage)
{
    // Сначала нормализуем (приводим к правильному виду)
    QString normalized = normalizeName(name);
    
    if (normalized.isEmpty()) {
        errorMessage = "Имя не может быть пустым";
        return false;
    }
    
    // Проверяем исходное имя (до нормализации) на недопустимые символы
    QString trimmed = name.trimmed();
    
    // Не может начинаться или заканчиваться на дефис
    if (trimmed.startsWith('-') || trimmed.endsWith('-')) {
        errorMessage = "Имя не может начинаться или заканчиваться на дефис";
        return false;
    }
    
    // Проверка: только буквы (любых алфавитов), цифры, дефис, пробел
    // ВАЖНО: разрешаем как заглавные, так и строчные буквы на входе
    QRegularExpression regex(R"(^[А-ЯЁA-ZЇІЄҐа-яёa-zїієґ0-9]([А-ЯЁA-ZЇІЄҐа-яёa-zїієґ0-9\s\-]*[А-ЯЁA-ZЇІЄҐа-яёa-zїієґ0-9])?$)");
    
    if (!regex.match(trimmed).hasMatch()) {
        errorMessage = "Имя может содержать только буквы, цифры, дефис и пробел";
        return false;
    }
    
    // Проверка: не должно быть двойных дефисов или пробелов
    if (trimmed.contains("--") || trimmed.contains(QRegularExpression(R"(\s{2,})"))) {
        errorMessage = "Имя не должно содержать двойные дефисы или множественные пробелы";
        return false;
    }
    
    return true;
}

bool ContactValidator::validatePhone(const QString& phone, QString& errorMessage)
{
    QString normalized = normalizePhone(phone);
    
    if (normalized.isEmpty()) {
        errorMessage = "Телефон не может быть пустым";
        return false;
    }
    
    // Извлекаем только цифры для проверки
    QString digits = extractPhoneDigits(normalized);
    
    // Международный формат: от 10 до 15 цифр
    if (digits.length() < 10) {
        errorMessage = "Телефон должен содержать минимум 10 цифр";
        return false;
    }
    
    if (digits.length() > 15) {
        errorMessage = "Телефон должен содержать максимум 15 цифр";
        return false;
    }
    
    // Проверяем формат: должен начинаться с + и содержать только цифры
    QRegularExpression formatRegex(R"(^\+\d+$)");
    if (!formatRegex.match(normalized).hasMatch()) {
        errorMessage = "Телефон должен быть в международном формате (+...)";
        return false;
    }
    
    return true;
}

bool ContactValidator::validateBirthDate(const QDate& date, QString& errorMessage)
{
    if (!date.isValid()) {
        errorMessage = "Неверная дата";
        return false;
    }
    
    QDate currentDate = QDate::currentDate();
    
    // Дата рождения должна быть меньше текущей даты
    if (date >= currentDate) {
        errorMessage = "Дата рождения должна быть меньше текущей даты";
        return false;
    }
    
    // Проверка разумности (не более 150 лет назад)
    QDate minDate = currentDate.addYears(-150);
    if (date < minDate) {
        errorMessage = "Дата рождения слишком давняя";
        return false;
    }
    
    // Проверка месяца (Qt уже проверяет, но для явности)
    if (date.month() < 1 || date.month() > 12) {
        errorMessage = "Месяц должен быть от 1 до 12";
        return false;
    }
    
    // Проверка дня с учетом високосных лет
    int year = date.year();
    int month = date.month();
    int day = date.day();
    
    // Количество дней в каждом месяце
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Проверка на високосный год
    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (isLeapYear && month == 2) {
        daysInMonth[1] = 29;
    }
    
    if (day < 1 || day > daysInMonth[month - 1]) {
        errorMessage = QString("День должен быть от 1 до %1 для выбранного месяца").arg(daysInMonth[month - 1]);
        return false;
    }
    
    return true;
}

bool ContactValidator::validateEmail(const QString& email, QString& errorMessage)
{
    QString normalized = normalizeEmail(email);
    
    if (normalized.isEmpty()) {
        errorMessage = "Email не может быть пустым";
        return false;
    }
    
    // Проверяем наличие ровно одного @
    if (!normalized.contains('@')) {
        errorMessage = "Email должен содержать символ @";
        return false;
    }
    
    QStringList parts = normalized.split('@');
    if (parts.size() != 2) {
        errorMessage = "Email должен содержать ровно один символ @";
        return false;
    }
    
    QString username = parts[0];
    QString domain = parts[1];
    
    if (username.isEmpty()) {
        errorMessage = "Имя пользователя не может быть пустым";
        return false;
    }
    
    if (domain.isEmpty()) {
        errorMessage = "Домен не может быть пустым";
        return false;
    }
    
    // Имя пользователя: латинские буквы, цифры, точки, подчеркивания
    // Не может начинаться или заканчиваться на точку
    if (username.startsWith('.') || username.endsWith('.')) {
        errorMessage = "Имя пользователя не может начинаться или заканчиваться на точку";
        return false;
    }
    
    QRegularExpression usernameRegex(R"(^[A-Za-z0-9._%+-]+$)");
    if (!usernameRegex.match(username).hasMatch()) {
        errorMessage = "Имя пользователя должно состоять из латинских букв, цифр, точек и подчеркиваний";
        return false;
    }
    
    // Домен: должен содержать минимум одну точку
    if (!domain.contains('.')) {
        errorMessage = "Домен должен содержать минимум одну точку (например, example.com)";
        return false;
    }
    
    // Проверка формата домена: латинские буквы, цифры, точки, дефисы
    QRegularExpression domainRegex(R"(^[A-Za-z0-9]([A-Za-z0-9\-]*[A-Za-z0-9])?(\.[A-Za-z0-9]([A-Za-z0-9\-]*[A-Za-z0-9])?)*\.[A-Za-z]{2,}$)");
    if (!domainRegex.match(domain).hasMatch()) {
        errorMessage = "Неверный формат домена";
        return false;
    }
    
    return true;
}

QString ContactValidator::normalizeName(const QString& name)
{
    QString normalized = name.trimmed();
    
    if (normalized.isEmpty()) {
        return normalized;
    }
    
    // Удаляем множественные пробелы (2 и более)
    normalized.replace(QRegularExpression(R"(\s{2,})"), " ");
    
    // Удаляем множественные дефисы
    normalized.replace(QRegularExpression(R"(-{2,})"), "-");
    
    // Делаем первую букву заглавной, остальные строчными
    normalized[0] = normalized[0].toUpper();
    
    // Обрабатываем каждый символ
    for (int i = 1; i < normalized.length(); ++i) {
        QChar current = normalized[i];
        QChar previous = normalized[i-1];
        
        // После пробела или дефиса делаем заглавную букву
        if ((previous == ' ' || previous == '-') && current.isLetter()) {
            normalized[i] = current.toUpper();
        }
        // Все остальные буквы делаем строчными (если они не после пробела/дефиса)
        else if (current.isLetter() && previous != ' ' && previous != '-') {
            normalized[i] = current.toLower();
        }
    }
    
    return normalized;
}

QString ContactValidator::normalizePhone(const QString& phone)
{
    QString normalized = phone.trimmed();
    
    // Удаляем все пробелы, скобки, дефисы
    normalized.remove(QRegularExpression(R"([\s\(\)\-])"));
    
    // Преобразуем в международный формат
    // Если начинается с 8 и длина 11 цифр (российский формат), заменяем на +7
    if (normalized.startsWith("8")) {
        QString digits = extractPhoneDigits(normalized);
        if (digits.length() == 11) {
            normalized = "+7" + digits.mid(1);
        }
    }
    // Если начинается с 7 без +, добавляем +
    else if (normalized.startsWith("7") && !normalized.startsWith("+")) {
        normalized = "+" + normalized;
    }
    // Если не начинается с +, добавляем +
    else if (!normalized.startsWith("+")) {
        normalized = "+" + normalized;
    }
    
    return normalized;
}

QString ContactValidator::normalizeEmail(const QString& email)
{
    QString normalized = email.trimmed().toLower();
    
    // Удаляем пробелы вокруг @
    normalized.replace(QRegularExpression(R"(\s*@\s*)"), "@");
    
    // Удаляем все пробелы
    normalized.remove(' ');
    
    return normalized;
}

QString ContactValidator::extractPhoneDigits(const QString& phone)
{
    QString digits;
    for (const QChar& ch : phone) {
        if (ch.isDigit()) {
            digits.append(ch);
        }
    }
    return digits;
}
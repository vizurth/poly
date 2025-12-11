#include "contactvalidator.h"
#include <QRegularExpression>
#include <QDate>

// проверка имени
bool ContactValidator::validateName(const QString& name, QString& errorMessage)
{
    // нормализуем
    QString normalized = normalizeName(name);
    
    if (normalized.isEmpty()) {
        errorMessage = "Имя не может быть пустым";
        return false;
    }
    
    // работаем с исходным вводом (обрезаем пробелы)
    QString trimmed = name.trimmed();
    
    // не должно начинаться или заканчиваться на дефис
    if (trimmed.startsWith('-') || trimmed.endsWith('-')) {
        errorMessage = "Имя не может начинаться или заканчиваться на дефис";
        return false;
    }
    
    // разрешенные символы: буквы, цифры, дефис, пробел
    QRegularExpression regex(R"(^[A-Za-zА-Яа-яЁё0-9][A-Za-zА-Яа-яЁё0-9\- ]*[A-Za-zА-Яа-яЁё0-9]$)");
    if (!regex.match(trimmed).hasMatch()) {
        errorMessage = "Имя может содержать только буквы, цифры, дефис и пробел";
        return false;
    }
    
    // запрет двойных дефисов и множественных пробелов
    if (trimmed.contains("--") || trimmed.contains(QRegularExpression(R"(\s{2,})"))) {
        errorMessage = "Имя не должно содержать двойные дефисы или множественные пробелы";
        return false;
    }
    
    return true;
}

// проверка телефона
bool ContactValidator::validatePhone(const QString& phone, QString& errorMessage)
{
    QString normalized = normalizePhone(phone);
    
    if (normalized.isEmpty()) {
        errorMessage = "Телефон не может быть пустым";
        return false;
    }
    
    // оставляем только цифры для проверки длины
    QString digits = extractPhoneDigits(normalized);
    
    if (digits.length() < 10) {
        errorMessage = "Телефон должен содержать минимум 10 цифр";
        return false;
    }
    if (digits.length() > 15) {
        errorMessage = "Телефон должен содержать максимум 15 цифр";
        return false;
    }
    
    // формат: должен быть + и цифры
    QRegularExpression formatRegex(R"(^\+\d+$)");
    if (!formatRegex.match(normalized).hasMatch()) {
        errorMessage = "Телефон должен быть в международном формате (+...)";
        return false;
    }
    
    return true;
}

// проверка даты рождения
bool ContactValidator::validateBirthDate(const QDate& date, QString& errorMessage)
{
    if (!date.isValid()) {
        errorMessage = "Неверная дата";
        return false;
    }
    
    QDate currentDate = QDate::currentDate();
    
    // дата должна быть в прошлом
    if (date >= currentDate) {
        errorMessage = "Дата рождения должна быть меньше текущей даты";
        return false;
    }
    
    // не старше 150 лет
    QDate minDate = currentDate.addYears(-150);
    if (date < minDate) {
        errorMessage = "Дата рождения слишком давняя";
        return false;
    }
    
    // тут можно было бы проверять дни/високосные года, но qt уже валидирует дату
    return true;
}

// проверка email
bool ContactValidator::validateEmail(const QString& email, QString& errorMessage)
{
    QString normalized = normalizeEmail(email);
    
    if (normalized.isEmpty()) {
        errorMessage = "Email не может быть пустым";
        return false;
    }
    
    // должно содержать @ ровно один
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
    
    // имя пользователя: латиница, цифры, точки, подчеркивания
    if (username.startsWith('.') || username.endsWith('.')) {
        errorMessage = "Имя пользователя не может начинаться или заканчиваться на точку";
        return false;
    }
    QRegularExpression usernameRegex(R"(^[A-Za-z0-9._%+\-]+$)");
    if (!usernameRegex.match(username).hasMatch()) {
        errorMessage = "Имя пользователя должно состоять из латинских букв, цифр, точек и подчеркиваний";
        return false;
    }
    
    // домен должен содержать точку
    if (!domain.contains('.')) {
        errorMessage = "Домен должен содержать минимум одну точку (например, example.com)";
        return false;
    }
    
    // проверка формата домена
    QRegularExpression domainRegex(R"(^[A-Za-z0-9][A-Za-z0-9\-]*(\.[A-Za-z0-9][A-Za-z0-9\-]*)*\.[A-Za-z]{2,}$)");
    if (!domainRegex.match(domain).hasMatch()) {
        errorMessage = "Неверный формат домена";
        return false;
    }
    
    return true;
}

// нормализация имени
QString ContactValidator::normalizeName(const QString& name)
{
    QString normalized = name.trimmed();
    
    if (normalized.isEmpty()) {
        return normalized;
    }
    
    // убираем лишние пробелы
    normalized.replace(QRegularExpression(R"(\s{2,})"), " ");
    // убираем повторные дефисы
    normalized.replace(QRegularExpression(R"(-{2,})"), "-");
    
    // первая буква заглавная
    normalized[0] = normalized[0].toUpper();
    
    // делаем буквы после пробела/дефиса заглавными, остальные строчными
    for (int i = 1; i < normalized.length(); ++i) {
        QChar current = normalized[i];
        QChar previous = normalized[i-1];
        if ((previous == ' ' || previous == '-') && current.isLetter()) {
            normalized[i] = current.toUpper();
        } else if (current.isLetter() && previous != ' ' && previous != '-') {
            normalized[i] = current.toLower();
        }
    }
    
    return normalized;
}

// нормализация телефона
QString ContactValidator::normalizePhone(const QString& phone)
{
    QString normalized = phone.trimmed();
    
    // удаляем пробелы, скобки, дефисы
    normalized.remove(QRegularExpression(R"([\s\(\)\-])"));
    
    // русский 8->+7
    if (normalized.startsWith("8")) {
        QString digits = extractPhoneDigits(normalized);
        if (digits.length() == 11) {
            normalized = "+7" + digits.mid(1);
        }
    }
    // если начинается на 7 без +, добавляем +
    else if (normalized.startsWith("7") && !normalized.startsWith("+")) {
        normalized = "+" + normalized;
    }
    // если нет +, добавляем
    else if (!normalized.startsWith("+")) {
        normalized = "+" + normalized;
    }
    
    return normalized;
}

// нормализация email
QString ContactValidator::normalizeEmail(const QString& email)
{
    QString normalized = email.trimmed().toLower();
    
    // убираем пробелы вокруг @ и все пробелы
    normalized.replace(QRegularExpression(R"(\s*@\s*)"), "@");
    normalized.remove(' ');
    
    return normalized;
}

// извлекаем только цифры
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

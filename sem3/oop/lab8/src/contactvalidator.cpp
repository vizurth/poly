#include "contactvalidator.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDate>
#include <QDebug>

bool ContactValidator::validateName(const QString& name, QString& errorMessage)
{
    QString normalized = normalizeName(name);
    
    if (normalized.isEmpty()) {
        errorMessage = "Имя не может быть пустым";
        return false;
    }
    
    // Должно начинаться с заглавной буквы
    if (normalized.isEmpty() || !normalized[0].isUpper()) {
        errorMessage = "Имя должно начинаться с заглавной буквы";
        return false;
    }
    
    // Не может начинаться или заканчиваться на дефис
    if (normalized.startsWith('-') || normalized.endsWith('-')) {
        errorMessage = "Имя не может начинаться или заканчиваться на дефис";
        return false;
    }
    
    // Регулярное выражение: буквы и цифры различных алфавитов, дефис и пробел
    // Должно начинаться с заглавной буквы
    QRegularExpression regex(R"(^[А-ЯЁA-Z][А-ЯЁа-яёA-Za-z0-9\s\-]*[А-ЯЁа-яёA-Za-z0-9]$|^[А-ЯЁA-Z][А-ЯЁа-яёA-Za-z0-9]$)");
    
    if (!regex.match(normalized).hasMatch()) {
        errorMessage = "Имя может содержать только буквы, цифры, дефис и пробел, и должно начинаться с заглавной буквы";
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
    
    // Международный формат: минимум 10 цифр (без кода страны), максимум 15
    if (digits.length() < 10 || digits.length() > 15) {
        errorMessage = "Телефон должен содержать от 10 до 15 цифр";
        return false;
    }
    
    // Проверяем, что все символы - цифры (после нормализации)
    QRegularExpression digitRegex(R"(^\d+$)");
    if (!digitRegex.match(digits).hasMatch()) {
        errorMessage = "Телефон должен содержать только цифры";
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
    if (date >= currentDate) {
        errorMessage = "Дата рождения должна быть меньше текущей даты";
        return false;
    }
    
    // Проверка месяца
    if (date.month() < 1 || date.month() > 12) {
        errorMessage = "Месяц должен быть от 1 до 12";
        return false;
    }
    
    // Проверка дня (учитывая количество дней в месяце)
    int daysInMonth = date.daysInMonth();
    if (date.day() < 1 || date.day() > daysInMonth) {
        errorMessage = QString("День должен быть от 1 до %1 для выбранного месяца").arg(daysInMonth);
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
    
    // Проверяем наличие @
    if (!normalized.contains('@')) {
        errorMessage = "Email должен содержать символ @";
        return false;
    }
    
    // Разделяем на имя пользователя и домен
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
    
    // Имя пользователя: латинские буквы и цифры
    QRegularExpression usernameRegex(R"(^[A-Za-z0-9]+$)");
    if (!usernameRegex.match(username).hasMatch()) {
        errorMessage = "Имя пользователя должно состоять из латинских букв и цифр";
        return false;
    }
    
    // Домен: латинские буквы и цифры
    QRegularExpression domainRegex(R"(^[A-Za-z0-9]+(\.[A-Za-z0-9]+)*$)");
    if (!domainRegex.match(domain).hasMatch()) {
        errorMessage = "Домен должен состоять из латинских букв и цифр";
        return false;
    }
    
    return true;
}

QString ContactValidator::normalizeName(const QString& name)
{
    QString normalized = name.trimmed();
    
    // Удаляем множественные пробелы
    normalized.replace(QRegularExpression(R"(\s+)"), " ");
    
    // Делаем первую букву заглавной, если она есть
    if (!normalized.isEmpty() && normalized[0].isLower()) {
        normalized[0] = normalized[0].toUpper();
    }
    
    return normalized;
}

QString ContactValidator::normalizePhone(const QString& phone)
{
    QString normalized = phone.trimmed();
    
    // Удаляем все пробелы, скобки, дефисы для хранения
    // Но для отображения можем оставить форматирование
    normalized.remove(QRegularExpression(R"([\s\(\)\-])"));
    
    // Если начинается с +, оставляем
    // Если начинается с 8, заменяем на +7
    if (normalized.startsWith("8") && normalized.length() == 11) {
        normalized = "+7" + normalized.mid(1);
    } else if (!normalized.startsWith("+")) {
        normalized = "+" + normalized;
    }
    
    return normalized;
}

QString ContactValidator::normalizeEmail(const QString& email)
{
    QString normalized = email.trimmed();
    
    // Удаляем пробелы вокруг @
    normalized.replace(QRegularExpression(R"(\s*@\s*)"), "@");
    
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


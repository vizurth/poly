#ifndef CONTACTVALIDATOR_H
#define CONTACTVALIDATOR_H

#include <QString>
#include <QDate>
#include <QStringList>

class ContactValidator
{
public:
    static bool validateName(const QString& name, QString& errorMessage);
    static bool validatePhone(const QString& phone, QString& errorMessage);
    static bool validateBirthDate(const QDate& date, QString& errorMessage);
    static bool validateEmail(const QString& email, QString& errorMessage);

    // Normalize functions
    static QString normalizeName(const QString& name);
    static QString normalizePhone(const QString& phone);
    static QString normalizeEmail(const QString& email);

private:
    static QString extractPhoneDigits(const QString& phone);
};

#endif // CONTACTVALIDATOR_H


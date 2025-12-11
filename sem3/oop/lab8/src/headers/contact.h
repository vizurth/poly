#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QDate>
#include <QStringList>
#include <QJsonObject>

class Contact
{
public:
    Contact();
    Contact(const QString& firstName, const QString& lastName, const QString& middleName,
            const QString& address, const QDate& birthDate, const QString& email,
            const QStringList& phoneNumbers);

    QString firstName() const { return m_firstName; }
    QString lastName() const { return m_lastName; }
    QString middleName() const { return m_middleName; }
    QString address() const { return m_address; }
    QDate birthDate() const { return m_birthDate; }
    QString email() const { return m_email; }
    QStringList phoneNumbers() const { return m_phoneNumbers; }

    void setFirstName(const QString& firstName) { m_firstName = firstName; }
    void setLastName(const QString& lastName) { m_lastName = lastName; }
    void setMiddleName(const QString& middleName) { m_middleName = middleName; }
    void setAddress(const QString& address) { m_address = address; }
    void setBirthDate(const QDate& birthDate) { m_birthDate = birthDate; }
    void setEmail(const QString& email) { m_email = email; }
    void setPhoneNumbers(const QStringList& phoneNumbers) { m_phoneNumbers = phoneNumbers; }

    QJsonObject toJson() const;
    static Contact fromJson(const QJsonObject& json);

    bool operator<(const Contact& other) const;
    bool operator==(const Contact& other) const;

private:
    QString m_firstName;
    QString m_lastName;
    QString m_middleName;
    QString m_address;
    QDate m_birthDate;
    QString m_email;
    QStringList m_phoneNumbers;
};

#endif // CONTACT_H


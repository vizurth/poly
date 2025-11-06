#include "contact.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

Contact::Contact()
    : m_birthDate(QDate::currentDate())
{
}

Contact::Contact(const QString& firstName, const QString& lastName, const QString& middleName,
                 const QString& address, const QDate& birthDate, const QString& email,
                 const QStringList& phoneNumbers)
    : m_firstName(firstName)
    , m_lastName(lastName)
    , m_middleName(middleName)
    , m_address(address)
    , m_birthDate(birthDate)
    , m_email(email)
    , m_phoneNumbers(phoneNumbers)
{
}

QJsonObject Contact::toJson() const
{
    QJsonObject json;
    json["firstName"] = m_firstName;
    json["lastName"] = m_lastName;
    json["middleName"] = m_middleName;
    json["address"] = m_address;
    json["birthDate"] = m_birthDate.toString(Qt::ISODate);
    json["email"] = m_email;
    
    QJsonArray phoneArray;
    for (const QString& phone : m_phoneNumbers) {
        phoneArray.append(phone);
    }
    json["phoneNumbers"] = phoneArray;
    
    return json;
}

Contact Contact::fromJson(const QJsonObject& json)
{
    Contact contact;
    contact.m_firstName = json["firstName"].toString();
    contact.m_lastName = json["lastName"].toString();
    contact.m_middleName = json["middleName"].toString();
    contact.m_address = json["address"].toString();
    contact.m_birthDate = QDate::fromString(json["birthDate"].toString(), Qt::ISODate);
    contact.m_email = json["email"].toString();
    
    QJsonArray phoneArray = json["phoneNumbers"].toArray();
    for (const QJsonValue& value : phoneArray) {
        contact.m_phoneNumbers.append(value.toString());
    }
    
    return contact;
}

bool Contact::operator<(const Contact& other) const
{
    if (m_lastName != other.m_lastName) {
        return m_lastName < other.m_lastName;
    }
    if (m_firstName != other.m_firstName) {
        return m_firstName < other.m_firstName;
    }
    return m_middleName < other.m_middleName;
}

bool Contact::operator==(const Contact& other) const
{
    return m_firstName == other.m_firstName &&
           m_lastName == other.m_lastName &&
           m_middleName == other.m_middleName &&
           m_address == other.m_address &&
           m_birthDate == other.m_birthDate &&
           m_email == other.m_email &&
           m_phoneNumbers == other.m_phoneNumbers;
}


#include "contactstorage.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

ContactStorage::ContactStorage(const QString& filename)
    : m_filename(filename)
{
}

bool ContactStorage::load()
{
    QFile file(m_filename);
    if (!file.exists()) {
        // Файл не существует - это нормально, просто пустая база
        m_contacts.clear();
        return true;
    }
    
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Не удалось открыть файл для чтения:" << m_filename;
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Ошибка парсинга JSON:" << error.errorString();
        return false;
    }
    
    if (!doc.isArray()) {
        qDebug() << "JSON должен быть массивом";
        return false;
    }
    
    QJsonArray array = doc.array();
    m_contacts.clear();
    
    for (const QJsonValue& value : array) {
        if (value.isObject()) {
            Contact contact = Contact::fromJson(value.toObject());
            m_contacts.append(contact);
        }
    }
    
    return true;
}

bool ContactStorage::save()
{
    QFile file(m_filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Не удалось открыть файл для записи:" << m_filename;
        return false;
    }
    
    QJsonArray array;
    for (const Contact& contact : m_contacts) {
        array.append(contact.toJson());
    }
    
    QJsonDocument doc(array);
    file.write(doc.toJson());
    file.close();
    
    return true;
}

void ContactStorage::addContact(const Contact& contact)
{
    m_contacts.append(contact);
}

void ContactStorage::removeContact(int index)
{
    if (index >= 0 && index < m_contacts.size()) {
        m_contacts.removeAt(index);
    }
}

void ContactStorage::updateContact(int index, const Contact& contact)
{
    if (index >= 0 && index < m_contacts.size()) {
        m_contacts[index] = contact;
    }
}


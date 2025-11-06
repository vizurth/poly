#ifndef CONTACTSTORAGE_H
#define CONTACTSTORAGE_H

#include "contact.h"
#include <QList>
#include <QString>

class ContactStorage
{
public:
    ContactStorage(const QString& filename = "phonebook.json");
    
    bool load();
    bool save();
    
    QList<Contact>& contacts() { return m_contacts; }
    const QList<Contact>& contacts() const { return m_contacts; }
    
    void addContact(const Contact& contact);
    void removeContact(int index);
    void updateContact(int index, const Contact& contact);
    
    QString filename() const { return m_filename; }
    void setFilename(const QString& filename) { m_filename = filename; }

private:
    QList<Contact> m_contacts;
    QString m_filename;
};

#endif // CONTACTSTORAGE_H


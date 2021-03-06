/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -a decisionmanagementserviceadaptor -i protocol_types.h -i ddms_service.h -l Nepomuk::DecisionManagementService ../interfaces/org.kde.nepomuk.DecisionManagementService.xml
 *
 * qdbusxml2cpp is Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef DECISIONMANAGEMENTSERVICEADAPTOR_H_1310141726
#define DECISIONMANAGEMENTSERVICEADAPTOR_H_1310141726

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "protocol_types.h"
#include "ddms_service.h"
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface org.kde.nepomuk.DecisionManagement
 */
class DecisionManagementAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.nepomuk.DecisionManagement")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.kde.nepomuk.DecisionManagement\">\n"
"    <method name=\"removeDecision\">\n"
"      <arg direction=\"out\" type=\"i\"/>\n"
"      <arg direction=\"in\" type=\"i\" name=\"id\"/>\n"
"    </method>\n"
"    <method name=\"addDecision\">\n"
"      <arg direction=\"in\" type=\"ay\" name=\"decision\"/>\n"
"      <annotation value=\"IdAndError\" name=\"com.trolltech.QtDBus.QtTypeName.Out0\"/>\n"
"      <arg direction=\"out\" type=\"(ii)\"/>\n"
"    </method>\n"
"    <!--\n"
"    <method name=\"addDecision\">\n"
"	<arg name=\"decision\" type=\"s\" direction=\"in\" />\n"
"	<arg name=\"uriList\" type=\"as\" direction=\"in\" />\n"
"	<arg type=\"i\" direction=\"out\" />\n"
"	<annotation name=\"com.trolltech.QtDBus.QtTypeName.Out0\" value=\"int\"/>\n"
"	<arg type=\"i\" direction=\"out\" />\n"
"    </method>\n"
"    -->\n"
"    <method name=\"getDecisions\">\n"
"      <annotation value=\"IdList\" name=\"com.trolltech.QtDBus.QtTypeName.Out0\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"uri\"/>\n"
"      <arg direction=\"out\" type=\"ai\"/>\n"
"    </method>\n"
"    <method name=\"existsDecision\">\n"
"      <arg direction=\"in\" type=\"i\" name=\"id\"/>\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"    </method>\n"
"    <method name=\"decisionMetadata\">\n"
"      <annotation value=\"MetadataAndError\" name=\"com.trolltech.QtDBus.QtTypeName.Out0\"/>\n"
"      <arg direction=\"in\" type=\"i\" name=\"id\"/>\n"
"      <arg direction=\"out\" type=\"(i(s))\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    DecisionManagementAdaptor(Nepomuk::DecisionManagementService *parent);
    virtual ~DecisionManagementAdaptor();

    inline Nepomuk::DecisionManagementService *parent() const
    { return static_cast<Nepomuk::DecisionManagementService *>(QObject::parent()); }

public: // PROPERTIES
public Q_SLOTS: // METHODS
    IdAndError addDecision(const QByteArray &decision);
    MetadataAndError decisionMetadata(int id);
    bool existsDecision(int id);
    IdList getDecisions(const QString &uri);
    int removeDecision(int id);
Q_SIGNALS: // SIGNALS
};

#endif

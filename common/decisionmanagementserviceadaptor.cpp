/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -a decisionmanagementserviceadaptor -i protocol_types.h -i ddms_service.h -l Nepomuk::DecisionManagementService ../interfaces/org.kde.nepomuk.DecisionManagementService.xml
 *
 * qdbusxml2cpp is Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "decisionmanagementserviceadaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class DecisionManagementAdaptor
 */

DecisionManagementAdaptor::DecisionManagementAdaptor(Nepomuk::DecisionManagementService *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

DecisionManagementAdaptor::~DecisionManagementAdaptor()
{
    // destructor
}

IdAndError DecisionManagementAdaptor::addDecision(const QString &decision, const QStringList &uriList)
{
    // handle method call org.kde.nepomuk.DecisionManagement.addDecision
    return parent()->addDecision(decision, uriList);
}

bool DecisionManagementAdaptor::existsDecision(int id)
{
    // handle method call org.kde.nepomuk.DecisionManagement.existsDecision
    return parent()->existsDecision(id);
}

IdList DecisionManagementAdaptor::getDecisions(const QString &uri)
{
    // handle method call org.kde.nepomuk.DecisionManagement.getDecisions
    return parent()->getDecisions(uri);
}

int DecisionManagementAdaptor::removeDecision(int id)
{
    // handle method call org.kde.nepomuk.DecisionManagement.removeDecision
    return parent()->removeDecision(id);
}


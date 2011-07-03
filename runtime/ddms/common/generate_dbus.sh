#! /bin/sh
qdbusxml2cpp -a decisionmanagementserviceadaptor -i protocol_types.h -i ddms_service.h -l Nepomuk::DecisionManagementService ../interfaces/org.kde.nepomuk.DecisionManagementService.xml
qdbusxml2cpp -p decisionmanagementserviceproxy -i protocol_types.h   ../interfaces/org.kde.nepomuk.DecisionManagementService.xml

#pragma once
#pragma execution_character_set("utf-8")
#include <QApplication>
#include <QtWidgets>
#include <iostream>
#include <QCoreApplication>
#include <QDebug>
#include <ctime>
#include "langcontrol.h"
#include <exception>
using namespace std;
static QMap<QString, QString> msglist;
clock_t timeStart, timeEnd;

int langset(QString langname) {
    timeStart = clock();
    QString langnameraw = langname;
    QFile SPLSettingsFile;
    SPLSettingsFile.setFileName("./Language/base.ini");
    if (langnameraw == "0") {      
        SPLSettingsFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QString SPLSetting = "ERROR";
        if (SPLSettingsFile.isOpen()) {
            QTextStream SPLSettingsText(&SPLSettingsFile);
            SPLSettingsText.setCodec("UTF-8");
            SPLSetting = SPLSettingsText.readAll();
            SPLSettingsFile.close();
        }
        if (SPLSetting == "ERROR") {
            langname = "zh_SC";
            SPLSettingsFile.open(QIODevice::NewOnly | QIODevice::Text);
            SPLSettingsFile.write("zh_SC");
            SPLSettingsFile.close();
        }
        else {
            langname = SPLSetting;
        }
        if (langname == "") { langname = "zh_SC"; }
    }
    QFile spLanguageFile;
    spLanguageFile.setFileName(".\\Language\\" + langname + ".splang");
    spLanguageFile.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!spLanguageFile.isOpen()) {
        qDebug().noquote() << "sysinfo¡ú" + msg("Function_Language_File_NotFound").arg(langname);
        return 0;
    }
    else {
        SPLSettingsFile.remove();
        SPLSettingsFile.open(QIODevice::NewOnly | QIODevice::Text);
        SPLSettingsFile.write(langname.toStdString().c_str());
        SPLSettingsFile.close();
    }
    QTextStream spLanguageText(&spLanguageFile);
    spLanguageText.setCodec("UTF-8");
    QString spSingleLine;
    for (;;) {
        spSingleLine = spLanguageText.readLine();
        if (spSingleLine[0] == '#' || spSingleLine[0] == '\n') { continue; }
        msglist[spSingleLine.section(":", 0, 0)] = spSingleLine.section(":", 1, -1);
        if (spLanguageText.atEnd()) { break; }
    }
    timeEnd = clock();
    qDebug().noquote() << "sysinfo¡ú" + msg("Function_Language_Loading_End").arg(timeEnd - timeStart);
    return 1;
}

QString msg(QString Usript) {
    try {
        if (msglist[Usript] == "") {
            throw "FAQ";
        }
        else {
            return msglist[Usript];
        }
    }
    catch (...) {
        return Usript;
    }
}

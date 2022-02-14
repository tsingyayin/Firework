#pragma once
#include "loadsettings.h"
#include "global_value.h"
#include "langcontrol.h"

#include <QtCore>
static QMap<QString, QString> SettingsList;
QString Initial_prompt_statement = "#这个文档是程序设置的记录文档。\n#如需快速初始化程序设定，可以直接删除本文档。但这（可能）会影响程序在删除本文档后的首次启动速度。\n\n";

void writesettings(QString SettingsKey, QString SettingsValue)
{
    if (SettingsKey != "" && SettingsValue != "") {
        SettingsList[SettingsKey] = SettingsValue; 
    }
    QMap<QString, QString>::const_iterator i = SettingsList.constBegin();
    QString SettingsText;
    SettingsText += Initial_prompt_statement;
    for (i; i != SettingsList.constEnd(); i++) {
        SettingsText += (i.key() + "=" + i.value() + "\n");
    }

    QFile SettingsFile;
    SettingsFile.setFileName("./options.txt");
    SettingsFile.open(QIODevice::WriteOnly | QIODevice::Text);
    SettingsFile.write(SettingsText.toStdString().data());
    SettingsFile.close();
}

int loadsettings(void) {
    SettingsList["CodecPath"] = "__Default__";
    SettingsList["Window_Size"] = "1366_768";
    SettingsList["Window_SizePolicy"] = "N";
    SettingsList["Volume"] = "100";
    QFile settingsFile;
    settingsFile.setFileName("./options.txt");
    settingsFile.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!settingsFile.isOpen()) {
        qDebug().noquote() << "CAN NOT FIND INIT SETTINGS!";
        writesettings();
    }else{
        QTextStream settingsFileText(&settingsFile);
        settingsFileText.setCodec("UTF-8");
        QString stSingleLine;
        for (;;) {
            stSingleLine = settingsFileText.readLine();
            if (stSingleLine[0] == '#' || stSingleLine[0] == '\n') { continue; }
            SettingsList[stSingleLine.section("=", 0, 0)] = stSingleLine.section("=", 1, -1);
            if (settingsFileText.atEnd()) { break; }
        }
    }
    return 0;
}

QString Program_Settings(QString SettingsName) 
{
    try {
        if (SettingsList[SettingsName] == "") {
            throw "FAQ";
        }
        else {
            return SettingsList[SettingsName];
        }
    }
    catch (...) {
        return "UNKNOWNSET:" + SettingsName;
    }
}

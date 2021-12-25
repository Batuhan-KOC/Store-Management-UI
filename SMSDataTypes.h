#ifndef SMSDATATYPES_H
#define SMSDATATYPES_H

#include <QString>
#include <QVector>
#include <QMap>

class GeneralStoreItemType{
public:
    GeneralStoreItemType(){}
    ~GeneralStoreItemType(){}

    QString ItemName;
    QString UnitSuffix;
    QString Description;
    double UnitCost;
};

class KitContentType{
public:
    KitContentType(){}
    ~KitContentType(){}

    QString contentName;
    QString contentUnitSuffix;
    double contentAmount;
    double contentUnitCost;
    double contentCost;
};

class KitType{
public:
    KitType(){}
    ~KitType(){}

    QString KitName;
    QList<KitContentType> KitContent;
    double TotalCost;
};

#endif // SMSDATATYPES_H

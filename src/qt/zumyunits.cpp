// Copyright (c) 2009-2018 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Developers
// Copyright (c) 2014-2018 The Dash Core Developers
// Copyright (c) 2016-2018 Duality Blockchain Solutions Developers
// Copyright (c) 2017-2018 Zumy Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "zumyunits.h"

#include "chainparams.h"
#include "primitives/transaction.h"

#include <QSettings>
#include <QStringList>

ZumyUnits::ZumyUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<ZumyUnits::Unit> ZumyUnits::availableUnits()
{
    QList<ZumyUnits::Unit> unitlist;
    unitlist.append(ZMY);
    unitlist.append(mZMY);
    unitlist.append(uZMY);
    unitlist.append(satoshis);
    return unitlist;
}

bool ZumyUnits::valid(int unit)
{
    switch(unit)
    {
    case ZMY:
    case mZMY:
    case uZMY:
    case satoshis:
        return true;
    default:
        return false;
    }
}

QString ZumyUnits::id(int unit)
{
    switch(unit)
    {
    case ZMY: return QString("zmy");
    case mZMY: return QString("mzmy");
    case uZMY: return QString("uzmy");
    case satoshis: return QString("satoshis");
    default: return QString("???");
    }
}

QString ZumyUnits::name(int unit)
{
    if(Params().NetworkIDString() == CBaseChainParams::MAIN)
    {
        switch(unit)
        {
            case ZMY: return QString("ZMY");
            case mZMY: return QString("mZMY");
            case uZMY: return QString::fromUtf8("μZMY");
            case satoshis: return QString("satoshis");
            default: return QString("???");
        }
    }
    else
    {
        switch(unit)
        {
            case ZMY: return QString("tZMY");
            case mZMY: return QString("mtZMY");
            case uZMY: return QString::fromUtf8("μtZMY");
            case satoshis: return QString("tsatoshis");
            default: return QString("???");
        }
    }
}

QString ZumyUnits::description(int unit)
{
    if(Params().NetworkIDString() == CBaseChainParams::MAIN)
    {
        switch(unit)
        {
            case ZMY: return QString("Zumy");
            case mZMY: return QString("Milli-Zumy (1 / 1" THIN_SP_UTF8 "000)");
            case uZMY: return QString("Micro-Zumy (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
            case satoshis: return QString("Ten Nano-Zumy (1 / 100" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
            default: return QString("???");
        }
    }
    else
    {
        switch(unit)
        {
            case ZMY: return QString("TestZumy");
            case mZMY: return QString("Milli-TestZumy (1 / 1" THIN_SP_UTF8 "000)");
            case uZMY: return QString("Micro-TestZumy (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
            case satoshis: return QString("Ten Nano-TestZumy (1 / 100" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
            default: return QString("???");
        }
    }
}

qint64 ZumyUnits::factor(int unit)
{
    switch(unit)
    {
    case ZMY:  return 100000000;
    case mZMY: return 100000;
    case uZMY: return 100;
    case satoshis: return 1;
    default:   return 100000000;
    }
}

int ZumyUnits::decimals(int unit)
{
    switch(unit)
    {
    case ZMY: return 8;
    case mZMY: return 5;
    case uZMY: return 2;
    case satoshis: return 0;
    default: return 0;
    }
}

QString ZumyUnits::format(int unit, const CAmount& nIn, bool fPlus, SeparatorStyle separators)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(!valid(unit))
        return QString(); // Refuse to format invalid unit
    qint64 n = (qint64)nIn;
    qint64 coin = factor(unit);
    int num_decimals = decimals(unit);
    qint64 n_abs = (n > 0 ? n : -n);
    qint64 quotient = n_abs / coin;
    qint64 remainder = n_abs % coin;
    QString quotient_str = QString::number(quotient);
    QString remainder_str = QString::number(remainder).rightJustified(num_decimals, '0');

    // Use SI-style thin space separators as these are locale independent and can't be
    // confused with the decimal marker.
    QChar thin_sp(THIN_SP_CP);
    int q_size = quotient_str.size();
    if (separators == separatorAlways || (separators == separatorStandard && q_size > 4))
        for (int i = 3; i < q_size; i += 3)
            quotient_str.insert(q_size - i, thin_sp);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');

    if (num_decimals <= 0)
        return quotient_str;

    return quotient_str + QString(".") + remainder_str;
}


// NOTE: Using formatWithUnit in an HTML context risks wrapping
// quantities at the thousands separator. More subtly, it also results
// in a standard space rather than a thin space, due to a bug in Qt's
// XML whitespace canonicalisation
//
// Please take care to use formatHtmlWithUnit instead, when
// appropriate.

QString ZumyUnits::formatWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    return format(unit, amount, plussign, separators) + QString(" ") + name(unit);
}

QString ZumyUnits::formatHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QString str(formatWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}

QString ZumyUnits::floorWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QSettings settings;
    int digits = settings.value("digits").toInt();

    QString result = format(unit, amount, plussign, separators);
    if(decimals(unit) > digits) result.chop(decimals(unit) - digits);

    return result + QString(" ") + name(unit);
}

QString ZumyUnits::floorHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QString str(floorWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}

bool ZumyUnits::parse(int unit, const QString &value, CAmount *val_out)
{
    if(!valid(unit) || value.isEmpty())
        return false; // Refuse to parse invalid unit or empty string
    int num_decimals = decimals(unit);

    // Ignore spaces and thin spaces when parsing
    QStringList parts = removeSpaces(value).split(".");

    if(parts.size() > 2)
    {
        return false; // More than one dot
    }
    QString whole = parts[0];
    QString decimals;

    if(parts.size() > 1)
    {
        decimals = parts[1];
    }
    if(decimals.size() > num_decimals)
    {
        return false; // Exceeds max precision
    }
    bool ok = false;
    QString str = whole + decimals.leftJustified(num_decimals, '0');

    if(str.size() > 18)
    {
        return false; // Longer numbers will exceed 63 bits
    }
    CAmount retvalue(str.toLongLong(&ok));
    if(val_out)
    {
        *val_out = retvalue;
    }
    return ok;
}

QString ZumyUnits::getAmountColumnTitle(int unit)
{
    QString amountTitle = QObject::tr("Amount");
    if (ZumyUnits::valid(unit))
    {
        amountTitle += " ("+ZumyUnits::name(unit) + ")";
    }
    return amountTitle;
}

int ZumyUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant ZumyUnits::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < unitlist.size())
    {
        Unit unit = unitlist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}

CAmount ZumyUnits::maxMoney()
{
    return MAX_MONEY;
}

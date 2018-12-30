// Copyright (c) 2009-2018 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Developers
// Copyright (c) 2014-2018 The Dash Core Developers
// Copyright (c) 2016-2018 Duality Blockchain Solutions Developers
// Copyright (c) 2017-2018 Zumy Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ZUMY_QT_ZUMYADDRESSVALIDATOR_H
#define ZUMY_QT_ZUMYADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class ZumyAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ZumyAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Zumy address widget validator, checks for a valid Zumy address.
 */
class ZumyAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ZumyAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // ZUMY_QT_ZUMYADDRESSVALIDATOR_H

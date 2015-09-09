// Copyright (c) 2013-2015 The Bitcoin sCrypt developers
// Copyright (c) 2009-2013 The Bitcoin developers
// Copyright (c) 2011-2012 The Litecoin developers
// Copyright (c) 2009-2010 Satoshi Nakamoto
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef BITCOIN_INIT_H
#define BITCOIN_INIT_H

#include "wallet.h"

extern CWallet* pwalletMain;

void StartShutdown();
void Shutdown(void* parg);
bool AppInit2();
std::string HelpMessage();

#endif

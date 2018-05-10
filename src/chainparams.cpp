// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        //add 1 to the start message for Linda 1.0.1 so we dont talk to old wallets, and change the default port's
        pchMessageStart[0] = 0x3b;
        pchMessageStart[1] = 0x60;
        pchMessageStart[2] = 0x40;
        pchMessageStart[3] = 0x3e;
        vAlertPubKey = ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f");
        nDefaultPort = 27540;
        nRPCPort = 27541;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);


        const char* pszTimestamp = "CNN - April 27 2018 - North and South Korean leaders hold historic summit";
        CTransaction txNew;
        txNew.nTime = 1524821199;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].SetEmpty();
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1524821199;
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 1989540;

       //  printf("calc new genesis block\n");
       //    printf("hashMerkleRoot %s\n", genesis.hashMerkleRoot.ToString().c_str());
       //     printf("bnProofOfWorkLimit 0x%x\n", bnProofOfWorkLimit.GetCompact());
       //     printf("genesis.nBits 0x%x\n", genesis.nBits);
//
//            for (genesis.nNonce = 0; ; genesis.nNonce++) {
//                hashGenesisBlock = genesis.GetHash();
//                if (hashGenesisBlock <= bnProofOfWorkLimit.getuint256()) break;
//            }
//
//            printf("hashGenesisBlock %s\n", hashGenesisBlock.ToString().c_str());
// 	printf("genesis.nNonce %d\n", genesis.nNonce);

        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("0x00000a8bba13ca4859bb523b0bf9cf8a39e83822fdbe36d677af7c73b7711d1a"));
        assert(genesis.hashMerkleRoot == uint256("0x08dfafdd36758ce537279e75197a7bfa591c04696e12d127afbf6c83497fe156"));


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,48);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,85);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,153);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        nLastPOWBlock = 10000000;
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xce;
        pchMessageStart[1] = 0xf2;
        pchMessageStart[2] = 0xc0;
        pchMessageStart[3] = 0xef;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
        vAlertPubKey = ParseHex("040f02342fd2fd9d30bc1d036f0b5f096cb93f46391061760db32a365cd1cda77dc4858b0532f8d96ba1ffd612f190706a2d41d8bb59620fc35c9c015d652c6227");
        nDefaultPort = 28888;
        nRPCPort = 28889;
        strDataDir = "testnet";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 38540;
        	
//		printf("calc new genesis block\n");
//        	printf("hashMerkleRoot %s\n", genesis.hashMerkleRoot.ToString().c_str());
//         	printf("bnProofOfWorkLimit 0x%x\n", bnProofOfWorkLimit.GetCompact());
//         	printf("genesis.nBits 0x%x\n", genesis.nBits);
//
//        	for (genesis.nNonce = 0; ; genesis.nNonce++) {
//                hashGenesisBlock = genesis.GetHash();
//                if (hashGenesisBlock <= bnProofOfWorkLimit.getuint256()) break;
//            }
//
//	        printf("hashGenesisBlock %s\n", hashGenesisBlock.ToString().c_str());
//	 	printf("genesis.nNonce %d\n", genesis.nNonce);

        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("0x000006d2446a534e7a0068a69dc83dba4555ed657ed328b5c4f63af5b9239fbc"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,23);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,187);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        nLastPOWBlock = 0x7fffffff;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    
    bool fTestNet = GetBoolArg("-testnet", false);
    
    if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}


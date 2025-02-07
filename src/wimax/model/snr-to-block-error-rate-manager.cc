/*
 *  Copyright (c) 2007,2008, 2009 INRIA, UDcast
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 *                              <amine.ismail@udcast.com>
 */

#include "snr-to-block-error-rate-manager.h"

#include "default-traces.h"
#include "snr-to-block-error-rate-record.h"

#include "ns3/assert.h"
#include "ns3/log.h"

#include <cstring>
#include <fstream>
#include <sstream>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("SNRToBlockErrorRateManager");

SNRToBlockErrorRateManager::SNRToBlockErrorRateManager()
{
    for (int i = 0; i < 7; i++)
    {
        m_recordModulation[i] = new std::vector<SNRToBlockErrorRateRecord*>();
    }
    m_activateLoss = false;
    m_traceFilePath = "DefaultTraces";
}

SNRToBlockErrorRateManager::~SNRToBlockErrorRateManager()
{
    ClearRecords();
    for (int i = 0; i < 7; i++)
    {
        delete m_recordModulation[i];
    }
}

void
SNRToBlockErrorRateManager::ClearRecords()
{
    for (int i = 0; i < 7; i++)
    {
        for (auto iter = m_recordModulation[i]->begin(); iter != m_recordModulation[i]->end();
             ++iter)
        {
            if (*iter)
            {
                delete (*iter);
                (*iter) = 0;
            }
        }
        m_recordModulation[i]->clear();
    }
}

void
SNRToBlockErrorRateManager::ActivateLoss(bool loss)
{
    m_activateLoss = loss;
}

void
SNRToBlockErrorRateManager::LoadTraces()
{
    std::ifstream traceFile;
    ClearRecords();
    double snrValue;
    double bitErrorRate;
    double burstErrorRate;
    double sigma2;
    double I1;
    double I2;

    for (int i = 0; i < 7; i++)
    {
        std::stringstream traceFilePath;
        traceFilePath << m_traceFilePath << "/modulation" << i << ".txt";

        traceFile.open(traceFilePath.str(), std::ifstream::in);
        if (!traceFile.good())
        {
            NS_LOG_INFO("Unable to load " << traceFilePath.str() << "!! Loading default traces...");
            LoadDefaultTraces();
            return;
        }
        while (traceFile.good())
        {
            traceFile >> snrValue >> bitErrorRate >> burstErrorRate >> sigma2 >> I1 >> I2;
            auto record = new SNRToBlockErrorRateRecord(snrValue,
                                                        bitErrorRate,
                                                        burstErrorRate,
                                                        sigma2,
                                                        I1,
                                                        I2);
            m_recordModulation[i]->push_back(record);
        }
        traceFile.close();
    }
    m_activateLoss = true;
}

void
SNRToBlockErrorRateManager::LoadDefaultTraces()
{
    double snrValue;
    double bitErrorRate;
    double burstErrorRate;
    double sigma2;
    double I1;
    double I2;
    ClearRecords();
    for (unsigned int j = 0; j < sizeof(modulation0[0]) / sizeof(double); j++)
    {
        snrValue = modulation0[0][j];
        bitErrorRate = modulation0[1][j];
        burstErrorRate = modulation0[2][j];
        sigma2 = modulation0[3][j];
        I1 = modulation0[4][j];
        I2 = modulation0[5][j];
        auto record =
            new SNRToBlockErrorRateRecord(snrValue, bitErrorRate, burstErrorRate, sigma2, I1, I2);
        m_recordModulation[0]->push_back(record);
    }
    for (unsigned int j = 0; j < sizeof(modulation1[0]) / sizeof(double); j++)
    {
        snrValue = modulation1[0][j];
        bitErrorRate = modulation1[1][j];
        burstErrorRate = modulation1[2][j];
        sigma2 = modulation1[3][j];
        I1 = modulation1[4][j];
        I2 = modulation1[5][j];
        auto record =
            new SNRToBlockErrorRateRecord(snrValue, bitErrorRate, burstErrorRate, sigma2, I1, I2);
        m_recordModulation[1]->push_back(record);
    }
    for (unsigned int j = 0; j < sizeof(modulation2[0]) / sizeof(double); j++)
    {
        snrValue = modulation2[0][j];
        bitErrorRate = modulation2[1][j];
        burstErrorRate = modulation2[2][j];
        sigma2 = modulation2[3][j];
        I1 = modulation2[4][j];
        I2 = modulation2[5][j];
        auto record =
            new SNRToBlockErrorRateRecord(snrValue, bitErrorRate, burstErrorRate, sigma2, I1, I2);
        m_recordModulation[2]->push_back(record);
    }
    for (unsigned int j = 0; j < sizeof(modulation3[0]) / sizeof(double); j++)
    {
        snrValue = modulation3[0][j];
        bitErrorRate = modulation3[1][j];
        burstErrorRate = modulation3[2][j];
        sigma2 = modulation3[3][j];
        I1 = modulation3[4][j];
        I2 = modulation3[5][j];
        auto record =
            new SNRToBlockErrorRateRecord(snrValue, bitErrorRate, burstErrorRate, sigma2, I1, I2);
        m_recordModulation[3]->push_back(record);
    }
    for (unsigned int j = 0; j < sizeof(modulation4[0]) / sizeof(double); j++)
    {
        snrValue = modulation4[0][j];
        bitErrorRate = modulation4[1][j];
        burstErrorRate = modulation4[2][j];
        sigma2 = modulation4[3][j];
        I1 = modulation4[4][j];
        I2 = modulation4[5][j];
        auto record =
            new SNRToBlockErrorRateRecord(snrValue, bitErrorRate, burstErrorRate, sigma2, I1, I2);
        m_recordModulation[4]->push_back(record);
    }
    for (unsigned int j = 0; j < sizeof(modulation5[0]) / sizeof(double); j++)
    {
        snrValue = modulation5[0][j];
        bitErrorRate = modulation5[1][j];
        burstErrorRate = modulation5[2][j];
        sigma2 = modulation5[3][j];
        I1 = modulation5[4][j];
        I2 = modulation5[5][j];
        auto record =
            new SNRToBlockErrorRateRecord(snrValue, bitErrorRate, burstErrorRate, sigma2, I1, I2);
        m_recordModulation[5]->push_back(record);
    }
    for (unsigned int j = 0; j < sizeof(modulation6[0]) / sizeof(double); j++)
    {
        snrValue = modulation6[0][j];
        bitErrorRate = modulation6[1][j];
        burstErrorRate = modulation6[2][j];
        sigma2 = modulation6[3][j];
        I1 = modulation6[4][j];
        I2 = modulation6[5][j];
        auto record =
            new SNRToBlockErrorRateRecord(snrValue, bitErrorRate, burstErrorRate, sigma2, I1, I2);
        m_recordModulation[6]->push_back(record);
    }
    m_activateLoss = true;
}

void
SNRToBlockErrorRateManager::ReLoadTraces()
{
    double snrValue;
    double bitErrorRate;
    double burstErrorRate;
    double sigma2;
    double I1;
    double I2;

    ClearRecords();

    std::ifstream traceFile;

    for (int i = 0; i < 7; i++)
    {
        std::stringstream traceFilePath;
        traceFilePath << m_traceFilePath << "/Modulation" << i << ".txt";

        traceFile.open(traceFilePath.str(), std::ifstream::in);
        if (!traceFile.good())
        {
            NS_LOG_INFO("Unable to load " << traceFilePath.str() << "!!Loading default traces...");
            LoadDefaultTraces();
            return;
        }
        while (traceFile.good())
        {
            traceFile >> snrValue >> bitErrorRate >> burstErrorRate >> sigma2 >> I1 >> I2;
            auto record = new SNRToBlockErrorRateRecord(snrValue,
                                                        bitErrorRate,
                                                        burstErrorRate,
                                                        sigma2,
                                                        I1,
                                                        I2);

            m_recordModulation[i]->push_back(record);
        }
        traceFile.close();
    }
    m_activateLoss = true;
}

void
SNRToBlockErrorRateManager::SetTraceFilePath(char* traceFilePath)
{
    m_traceFilePath = traceFilePath;
}

std::string
SNRToBlockErrorRateManager::GetTraceFilePath()
{
    return m_traceFilePath;
}

double
SNRToBlockErrorRateManager::GetBlockErrorRate(double SNR, uint8_t modulation)
{
    if (!m_activateLoss)
    {
        return 0;
    }

    std::vector<SNRToBlockErrorRateRecord*>* record = nullptr;

    record = m_recordModulation[modulation];

    if (SNR <= (record->at(0)->GetSNRValue()))
    {
        return 1;
    }
    if (SNR >= (record->at(record->size() - 1)->GetSNRValue()))
    {
        return 0;
    }

    unsigned int i;
    for (i = 0; i < record->size(); i++)
    {
        if (SNR < record->at(i)->GetSNRValue())
        {
            break;
        }
    }
    double intervalSize = (record->at(i)->GetSNRValue() - record->at(i - 1)->GetSNRValue());
    double coeff1 = (SNR - record->at(i - 1)->GetSNRValue()) / intervalSize;
    double coeff2 = -1 * (SNR - record->at(i)->GetSNRValue()) / intervalSize;
    double BlockErrorRate = coeff2 * (record->at(i - 1)->GetBlockErrorRate()) +
                            coeff1 * (record->at(i)->GetBlockErrorRate());
    return BlockErrorRate;
}

SNRToBlockErrorRateRecord*
SNRToBlockErrorRateManager::GetSNRToBlockErrorRateRecord(double SNR, uint8_t modulation)
{
    if (!m_activateLoss)
    {
        return new SNRToBlockErrorRateRecord(SNR, 0, 0, 0, 0, 0);
    }

    std::vector<SNRToBlockErrorRateRecord*>* record = nullptr;
    record = m_recordModulation[modulation];

    if (SNR <= (record->at(0)->GetSNRValue()))
    {
        return record->at(0)->Copy();
    }
    if (SNR >= (record->at(record->size() - 1)->GetSNRValue()))
    {
        return record->at(record->size() - 1)->Copy();
    }

    unsigned int i;
    for (i = 0; i < record->size(); i++)
    {
        if (SNR < record->at(i)->GetSNRValue())
        {
            break;
        }
    }
    double intervalSize = (record->at(i)->GetSNRValue() - record->at(i - 1)->GetSNRValue());
    double coeff1 = (SNR - record->at(i - 1)->GetSNRValue()) / intervalSize;
    double coeff2 = -1 * (SNR - record->at(i)->GetSNRValue()) / intervalSize;
    double BER = coeff2 * (record->at(i - 1)->GetBitErrorRate()) +
                 coeff1 * (record->at(i)->GetBitErrorRate());
    double BlcER = coeff2 * (record->at(i - 1)->GetBlockErrorRate()) +
                   coeff1 * (record->at(i)->GetBlockErrorRate());
    double sigma2 =
        coeff2 * (record->at(i - 1)->GetSigma2()) + coeff1 * (record->at(i)->GetSigma2());
    double I1 = coeff2 * (record->at(i - 1)->GetI1()) + coeff1 * (record->at(i)->GetI1());
    double I2 = coeff2 * (record->at(i - 1)->GetI2()) + coeff1 * (record->at(i)->GetI2());

    auto SNRToBlockErrorRate = new SNRToBlockErrorRateRecord(SNR, BER, BlcER, sigma2, I1, I2);
    return SNRToBlockErrorRate;
}

} // namespace ns3

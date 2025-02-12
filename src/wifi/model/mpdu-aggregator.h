/*
 * Copyright (c) 2013
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Ghada Badawy <gbadawy@gmail.com>
 */

#ifndef MPDU_AGGREGATOR_H
#define MPDU_AGGREGATOR_H

#include "qos-utils.h"
#include "wifi-mode.h"

#include "ns3/nstime.h"
#include "ns3/object.h"

#include <vector>

namespace ns3
{

class AmpduSubframeHeader;
class WifiTxVector;
class QosTxop;
class Packet;
class WifiMac;
class WifiMpdu;
class WifiTxParameters;
class HtFrameExchangeManager;

/**
 * @brief Aggregator used to construct A-MPDUs
 * @ingroup wifi
 */
class MpduAggregator : public Object
{
  public:
    /**
     * EDCA queues typedef
     */
    typedef std::map<AcIndex, Ptr<QosTxop>> EdcaQueues;

    /**
     * @brief Get the type ID.
     * @return the object TypeId
     */
    static TypeId GetTypeId();

    MpduAggregator() = default;
    ~MpduAggregator() override = default;

    /**
     * Aggregate an MPDU to an A-MPDU.
     *
     * @param mpdu the MPDU.
     * @param ampdu the A-MPDU.
     * @param isSingle whether it is a single MPDU.
     */
    static void Aggregate(Ptr<const WifiMpdu> mpdu, Ptr<Packet> ampdu, bool isSingle);

    /**
     * Set the ID of the link this MPDU aggregator is associated with.
     *
     * @param linkId the ID of the link this MPDU aggregator is associated with
     */
    void SetLinkId(uint8_t linkId);

    /**
     * Compute the size of the A-MPDU resulting from the aggregation of an MPDU of
     * size <i>mpduSize</i> and an A-MPDU of size <i>ampduSize</i>.
     *
     * @param mpduSize the MPDU size in bytes.
     * @param ampduSize the A-MPDU size in bytes.
     * @return the size of the resulting A-MPDU in bytes.
     */
    static uint32_t GetSizeIfAggregated(uint32_t mpduSize, uint32_t ampduSize);

    /**
     * Determine the maximum size for an A-MPDU of the given TID that can be sent
     * to the given receiver when using the given modulation class.
     *
     * @param recipient the receiver station address.
     * @param tid the TID.
     * @param modulation the modulation class.
     * @return the maximum A-MPDU size in bytes.
     */
    uint32_t GetMaxAmpduSize(Mac48Address recipient,
                             uint8_t tid,
                             WifiModulationClass modulation) const;

    /**
     * Attempt to aggregate other MPDUs to the given MPDU, while meeting the
     * following constraints:
     *
     * - the size of the resulting A-MPDU does not exceed the maximum A-MPDU size
     * as determined for the modulation class indicated by the given TxVector
     *
     * - the time to transmit the resulting PPDU, according to the given TxVector,
     * does not exceed the maximum PPDU duration allowed by the corresponding
     * modulation class (if any)
     *
     * - the time to transmit the resulting PPDU and to carry out protection and
     * acknowledgment, as specified by the given TX parameters, does not exceed the
     * given available time (if distinct from Time::Min ())
     *
     * For now, only non-broadcast QoS Data frames can be aggregated (do not pass
     * other types of frames to this method). MPDUs to aggregate are looked for
     * among those with the same TID and receiver as the given MPDU.
     *
     * The resulting A-MPDU is returned as a vector of the constituent MPDUs
     * (including the given MPDU), which are not actually aggregated (call the
     * Aggregate method afterwards to get the actual A-MPDU). If aggregation was
     * not possible (aggregation is disabled, there is no Block Ack agreement
     * established with the receiver, or another MPDU to aggregate was not found),
     * the returned vector is empty.
     *
     * @param mpdu the given MPDU.
     * @param txParams the TX parameters used to transmit the frame
     * @param availableTime the time available for the frame exchange
     * @return the resulting A-MPDU, if aggregation is possible.
     */
    std::vector<Ptr<WifiMpdu>> GetNextAmpdu(Ptr<WifiMpdu> mpdu,
                                            WifiTxParameters& txParams,
                                            Time availableTime) const;

    /**
     * Set the MAC layer to use.
     *
     * @param mac the MAC layer to use
     */
    void SetWifiMac(const Ptr<WifiMac> mac);

    /**
     * @param ampduSize the size of the A-MPDU that needs to be padded in bytes
     * @return the size of the padding that must be added to the end of an A-MPDU in bytes
     *
     * Calculates how much padding must be added to the end of an A-MPDU of the given size
     * (once another MPDU is aggregated).
     * Each A-MPDU subframe is padded so that its length is multiple of 4 octets.
     */
    static uint8_t CalculatePadding(uint32_t ampduSize);

    /**
     * Get the A-MPDU subframe header corresponding to the MPDU size and
     * whether the MPDU is a single MPDU.
     *
     * @param mpduSize size of the MPDU in bytes.
     * @param isSingle true if S-MPDU.
     * @return the A-MPDU subframe header
     */
    static AmpduSubframeHeader GetAmpduSubframeHeader(uint16_t mpduSize, bool isSingle);

  protected:
    void DoDispose() override;

  private:
    Ptr<WifiMac> m_mac;                  //!< the MAC of this station
    Ptr<HtFrameExchangeManager> m_htFem; //!< the HT Frame Exchange Manager of this station
    uint8_t m_linkId{0};                 //!< ID of the link this object is connected to
};

} // namespace ns3

#endif /* MPDU_AGGREGATOR_H */

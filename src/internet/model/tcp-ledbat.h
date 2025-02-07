/*
 * Copyright (c) 2016 NITK Surathkal
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Ankit Deepak <adadeepak8@gmail.com>
 *
 */

#ifndef TCP_LEDBAT_H
#define TCP_LEDBAT_H

#include "tcp-congestion-ops.h"

#include <vector>

namespace ns3
{

class TcpSocketState;

/**
 * @ingroup congestionOps
 *
 * @brief An implementation of LEDBAT
 */

class TcpLedbat : public TcpNewReno
{
  private:
    /**
     * @brief The slowstart types
     */
    enum SlowStartType
    {
        DO_NOT_SLOWSTART, //!< Do not Slow Start
        DO_SLOWSTART,     //!< Do NewReno Slow Start
    };

    /**
     * @brief The state of LEDBAT. If LEDBAT is not in VALID_OWD state, it falls to
     *        default congestion ops.
     */
    enum State : uint32_t
    {
        LEDBAT_VALID_OWD = (1 << 1), //!< If valid timestamps are present
        LEDBAT_CAN_SS = (1 << 3)     //!< If LEDBAT allows Slow Start
    };

  public:
    /**
     * @brief Get the type ID.
     * @return the object TypeId
     */
    static TypeId GetTypeId();

    /**
     * Create an unbound tcp socket.
     */
    TcpLedbat();

    /**
     * @brief Copy constructor
     * @param sock the object to copy
     */
    TcpLedbat(const TcpLedbat& sock);

    /**
     * @brief Destructor
     */
    ~TcpLedbat() override;

    /**
     * @brief Get the name of the TCP flavour
     *
     * @return The name of the TCP
     */
    std::string GetName() const override;

    /**
     * @brief Get information from the acked packet
     *
     * @param tcb internal congestion state
     * @param segmentsAcked count of segments ACKed
     * @param rtt The estimated rtt
     */
    void PktsAcked(Ptr<TcpSocketState> tcb, uint32_t segmentsAcked, const Time& rtt) override;

    // Inherited
    Ptr<TcpCongestionOps> Fork() override;

    /**
     * @brief Adjust cwnd following LEDBAT algorithm
     *
     * @param tcb internal congestion state
     * @param segmentsAcked count of segments ACKed
     */
    void IncreaseWindow(Ptr<TcpSocketState> tcb, uint32_t segmentsAcked) override;

    /**
     * @brief Change the Slow Start Capability
     *
     * @param doSS Slow Start Option
     */
    void SetDoSs(SlowStartType doSS);

  protected:
    /**
     * @brief Reduce Congestion
     *
     * @param tcb internal congestion state
     * @param segmentsAcked count of segments ACKed
     */
    void CongestionAvoidance(Ptr<TcpSocketState> tcb, uint32_t segmentsAcked) override;

  private:
    /**
     * @brief Buffer structure to store delays
     */
    struct OwdCircBuf
    {
        std::vector<uint32_t> buffer; //!< Vector to store the delay
        uint32_t min;                 //!< The index of minimum value
    };

    /**
     * @brief Initialise a new buffer
     *
     * @param buffer The buffer to be initialised
     */
    void InitCircBuf(OwdCircBuf& buffer);

    /// Filter function used by LEDBAT for current delay
    typedef uint32_t (*FilterFunction)(OwdCircBuf&);

    /**
     * @brief Return the minimum delay of the buffer
     *
     * @param b The buffer
     * @return The minimum delay
     */
    static uint32_t MinCircBuf(OwdCircBuf& b);

    /**
     * @brief Return the value of current delay
     *
     * @param filter The filter function
     * @return The current delay
     */
    uint32_t CurrentDelay(FilterFunction filter);

    /**
     * @brief Return the value of base delay
     *
     * @return The base delay
     */
    uint32_t BaseDelay();

    /**
     * @brief Add new delay to the buffers
     *
     * @param cb The buffer
     * @param owd The new delay
     * @param maxlen The maximum permitted length
     */
    void AddDelay(OwdCircBuf& cb, uint32_t owd, uint32_t maxlen);

    /**
     * @brief Update the base delay buffer
     *
     * @param owd The delay
     */
    void UpdateBaseDelay(uint32_t owd);

    Time m_target;             //!< Target Queue Delay
    double m_gain;             //!< GAIN value from RFC
    SlowStartType m_doSs;      //!< Permissible Slow Start State
    uint32_t m_baseHistoLen;   //!< Length of base delay history buffer
    uint32_t m_noiseFilterLen; //!< Length of current delay buffer
    uint64_t m_lastRollover;   //!< Timestamp of last added delay
    int32_t m_sndCwndCnt;      //!< The congestion window addition parameter
    OwdCircBuf m_baseHistory;  //!< Buffer to store the base delay
    OwdCircBuf m_noiseFilter;  //!< Buffer to store the current delay
    uint32_t m_flag;           //!< LEDBAT Flag
    uint32_t m_minCwnd;        //!< Minimum cWnd value mentioned in RFC 6817
};

} // namespace ns3

#endif /* TCP_LEDBAT_H */

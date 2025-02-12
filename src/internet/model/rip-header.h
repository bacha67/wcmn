/*
 * Copyright (c) 2016 Universita' di Firenze, Italy
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Tommaso Pecorella <tommaso.pecorella@unifi.it>
 */

#ifndef RIP_HEADER_H
#define RIP_HEADER_H

#include "ipv4-header.h"

#include "ns3/header.h"
#include "ns3/ipv4-address.h"
#include "ns3/packet.h"

#include <list>

namespace ns3
{

/**
 * @ingroup rip
 * @brief Rip v2 Routing Table Entry (RTE) - see \RFC{2453}.
 */
class RipRte : public Header
{
  public:
    RipRte();

    /**
     * @brief Get the type ID.
     * @return The object TypeId.
     */
    static TypeId GetTypeId();

    /**
     * @brief Return the instance type identifier.
     * @return Instance type ID.
     */
    TypeId GetInstanceTypeId() const override;

    void Print(std::ostream& os) const override;

    /**
     * @brief Get the serialized size of the packet.
     * @return Size.
     */
    uint32_t GetSerializedSize() const override;

    /**
     * @brief Serialize the packet.
     * @param start Buffer iterator.
     */
    void Serialize(Buffer::Iterator start) const override;

    /**
     * @brief Deserialize the packet.
     * @param start Buffer iterator.
     * @return Size of the packet.
     */
    uint32_t Deserialize(Buffer::Iterator start) override;

    /**
     * @brief Set the prefix.
     * @param prefix The prefix.
     */
    void SetPrefix(Ipv4Address prefix);

    /**
     * @brief Get the prefix.
     * @returns The prefix.
     */
    Ipv4Address GetPrefix() const;

    /**
     * @brief Set the subnet mask.
     * @param subnetMask The subnet mask.
     */
    void SetSubnetMask(Ipv4Mask subnetMask);

    /**
     * @brief Get the subnet mask.
     * @returns The subnet mask.
     */
    Ipv4Mask GetSubnetMask() const;

    /**
     * @brief Set the route tag.
     * @param routeTag The route tag.
     */
    void SetRouteTag(uint16_t routeTag);

    /**
     * @brief Get the route tag.
     * @returns The route tag.
     */
    uint16_t GetRouteTag() const;

    /**
     * @brief Set the route metric.
     * @param routeMetric The route metric.
     */
    void SetRouteMetric(uint32_t routeMetric);

    /**
     * @brief Get the route metric.
     * @returns The route metric.
     */
    uint32_t GetRouteMetric() const;

    /**
     * @brief Set the next hop.
     * @param nextHop The next hop.
     */
    void SetNextHop(Ipv4Address nextHop);

    /**
     * @brief Get the next hop.
     * @returns The next hop.
     */
    Ipv4Address GetNextHop() const;

  private:
    uint16_t m_tag;        //!< Route tag.
    Ipv4Address m_prefix;  //!< Advertised prefix.
    Ipv4Mask m_subnetMask; //!< Subnet mask.
    Ipv4Address m_nextHop; //!< Next hop.
    uint32_t m_metric;     //!< Route metric.
};

/**
 * @brief Stream insertion operator.
 *
 * @param os the reference to the output stream
 * @param h the Routing Table Entry
 * @returns the reference to the output stream
 */
std::ostream& operator<<(std::ostream& os, const RipRte& h);

/**
 * @ingroup rip
 * @brief RipHeader - see \RFC{2453}
 */
class RipHeader : public Header
{
  public:
    RipHeader();

    /**
     * @brief Get the type ID.
     * @return the object TypeId
     */
    static TypeId GetTypeId();

    /**
     * @brief Return the instance type identifier.
     * @return instance type ID
     */
    TypeId GetInstanceTypeId() const override;

    void Print(std::ostream& os) const override;

    /**
     * @brief Get the serialized size of the packet.
     * @return size
     */
    uint32_t GetSerializedSize() const override;

    /**
     * @brief Serialize the packet.
     * @param start Buffer iterator
     */
    void Serialize(Buffer::Iterator start) const override;

    /**
     * @brief Deserialize the packet.
     * @param start Buffer iterator
     * @return size of the packet
     */
    uint32_t Deserialize(Buffer::Iterator start) override;

    /**
     * Commands to be used in Rip headers
     */
    enum Command_e
    {
        REQUEST = 0x1,
        RESPONSE = 0x2,
    };

    /**
     * @brief Set the command
     * @param command the command
     */
    void SetCommand(Command_e command);

    /**
     * @brief Get the command
     * @returns the command
     */
    Command_e GetCommand() const;

    /**
     * @brief Add a RTE to the message
     * @param rte the RTE
     */
    void AddRte(RipRte rte);

    /**
     * @brief Clear all the RTEs from the header
     */
    void ClearRtes();

    /**
     * @brief Get the number of RTE included in the message
     * @returns the number of RTE in the message
     */
    uint16_t GetRteNumber() const;

    /**
     * @brief Get the list of the RTEs included in the message
     * @returns the list of the RTEs in the message
     */
    std::list<RipRte> GetRteList() const;

  private:
    uint8_t m_command;           //!< command type
    std::list<RipRte> m_rteList; //!< list of the RTEs in the message
};

/**
 * @brief Stream insertion operator.
 *
 * @param os the reference to the output stream
 * @param h the Rip header
 * @returns the reference to the output stream
 */
std::ostream& operator<<(std::ostream& os, const RipHeader& h);

} // namespace ns3

#endif /* Rip_HEADER_H */

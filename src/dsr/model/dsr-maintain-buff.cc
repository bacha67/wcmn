/*
 * Copyright (c) 2011 Yufei Cheng
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Yufei Cheng   <yfcheng@ittc.ku.edu>
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  https://resilinets.org/
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 */

#include "dsr-maintain-buff.h"

#include "ns3/ipv4-route.h"
#include "ns3/log.h"
#include "ns3/socket.h"

#include <algorithm>
#include <functional>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("DsrMaintainBuffer");

namespace dsr
{

uint32_t
DsrMaintainBuffer::GetSize()
{
    Purge();
    return m_maintainBuffer.size();
}

bool
DsrMaintainBuffer::Enqueue(DsrMaintainBuffEntry& entry)
{
    Purge();
    for (auto i = m_maintainBuffer.begin(); i != m_maintainBuffer.end(); ++i)
    {
        //      NS_LOG_INFO ("nexthop " << i->GetNextHop () << " " << entry.GetNextHop () << " our
        //      add " << i->GetOurAdd () << " " << entry.GetOurAdd ()
        //                              << " src " << i->GetSrc () << " " << entry.GetSrc () << "
        //                              dst " << i->GetDst () << " " << entry.GetDst ()
        //                              << " ackId " << i->GetAckId () << " " << entry.GetAckId ()
        //                              << " SegsLeft " << (uint32_t)i->GetSegsLeft () << " " <<
        //                              (uint32_t)entry.GetSegsLeft ()
        //                   );

        if ((i->GetNextHop() == entry.GetNextHop()) && (i->GetOurAdd() == entry.GetOurAdd()) &&
            (i->GetSrc() == entry.GetSrc()) && (i->GetDst() == entry.GetDst()) &&
            (i->GetAckId() == entry.GetAckId()) && (i->GetSegsLeft() == entry.GetSegsLeft()))
        {
            NS_LOG_DEBUG("Same maintenance entry found");
            return false;
        }
    }

    entry.SetExpireTime(m_maintainBufferTimeout);
    if (m_maintainBuffer.size() >= m_maxLen)
    {
        NS_LOG_DEBUG("Drop the most aged packet");
        m_maintainBuffer.erase(m_maintainBuffer.begin()); // Drop the most aged packet
    }
    m_maintainBuffer.push_back(entry);
    return true;
}

void
DsrMaintainBuffer::DropPacketWithNextHop(Ipv4Address nextHop)
{
    NS_LOG_FUNCTION(this << nextHop);
    Purge();
    NS_LOG_INFO("Drop Packet With next hop " << nextHop);

    auto new_end =
        std::remove_if(m_maintainBuffer.begin(),
                       m_maintainBuffer.end(),
                       [&](const DsrMaintainBuffEntry& en) { return en.GetNextHop() == nextHop; });
    m_maintainBuffer.erase(new_end, m_maintainBuffer.end());
}

bool
DsrMaintainBuffer::Dequeue(Ipv4Address nextHop, DsrMaintainBuffEntry& entry)
{
    Purge();
    for (auto i = m_maintainBuffer.begin(); i != m_maintainBuffer.end(); ++i)
    {
        if (i->GetNextHop() == nextHop)
        {
            entry = *i;
            i = m_maintainBuffer.erase(i);
            NS_LOG_DEBUG("Packet size while dequeuing " << entry.GetPacket()->GetSize());
            return true;
        }
    }
    return false;
}

bool
DsrMaintainBuffer::Find(Ipv4Address nextHop)
{
    for (auto i = m_maintainBuffer.begin(); i != m_maintainBuffer.end(); ++i)
    {
        if (i->GetNextHop() == nextHop)
        {
            NS_LOG_DEBUG("Found the packet in maintenance buffer");
            return true;
        }
    }
    return false;
}

bool
DsrMaintainBuffer::AllEqual(DsrMaintainBuffEntry& entry)
{
    for (auto i = m_maintainBuffer.begin(); i != m_maintainBuffer.end(); ++i)
    {
        //      NS_LOG_DEBUG ("nexthop " << i->GetNextHop () << " " << entry.GetNextHop () << " our
        //      address " << i->GetOurAdd () << " " << entry.GetOurAdd ()
        //                               << " src " << i->GetSrc () << " " << entry.GetSrc () << "
        //                               dst " << i->GetDst () << " " << entry.GetDst ()
        //                               << " ackId " << i->GetAckId () << " " << entry.GetAckId
        //                               ());

        if ((i->GetOurAdd() == entry.GetOurAdd()) && (i->GetNextHop() == entry.GetNextHop()) &&
            (i->GetSrc() == entry.GetSrc()) && (i->GetDst() == entry.GetDst()) &&
            (i->GetAckId() == entry.GetAckId()) && (i->GetSegsLeft() == entry.GetSegsLeft()))
        {
            i = m_maintainBuffer.erase(
                i); // Erase the same maintain buffer entry for the received packet
            return true;
        }
    }
    return false;
}

bool
DsrMaintainBuffer::NetworkEqual(DsrMaintainBuffEntry& entry)
{
    for (auto i = m_maintainBuffer.begin(); i != m_maintainBuffer.end(); ++i)
    {
        //      NS_LOG_DEBUG ("nexthop " << i->GetNextHop () << " " << entry.GetNextHop () << " our
        //      address " << i->GetOurAdd () << " " << entry.GetOurAdd ()
        //                               << " src " << i->GetSrc () << " " << entry.GetSrc () << "
        //                               dst " << i->GetDst () << " " << entry.GetDst ()
        //                               << " ackId " << i->GetAckId () << " " << entry.GetAckId
        //                               ());

        if ((i->GetOurAdd() == entry.GetOurAdd()) && (i->GetNextHop() == entry.GetNextHop()) &&
            (i->GetSrc() == entry.GetSrc()) && (i->GetDst() == entry.GetDst()) &&
            (i->GetAckId() == entry.GetAckId()))
        {
            i = m_maintainBuffer.erase(
                i); // Erase the same maintain buffer entry for the received packet
            return true;
        }
    }
    return false;
}

bool
DsrMaintainBuffer::PromiscEqual(DsrMaintainBuffEntry& entry)
{
    NS_LOG_DEBUG("The maintenance buffer size " << m_maintainBuffer.size());
    for (auto i = m_maintainBuffer.begin(); i != m_maintainBuffer.end(); ++i)
    {
        //      NS_LOG_DEBUG ("src " << i->GetSrc () << " " << entry.GetSrc () << " dst " <<
        //      i->GetDst () << " " << entry.GetDst ()
        //                           << " SegsLeft " << (uint32_t)i->GetSegsLeft () << " " <<
        //                           (uint32_t)entry.GetSegsLeft () << " ackId " <<
        //                           (uint32_t)i->GetAckId () << " "
        //                           << (uint32_t)entry.GetAckId ()
        //                    );

        if ((i->GetSrc() == entry.GetSrc()) && (i->GetDst() == entry.GetDst()) &&
            (i->GetSegsLeft() == entry.GetSegsLeft()) && (i->GetAckId() == entry.GetAckId()))
        {
            i = m_maintainBuffer.erase(
                i); // Erase the same maintain buffer entry for the promisc received packet
            return true;
        }
    }
    return false;
}

bool
DsrMaintainBuffer::LinkEqual(DsrMaintainBuffEntry& entry)
{
    NS_LOG_DEBUG("The maintenance buffer size " << m_maintainBuffer.size());
    for (auto i = m_maintainBuffer.begin(); i != m_maintainBuffer.end(); ++i)
    {
        //      NS_LOG_DEBUG ("src " << i->GetSrc () << " " << entry.GetSrc () << " dst " <<
        //      i->GetDst () << " " << entry.GetDst ()
        //                           << " OurAddress " << i->GetOurAdd () << " " << entry.GetOurAdd
        //                           () << " next hop " << i->GetNextHop () << " "
        //                           << entry.GetNextHop ()
        //                    );

        if ((i->GetSrc() == entry.GetSrc()) && (i->GetDst() == entry.GetDst()) &&
            (i->GetOurAdd() == entry.GetOurAdd()) && (i->GetNextHop() == entry.GetNextHop()))
        {
            i = m_maintainBuffer.erase(
                i); // Erase the same maintain buffer entry for the promisc received packet
            return true;
        }
    }
    return false;
}

/// IsExpired structure
struct IsExpired
{
    /**
     * @brief comparison operator
     * @param e maintain buffer entry
     * @return true if the entry is expired
     */
    bool operator()(const DsrMaintainBuffEntry& e) const
    {
        // NS_LOG_DEBUG("Expire time for packet in req queue: "<<e.GetExpireTime ());
        return (e.GetExpireTime().IsStrictlyNegative());
    }
};

void
DsrMaintainBuffer::Purge()
{
    NS_LOG_DEBUG("Purging Maintenance Buffer");
    IsExpired pred;
    m_maintainBuffer.erase(std::remove_if(m_maintainBuffer.begin(), m_maintainBuffer.end(), pred),
                           m_maintainBuffer.end());
}

} // namespace dsr
} // namespace ns3

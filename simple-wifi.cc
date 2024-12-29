#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/wifi-module.h"
#include "ns3/ssid.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SimpleWifiExample");

int main (int argc, char *argv[])
{
  // Create nodes
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (3); // Create 3 nodes

  NodeContainer wifiApNode;
  wifiApNode.Create (1); // Create 1 Access Point node

  // Set up mobility (optional: for mobility of nodes in space)
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", StringValue ("0.0"),
                                 "MinY", StringValue ("0.0"),
                                 "DeltaX", StringValue ("5.0"),
                                 "DeltaY", StringValue ("10.0"),
                                 "GridWidth", StringValue ("3"),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiStaNodes);
  mobility.Install (wifiApNode);

  // Set up Wi-Fi
  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  WifiMacHelper mac;
  Ssid ssid = Ssid ("ns3-simple-wifi");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "Active", BooleanValue (true));

  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiStaNodes);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevice;
  apDevice = wifi.Install (phy, mac, wifiApNode);

  // Install Internet stack (IP addressing)
  InternetStackHelper stack;
  stack.Install (wifiStaNodes);
  stack.Install (wifiApNode);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer staInterfaces;
  staInterfaces = address.Assign (staDevices);
  Ipv4InterfaceContainer apInterfaces;
  apInterfaces = address.Assign (apDevice);

  // Set up traffic (e.g., On/Off application between nodes)
  OnOffHelper onOff ("ns3::TcpSocketFactory", InetSocketAddress (apInterfaces.GetAddress (0), 9));
  onOff.SetConstantRate (DataRate ("500kb/s"));
  ApplicationContainer apps = onOff.Install (wifiStaNodes.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  // Enable flow monitor to measure throughput, delay, and packet loss
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();

  // Run the simulation
  Simulator::Run ();

  // Print out statistics
  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();
  for (auto &stat : stats)
    {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (stat.first);
      std::cout << "Flow " << t.sourceAddress << " -> " << t.destinationAddress
                << ", Throughput: " << stat.second.txBytes / 1e3 << " kbps"
                << ", Delay: " << stat.second.delaySum.GetSeconds () / stat.second.rxPackets << " s"
                << ", Lost packets: " << stat.second.lostPackets << std::endl;
    }

  Simulator::Destroy ();
  return 0;
}


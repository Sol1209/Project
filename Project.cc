#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"


using namespace std;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("NetworkProject");

int 
main (int argc, char *argv[])
{

  bool verbose = true;
  uint32_t nCsma_0 = 6;
  uint32_t nCsma_1 = 6;
  uint32_t nCsma_2 = 6;
  uint32_t nCsma_3 = 6;
  uint32_t clientEcho_0 = 5, clientEcho_1 = 5, clientEcho_2 = 5, clientUDP_3 = 4;
  uint32_t serverEcho_0 = 4, serverEcho_1 = 4, serverEcho_2 = 4, serverUDP_3 = 3;
  int unsigned routers = 7;

  if (verbose)
      {
	    //LogComponentEnable ("NetworkProject", LOG_LEVEL_INFO);
        LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
        LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
        LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);
        LogComponentEnable ("OnOffApplication", LOG_LEVEL_INFO);
        LogComponentEnable ("PacketSink", LOG_LEVEL_INFO);
      }

  NS_LOG_INFO ("Creating Nodes");
  NodeContainer p2pNodes;
  p2pNodes.Create (routers);

  NodeContainer n0n1 = NodeContainer (p2pNodes.Get (0), p2pNodes.Get (1));
  NodeContainer n1n2 = NodeContainer (p2pNodes.Get (1), p2pNodes.Get (2));
  NodeContainer n1n3 = NodeContainer (p2pNodes.Get (1), p2pNodes.Get (3));
  NodeContainer n3n4 = NodeContainer (p2pNodes.Get (3), p2pNodes.Get (4));
  NodeContainer n6n5 = NodeContainer (p2pNodes.Get (6), p2pNodes.Get (5));
  NodeContainer n1n5 = NodeContainer (p2pNodes.Get (1), p2pNodes.Get (5));
  NodeContainer n3n2 = NodeContainer (p2pNodes.Get (3), p2pNodes.Get (2));
  NodeContainer n135n = NodeContainer (p2pNodes.Get (1),p2pNodes.Get(3), p2pNodes.Get (5));


  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  pointToPoint.SetDeviceAttribute ("Mtu", UintegerValue (1400));

  PointToPointHelper p2pSecondary;
  p2pSecondary.SetDeviceAttribute ("DataRate", StringValue ("2Mbps"));
  p2pSecondary.SetChannelAttribute ("Delay", StringValue ("3ms"));
  pointToPoint.SetDeviceAttribute ("Mtu", UintegerValue (1400));



  NetDeviceContainer n01n, n12n, n13n, n34n, n65n, n15n, n32n;
  n01n = pointToPoint.Install (n0n1);
  n12n = pointToPoint.Install (n1n2);
  n13n = pointToPoint.Install (n1n3);
  n34n = pointToPoint.Install (n3n4);
  n65n = pointToPoint.Install (n6n5);
  n15n = pointToPoint.Install (n1n5);
  n32n = p2pSecondary.Install (n3n2);


  NS_LOG_INFO ("Creating LANs");


  NodeContainer csmaNodes_0;
  csmaNodes_0.Create (nCsma_0);
  csmaNodes_0.Add (p2pNodes.Get (0));

  NodeContainer csmaNodes_1;
  csmaNodes_1.Add (p2pNodes.Get (2));
  csmaNodes_1.Create (nCsma_1);

  NodeContainer csmaNodes_2;
  csmaNodes_2.Add (p2pNodes.Get (4));
  csmaNodes_2.Create (nCsma_2);

  NodeContainer csmaNodes_3;
  csmaNodes_3.Create (nCsma_3);
  csmaNodes_3.Add (p2pNodes.Get (6));

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));
  csma.SetDeviceAttribute ("Mtu", UintegerValue (1400));

  NetDeviceContainer csmaDevices_0, csmaDevices_1, csmaDevices_2, csmaDevices_3, n53n;
  csmaDevices_0 = csma.Install (csmaNodes_0);
  csmaDevices_1 = csma.Install (csmaNodes_1);
  csmaDevices_2 = csma.Install (csmaNodes_2);
  csmaDevices_3 = csma.Install (csmaNodes_3);


  InternetStackHelper stack;
  stack.Install (csmaNodes_0);
  stack.Install (csmaNodes_1);
  stack.Install (csmaNodes_2);
  stack.Install (csmaNodes_3);
  stack.Install (n135n);

  NS_LOG_INFO ("Assigning IP address ");
  Ipv4AddressHelper address;

  address.SetBase ("192.168.2.0", "255.255.255.224");
  Ipv4InterfaceContainer d0d1;
  d0d1 = address.Assign (n01n);

  address.SetBase ("192.168.2.32", "255.255.255.224");
  Ipv4InterfaceContainer d1d2;
  d1d2 = address.Assign (n12n);

  address.SetBase ("192.168.2.64", "255.255.255.224");
  Ipv4InterfaceContainer d1d3;
  d1d3 = address.Assign (n13n);

  address.SetBase ("192.168.2.96", "255.255.255.224");
  Ipv4InterfaceContainer d3d4;
  d3d4 = address.Assign (n34n);

  address.SetBase ("192.168.2.128", "255.255.255.224");
  Ipv4InterfaceContainer d1d5;
  d1d5 = address.Assign (n15n);

  address.SetBase ("192.168.2.160", "255.255.255.224");
  Ipv4InterfaceContainer d6d5;
  d6d5 = address.Assign (n65n);


  address.SetBase ("192.168.2.224", "255.255.255.224");
  Ipv4InterfaceContainer d3d2;
  d3d2 = address.Assign (n32n);

  address.SetBase ("192.168.1.0", "255.255.255.224");
  Ipv4InterfaceContainer csmaInterfaces_0;
  csmaInterfaces_0 = address.Assign (csmaDevices_0);

  address.SetBase ("192.168.1.32", "255.255.255.224");
  Ipv4InterfaceContainer csmaInterfaces_1;
  csmaInterfaces_1 = address.Assign (csmaDevices_1);

  address.SetBase ("192.168.1.64", "255.255.255.224");
  Ipv4InterfaceContainer csmaInterfaces_2;
  csmaInterfaces_2 = address.Assign (csmaDevices_2);

  address.SetBase ("192.168.1.96", "255.255.255.224");
  Ipv4InterfaceContainer csmaInterfaces_3;
  csmaInterfaces_3 = address.Assign (csmaDevices_3);

  NS_LOG_INFO ("Creating Echo Server");

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps_0 ;
  serverApps_0 = echoServer.Install (csmaNodes_0.Get (serverEcho_0));
  serverApps_0.Start (Seconds (1.0));
  serverApps_0.Stop (Seconds (3.0));

  ApplicationContainer serverApps_1 ;
  serverApps_1 = echoServer.Install (csmaNodes_2.Get (serverEcho_1));
  serverApps_1.Start (Seconds (4.0));
  serverApps_1.Stop (Seconds (6.0));

  ApplicationContainer serverApps_2 ;
  serverApps_2 = echoServer.Install (csmaNodes_3.Get (serverEcho_2));
  serverApps_2.Start (Seconds (6.0));
  serverApps_2.Stop (Seconds (10.0));

  NS_LOG_INFO ("Creating Echo Client");
  UdpEchoClientHelper echoClient (csmaInterfaces_0.GetAddress (serverEcho_0), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  UdpEchoClientHelper echoClient_1 (csmaInterfaces_2.GetAddress (serverEcho_1), 9);
  echoClient_1.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient_1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient_1.SetAttribute ("PacketSize", UintegerValue (1024));

  UdpEchoClientHelper echoClient_2 (csmaInterfaces_3.GetAddress (serverEcho_2), 9);
  echoClient_2.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient_2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient_2.SetAttribute ("PacketSize", UintegerValue (1024));


  ApplicationContainer clientApps = echoClient.Install (csmaNodes_1.Get (clientEcho_0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (3.0));

  ApplicationContainer clientApps_1 = echoClient_1.Install (csmaNodes_0.Get (clientEcho_1));
  clientApps_1.Start (Seconds (4.5));
  clientApps_1.Stop (Seconds (6.5));

  ApplicationContainer clientApps_2 = echoClient_2.Install (csmaNodes_2.Get (clientEcho_2));
  clientApps_2.Start (Seconds (6.5));
  clientApps_2.Stop (Seconds (7.0));

  NS_LOG_INFO ("Creating UDP server");
  uint16_t port = 4000;

  UdpServerHelper server (port);

  ApplicationContainer apps = server.Install (csmaNodes_2.Get (serverUDP_3));
  apps.Start (Seconds (7.0));
  apps.Stop (Seconds (10.0));

  NS_LOG_INFO ("Creating UDP client");
  uint32_t MaxPacketSize = 1024;
  Time interPacketInterval = Seconds (0.05);
  uint32_t maxPacketCount = 320;
  UdpClientHelper client (csmaInterfaces_2.GetAddress (serverUDP_3), port);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (MaxPacketSize));
  apps = client.Install (csmaNodes_0.Get (clientUDP_3));
  apps.Start (Seconds (8.0));
  apps.Stop (Seconds (10.0));


  uint16_t port_1 = 8;   // Discard port (RFC 863)
  OnOffHelper onoff ("ns3::UdpSocketFactory",
                       InetSocketAddress (csmaInterfaces_2.GetAddress (1), port_1));
  onoff.SetConstantRate (DataRate ("2kbps"));
  onoff.SetAttribute ("PacketSize", UintegerValue (50));

  ApplicationContainer appls = onoff.Install (csmaNodes_1.Get (1));
  appls.Start (Seconds (11.0));
  appls.Stop (Seconds (20.0));

  PacketSinkHelper sink ("ns3::UdpSocketFactory",
                         Address (InetSocketAddress (Ipv4Address::GetAny (), port_1)));
  appls = sink.Install (csmaNodes_2.Get (1));
  appls.Start (Seconds (11.0));
  appls.Stop (Seconds (20.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();


  // Trace routing tables
  Ipv4GlobalRoutingHelper g;
  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("Project-Routing-Tables.routes", std::ios::out);
  g.PrintRoutingTableAllAt (Seconds (12), routingStream);
  //Simulator::Stop (Seconds (12.0));

  AnimationInterface anim ("Project.xml");

  anim.SetConstantPosition (csmaNodes_0.Get(0), 1.0, 10.0);
  anim.SetConstantPosition (csmaNodes_0.Get(1), 2.0, 10.0);
  anim.SetConstantPosition (csmaNodes_0.Get(2), 3.0, 10.0);
  anim.SetConstantPosition (csmaNodes_0.Get(3), 4.0, 10.0);
  anim.SetConstantPosition (csmaNodes_0.Get(4), 5.0, 10.0);
  anim.SetConstantPosition (csmaNodes_0.Get(5), 6.0, 10.0);
  anim.SetConstantPosition (csmaNodes_0.Get(6), 7.0, 10.0);

  anim.SetConstantPosition (csmaNodes_1.Get(0), 11.0, 10.0);
  anim.SetConstantPosition (csmaNodes_1.Get(1), 12.0, 10.0);
  anim.SetConstantPosition (csmaNodes_1.Get(2), 13.0, 10.0);
  anim.SetConstantPosition (csmaNodes_1.Get(3), 14.0, 10.0);
  anim.SetConstantPosition (csmaNodes_1.Get(4), 15.0, 10.0);
  anim.SetConstantPosition (csmaNodes_1.Get(5), 16.0, 10.0);
  anim.SetConstantPosition (csmaNodes_1.Get(6), 17.0, 10.0);

  anim.SetConstantPosition (csmaNodes_3.Get(0), 1.0, 3.0);
  anim.SetConstantPosition (csmaNodes_3.Get(1), 2.0, 3.0);
  anim.SetConstantPosition (csmaNodes_3.Get(2), 3.0, 3.0);
  anim.SetConstantPosition (csmaNodes_3.Get(3), 4.0, 3.0);
  anim.SetConstantPosition (csmaNodes_3.Get(4), 5.0, 3.0);
  anim.SetConstantPosition (csmaNodes_3.Get(5), 6.0, 3.0);
  anim.SetConstantPosition (csmaNodes_3.Get(6), 7.0, 3.0);

  anim.SetConstantPosition (csmaNodes_2.Get(0), 11.0, 3.0);
  anim.SetConstantPosition (csmaNodes_2.Get(1), 12.0, 3.0);
  anim.SetConstantPosition (csmaNodes_2.Get(2), 13.0, 3.0);
  anim.SetConstantPosition (csmaNodes_2.Get(3), 14.0, 3.0);
  anim.SetConstantPosition (csmaNodes_2.Get(4), 15.0, 3.0);
  anim.SetConstantPosition (csmaNodes_2.Get(5), 16.0, 3.0);
  anim.SetConstantPosition (csmaNodes_2.Get(6), 17.0, 3.0);

  anim.SetConstantPosition (p2pNodes.Get(1), 9.0, 7.0);
  anim.SetConstantPosition (p2pNodes.Get(5), 8.0, 5.0);
  anim.SetConstantPosition (p2pNodes.Get(3), 10.0, 5.0);

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
  return 0;
}

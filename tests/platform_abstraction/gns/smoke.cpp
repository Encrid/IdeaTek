/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>

#include <steam/isteamnetworkingutils.h>
#include <steam/steamnetworkingsockets.h>

static HSteamNetConnection server_side = k_HSteamNetConnection_Invalid;
static bool connected;

static void on_status(SteamNetConnectionStatusChangedCallback_t *info)
{
    bool listened = info->m_info.m_hListenSocket != k_HSteamListenSocket_Invalid;
    ESteamNetworkingConnectionState state = info->m_info.m_eState;

    if (listened && state == k_ESteamNetworkingConnectionState_Connecting) {
        SteamNetworkingSockets()->AcceptConnection(info->m_hConn);
        server_side = info->m_hConn;
        std::printf("accepted\n");
    }
    if (!listened && state == k_ESteamNetworkingConnectionState_Connected) {
        connected = true;
        std::printf("connected\n");
    }
    if (state == k_ESteamNetworkingConnectionState_ProblemDetectedLocally
        || state == k_ESteamNetworkingConnectionState_ClosedByPeer)
        std::printf("%s ended: %d %s\n", listened ? "server" : "client",
            info->m_info.m_eEndReason, info->m_info.m_szEndDebug);
}

static bool receive(ISteamNetworkingSockets *sockets)
{
    SteamNetworkingMessage_t *message = nullptr;

    if (server_side == k_HSteamNetConnection_Invalid
        || sockets->ReceiveMessagesOnConnection(server_side, &message, 1) != 1)
        return (false);
    std::printf("received %.*s\n", message->m_cbSize,
        static_cast<const char *>(message->m_pData));
    message->Release();
    return (true);
}

int main(int argc, char **argv)
{
    SteamNetworkingErrMsg error;
    SteamNetworkingIPAddr address;
    ISteamNetworkingSockets *sockets;
    HSteamListenSocket listen;
    HSteamNetConnection client;
    const char *text = argc > 2 ? argv[2] : "hello";
    bool sent = false;
    bool received = false;

    if (!GameNetworkingSockets_Init(nullptr, error)) {
        std::printf("no init: %s\n", error);
        return (1);
    }
    sockets = SteamNetworkingSockets();
    SteamNetworkingUtils()->SetGlobalCallback_SteamNetConnectionStatusChanged(
        on_status);
    address.Clear();
    address.SetIPv6LocalHost(argc > 1 ? static_cast<uint16>(std::atoi(argv[1]))
        : 27020);
    listen = sockets->CreateListenSocketIP(address, 0, nullptr);
    client = sockets->ConnectByIPAddress(address, 0, nullptr);
    for (int i = 0; i < 500 && !received; i++) {
        sockets->RunCallbacks();
        if (connected && !sent) {
            sockets->SendMessageToConnection(client, text,
                static_cast<uint32>(std::strlen(text)),
                k_nSteamNetworkingSend_Reliable, nullptr);
            sent = true;
        }
        received = receive(sockets);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    sockets->CloseConnection(client, 0, nullptr, false);
    sockets->CloseListenSocket(listen);
    GameNetworkingSockets_Kill();
    std::printf("%s\n", received ? "done" : "no message");
    return (received ? 0 : 1);
}

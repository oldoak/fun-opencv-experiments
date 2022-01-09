#include "WebSocketServerMain.h"

void WebSocketRequestHandler::handleRequest
(HTTPServerRequest& request, HTTPServerResponse& response)
{
    Application& app = Application::instance();
    try
    {
        WebSocket ws(request, response);
        app.logger().information("WebSocket connection established.");
        char inBuffer[1024];
        int flags;
        int n;
        ws.setReceiveTimeout(1000000);
        do
        {
            try
            {
                n = ws.receiveFrame(inBuffer, sizeof(inBuffer), flags);
                app.logger().information(Poco::format
                        ("Frame received (length=%d, flags=0x%x).", 
                         n, 
                         unsigned(flags)));
                std::cout << "Received from client: " << inBuffer << std::endl;
            }
            catch(Poco::TimeoutException&)
            {
                Timestamp now;
                std::string message = DNS::hostName()
                    + "@"
                    + DateTimeFormatter::format(now, 
                            DateTimeFormat::ISO8601_FORMAT);
                const char *serverMsg = (char *) message.c_str();
                ws.sendFrame(serverMsg, std::strlen(serverMsg), flags);
            }
        } while (n > 0 || 
                (flags & WebSocket::FRAME_OP_BITMASK) 
                != WebSocket::FRAME_OP_CLOSE);

        app.logger().information("WebSocket connection closed.");
     }
     catch (WebSocketException& exc)
     {
         app.logger().log(exc);
         switch (exc.code())
         {
             case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                 response.set("Sec-WebSocket-Version", 
                              WebSocket::WEBSOCKET_VERSION);
                 // fallthrough
             case WebSocket::WS_ERR_NO_HANDSHAKE:
             case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
             case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
                 response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
                 response.setContentLength(0);
                 response.send();
                 break;
         }
     }
}

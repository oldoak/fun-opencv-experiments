#include "WebSocketServerMain.h"

void PageRequestHandler::handleRequest
(HTTPServerRequest& request, HTTPServerResponse& response)
{
    response.setChunkedTransferEncoding(true);
    response.setContentType("text/html");
    Timestamp now;
    std::string s = "Local WebSocket server - date/time is: " + 
        DateTimeFormatter::format(now, DateTimeFormat::ISO8601_FORMAT);
    const char *message = (char *) s.c_str();
    response.sendBuffer(message, std::strlen(message));
}

#ifndef WEBSOCKETREQUESTHANDLER_H
#define WEBSOCKETREQUESTHANDLER_H

#include "WebSocketServerMain.h"

class WebSocketRequestHandler: public HTTPRequestHandler
	/// Handle a WebSocket connection.
{
    public:
        void handleRequest
            (HTTPServerRequest& request, HTTPServerResponse& response);
};

#endif

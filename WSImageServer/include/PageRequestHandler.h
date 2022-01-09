#ifndef PAGEREQUESTHANDLER_H
#define PAGEREQUESTHANDLER_H

#include "WebSocketServerMain.h"

class PageRequestHandler: public HTTPRequestHandler
{
    public:
        void handleRequest
            (HTTPServerRequest& request, HTTPServerResponse& response);
};

#endif

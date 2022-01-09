#ifndef REQUESTHANDLERFACTORY_H
#define REQUESTHANDLERFACTORY_H

#include "WebSocketServerMain.h"

class RequestHandlerFactory: public HTTPRequestHandlerFactory
{
    public:
        HTTPRequestHandler* createRequestHandler
            (const HTTPServerRequest& request);
};

#endif

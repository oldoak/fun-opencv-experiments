#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include "WebSocketServerMain.h"

class WebSocketServer: public Poco::Util::ServerApplication
{
    public:
        WebSocketServer(): _helpRequested(false)
        {
        }
    
        ~WebSocketServer()
        {
        }

    protected:
        void initialize(Application& self);
        
        void uninitialize();

        void defineOptions(OptionSet& options);

        void handleOption(const std::string& name, const std::string& value);

        void displayHelp();

        int main(const std::vector<std::string>& args);

    private:
        bool _helpRequested;
};

#endif

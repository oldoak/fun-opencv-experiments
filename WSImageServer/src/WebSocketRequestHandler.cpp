#include "WebSocketServerMain.h"

void WebSocketRequestHandler::handleRequest
(HTTPServerRequest& request, HTTPServerResponse& response)
{
    Application& app = Application::instance();
    try
    {
        WebSocket ws(request, response);
        app.logger().information("WebSocket connection established.");
        char *inBuffer = new char[200000];
        int flags;
        int imageBytes;
        do
        {
            //imageBytes = ws.receiveFrame(inBuffer, sizeof(inBuffer), flags);
            imageBytes = ws.receiveFrame(inBuffer, 200000, flags);
            app.logger().information(Poco::format
                        ("Frame received (length=%d, flags=0x%x).", 
                         imageBytes, 
                         unsigned(flags)));

            stringstream ss;
            ss << inBuffer;
            Poco::Base64Decoder b64in(ss);

            ostringstream ostr;
            Poco::StreamCopier::copyStream(b64in, ostr);
            //vector<unsigned char> 
            //    imgBin(ostr.str().begin(), ostr.str().end());

                cout << ostr.str() << endl;
            /*Mat receivedImage = imdecode(imgBin, IMREAD_UNCHANGED);
            if(receivedImage.data == NULL)
            {
                cout << "Image content is empty!" << endl;
            }
            else
            {
                cout << "******" << endl;
            }*/

            //const char *returnImage = imageBin.str().c_str();

            //flags = Poco::Net::WebSocket::FRAME_FLAG_FIN | 
            //    Poco::Net::WebSocket::FRAME_OP_BINARY;
            //vector<unsigned char> returnImage;
            //vector<int> writeFlags;
            //writeFlags.push_back(IMWRITE_JPEG_QUALITY);
            //imencode(".jpg", receivedImage, returnImage, writeFlags);
            //Size inputImgSize {receivedImage.size()};
            //Mat returnImage = receivedImage;

            //ws.sendFrame(returnImage, sizeof(returnImage), flags);
        } while (imageBytes > 0 || (flags & WebSocket::FRAME_OP_BITMASK) 
                != WebSocket::FRAME_OP_CLOSE);

        delete [] inBuffer;
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

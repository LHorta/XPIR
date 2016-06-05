#include <iostream>
#include <cpr/cpr.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "fcm.hpp"

using namespace std;
using namespace cpr;

string USER_TOKEN = "dhWOQQEwOn8:APA91bHcs6nXmYZxkXaNzx7Hvh54y6_9XYSyBZ0Ec7SoxEUhWPnltPBsG4Q26xp1qjMe2ntiPA5pLAXcVzvaWSpDNrStRwN2JZalgoZ3CUs5ExB4GQqHnlUx21aJfOnBVqkpvPz6lzn1";
string AUTH_KEY = "AIzaSyBCe3oMcRevwnmBLb86xy_pSih6-_4bXSg";
unsigned short int PORT = 4444;

bool validateFingerprint() {
    using boost::asio::ip::tcp;
    bool result = false;
    string body =
            "{\"notification\": {\"title\": \"Authentication required\", \"icon\": \"ic_notification\", \"click_action\": \"OPEN_ACTIVITY_VERIFY\"}, \"to\": \"" +
            USER_TOKEN + "\"}";
    Response response = Post(Url{"https://fcm.googleapis.com/fcm/send"},
                             Header{{"Content-Type",  "application/json"},
                                    {"Authorization", "key=" + AUTH_KEY}},
                             Body{body});

    try {
        // Any program that uses asio need to have at least one io_service object
        boost::asio::io_service io_service;

        // acceptor object needs to be created to listen for new connections
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), PORT));

        // creates a socket
        tcp::socket socket(io_service);

        // wait and listen
        acceptor.accept(socket);

        boost::array<char, 128> buf;
        boost::system::error_code error;

        // The boost::asio::buffer() function automatically determines
        // the size of the array to help prevent buffer overruns.
        size_t len = socket.read_some(boost::asio::buffer(buf), error);

        // When the server closes the connection,
        // the ip::tcp::socket::read_some() function will exit with the boost::asio::error::eof error,
        // which is how we know to exit the loop.
        if (error == boost::asio::error::eof)
            cout << "connection closed by peer" << endl; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.

        string stuff = string(buf.data(), len);
        cout << stuff << endl;
        result = stuff.compare("true") == 0;
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return result;
}

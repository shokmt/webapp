//
// test for chat app with websocket
//
//
#include <iostream>
#include <string>
#include <fstream>
#include <ws.hpp>
using namespace std;

class MyHandler: public ws::Handler {
   int x;
   unordered_map<ws::Socket*,int> map;
public:
   MyHandler(): x(0){}
   void connect(ws::Socket& socket)
   {
      socket.broadcast(to_string(x)+" entered");
      socket.send("Hello!");
      map[&socket] = x++;
   }

   void receive(ws::Socket& socket, std::string msg)
   {
      socket.broadcast(to_string(map[&socket])+ " said: "+ msg);
      socket.send("You said: " + msg);
   }

   void closed(ws::Socket& socket){ 
      socket.broadcast(to_string(map[&socket])+" quit");
      map.erase(&socket);
   }
};

int main(int argc, char *argv[])
{
   MyHandler wsh;

   http::createServer([](auto req, auto res) { // c++14
      string pathname = url::parse(req.url)["pathname"];
      if (req.method == "GET") {

         string path = pathname;
         if (pathname == "/chat")
            path = "/ws-chat.html";

         ifstream in("./public/" + path);
         if (!in) {
            res.writeHead(404,{{"Content-Type", "text/html"},
                               {"Connection", "close"} });
            res.end("<html><body><h1>404 Not Found</h1></body></html>");
         } else {
            res.writeHead(200,{{"Content-Type", "text/html"},
                               {"Transfer-Encoding", "chunked"},
                               // {"Connection", "close"}
                              });
            string line;
            while (getline(in, line))
               res.write(line+"\r\n");
            res.end();
         }

      } else {
         res.writeHead(405);
         res.end("<html><body><h1>405 Method Not Allowed</h1></body></html>");
      }
   }, wsh ).listen(3001);
}

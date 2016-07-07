//
// test for chat app with websocket
// webapp version
//
#include <webapp.hpp>

class MyHandler: public ws::Handler {
   int x;
   std::unordered_map<ws::Socket*,int> map;
public:
   MyHandler(): x(0){}
   void connect(ws::Socket& socket)
   {
      socket.broadcast("Guest"+std::to_string(x)+" entered");
      socket.send("Hello!" );
      map[&socket] = x++;
   }

   void receive(ws::Socket& socket, std::string msg)
   {
      socket.broadcast("Guest"+std::to_string(map[&socket])+ " said: "+ msg);
      socket.send("You said: " + msg);
   }

   void closed(ws::Socket& socket){ 
      socket.broadcast(std::to_string(map[&socket])+" quit");
      map.erase(&socket);
   }
};

int main(int argc, char *argv[])
{
   http::WebApp app;
   app.get("/chat", [](auto req, auto res) {
      res.sendFile("./public/ws-chat.html");
      return true;
   });

   MyHandler wsh;

   http::createServer(app, wsh).listen(3001);
   return 0;
}

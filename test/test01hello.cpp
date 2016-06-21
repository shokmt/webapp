//
// hello, world
//
// g++ -Wall -I./inc -std=c++14 thisfile.cpp
//
#include <websvr.hpp>
int main(int argc, char *argv[])
{
   http::createServer([](auto req, auto res){
      res.writeHead(200, {{"Content-Type","text/plain"},
                          {"Content-Length","14"}});
      res.end("hello, world\r\n");
   }).listen(3001);
}

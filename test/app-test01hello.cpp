//
// basic webapp
//
#include <webapp.hpp>
int main(int argc, char *argv[])
{
   http::WebApp app;
   app.get("/about", [](auto req, auto res) {
         res.send("hello, world");
         return true;
   });
   http::createServer(app).listen(3001);
}

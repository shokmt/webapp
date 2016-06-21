//
// route match 
// sendfile
//
#include "webapp.hpp"

int main(int argc, char *argv[])
{
   http::createServer(
   http::WebApp()
   .get("/about", [](auto req, auto res) {
         res.send("Hello, World");
         return true;
   })
   .get("/image", [](auto req, auto res) {
         res.sendFile("./public/nup.gif");
         return true;
   })
   .get("/:html", [](auto req, auto res) {
         std::string p = "./public/"+ req.params("html");
         std::cout << "get a static file: " << p << "\n";
         res.sendFile(p);
         return true;
   })
   .post("/:CGI", [](auto req, auto res) {
         std::string msg = req.params("CGI")+"<br>\r\n";
         for (auto &a: querystring::parse(req.body().str()))
            msg += a.first+": "+a.second+"<br>\r\n";
         res.send(msg);
         return true;
   })).listen(3001);
}

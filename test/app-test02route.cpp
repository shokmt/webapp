//
// route match 
// sendfile
//
#include <webapp.hpp>

int main(int argc, char *argv[])
{
   http::WebApp app;
   app.get("/about", [](auto req, auto res) {
         res.send("hello, world");
         return true;
   });
   app.get("/image", [](auto req, auto res) {
         res.sendFile("./public/nup.gif");
         return true;
   });
   app.get("/:html", [](auto req, auto res) {
         res.sendFile("./public/"+ req.params("html"));
         return true;
   });
   app.post("/:cgi", [](auto req, auto res) {
         std::string msg = req.params("cgi")+"<br>\r\n";
         for (auto &a: querystring::parse(req.body().str()))
            msg += a.first+": "+a.second+"<br>\r\n";
         res.send(msg);
         return true;
   });

   http::createServer(app).listen(3001);
}

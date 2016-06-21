//
// class based implementation
// avoid closure
// route match 
// sendfile
//
#include "webapp.hpp"
using namespace std;

class T {
   int x;
public:
   T():x(0){}
   bool about(http::AppRequest& req, http::AppResponse& res) {
         ++x;
         res.send("hello, world: <h1>"+to_string(x)+"</h1>");
         return true;
   }
   bool image(http::AppRequest& req, http::AppResponse& res) {
         ++x;
         res.sendFile("./public/nup.gif");
         return true;
   }
   bool sfile(http::AppRequest& req, http::AppResponse& res) {
         ++x;
         res.sendFile("./public/"+ req.params("html"));
         return true;
   }
   bool post(http::AppRequest& req, http::AppResponse& res) {
         ++x;
         string msg;
         for (auto &a: querystring::parse(req.body().str()))
            msg += a.first+": "+a.second+"<br>\r\n";
         res.send(msg);
         return true;
   }
};

int main(int argc, char *argv[])
{
   T x;
   http::WebApp app;
   app.get("/about",   &x, &T::about);
   app.get("/image",   &x, &T::image);
   app.get("/:html",   &x, &T::sfile);
   app.post("/post",   &x, &T::post);
   http::createServer(app).listen(3001);
}

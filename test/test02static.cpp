//
// serve a static file content with Content-Length
//
#include <string>
#include <fstream>
#include <websvr.hpp>

int main()
{
   http::createServer( [](auto req, auto res){
      int code;
      std::string body;
      if (req.method != "GET") {
         code = 405;
         body = "<html><body>405 Method Not Allowed</body></html>";
      } else {
         std::ifstream in("./public/" + req.url);
         if (!in) {
            code = 404;
            body = "<html><body>404 Not Found</body></html>";
         } else {
            code = 200;
            std::string line;
            while (getline(in, line))
               body += (line+"\r\n");
         }
      }

      res.writeHead(code,
          {{"Content-Type","text/html"},
           {"Content-Length", std::to_string(body.size())}});
      res.end(body);
   }).listen(3001);
}

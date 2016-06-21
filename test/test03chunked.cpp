//
// recv/send chunked data
//
#include <string>
#include <fstream>
#include <websvr.hpp>

int main(int argc, char *argv[])
{
   http::createServer([](auto req, auto res) {
      res.setHeader("Content-Type", "text/html");
      res.setHeader("Transfer-Encoding", "chunked");
      if (req.method == "GET") {
         std::ifstream in("./public/" + req.url);
         if (!in) {
            res.writeHead(404);
            res.end("<html><body>404 Not Found</body></html>");
         } else {
            res.writeHead(200);
            std::string line;
            while (getline(in, line)) 
               res.write(line+"\r\n");
            res.end();
         }

      } else if (req.method == "POST") {
         std::string body;
         req.on("data", [&](std::string chunk) {
            body += chunk;
         });
         req.on("end", [&]() {
            res.writeHead(200);
            res.end("<html><body>"+body+"</body></html>");
         });

      } else {
         res.writeHead(405);
         res.end("<html><body>405 Method Not Allowed</body></html>");
      }
   }).listen(3001);
}

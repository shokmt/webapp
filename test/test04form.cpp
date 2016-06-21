//
// form
//
#include <iostream>
#include <string>
#include <fstream>
#include <websvr.hpp>
using namespace std;

int main(int argc, char *argv[])
{
   http::createServer([](auto req, auto res) {
      if (req.method == "GET") {
         auto x = url::parse(req.url);
         auto y = querystring::parse(x["query"]);
         
         for (auto &a:y)
            cout << a.first << ": " << a.second << "\n";

         string path = "./public" + x["pathname"];
         cout << "open file \"" << path << "\"\n";

         ifstream in(path);
         if (!in) {
            res.writeHead(404,{{"Content-Type", "text/html"}});
            res.end("<html><body>404 Not Found</body></html>");
         } else {
            res.writeHead(200,{{"Content-Type", "text/html"},
                               {"Transfer-Encoding", "chunked"}});
            string line;
            while (getline(in, line))
               res.write(line+"\r\n");
            res.end();
         }

      } else if (req.method == "POST") {
         http::Buffer body;
         req.on("data", [&](http::Buffer chunk) {
            body += chunk;
         });
         req.on("end", [&]() {
            if (req.headers["Content-Type"] == 
               "application/x-www-form-urlencoded") {
               res.writeHead(200, {{"Content-Type", "text/html"}});
               res.write(R"aa(<html><head><meta charset="UTF-8">
                              <title>Posted data</title></head><body>)aa");
               for (auto &a: querystring::parse(body.str()))
                  res.write(a.first+": "+a.second+"<br>");
               res.end("</body></html>");
            } else {
               res.writeHead(200, {{"Content-Type", "text/plain"}});
               res.write(body);
               res.end();
            }
         });

      } else {
         res.writeHead(405);
         res.end("<html><body>405 Method Not Allowed</body></html>");
      }

   }).listen(3001);
}

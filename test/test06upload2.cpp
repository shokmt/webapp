//
// test for uploading file 2
//
//
#include <iostream>
#include <fstream>
#include <websvr.hpp>
using namespace std;

int main(int argc, char *argv[])
{
   http::createServer([](auto req, auto res) {
      string pathname = url::parse(req.url)["pathname"];
      if (req.method == "GET") {
         string path = "./public" + pathname;
         ifstream in(path);
         if (!in) {
            res.writeHead(404,{{"Content-Type", "text/html"}});
            res.end("<html><body><h1>404 Not Found</h1></body></html>");
         } else {
            res.writeHead(200,{{"Content-Type", "text/html"},
                               {"Transfer-Encoding", "chunked"}});
            string line;
            while (getline(in, line))
               res.write(line+"\r\n");
            res.end();
         }

      } else if (pathname == "/upload" && req.method == "POST") {

         http::Upload("./tmp").parse(req,
         [&](string err, auto fields, auto files) {
            res.writeHead(200, {{"Content-Type","text/plain; charset=utf-8"}});
            if (!err.empty()) {
               res.write(err+"\n");
               return;
            }
            for (auto& x:fields)
               res.write(x.first+": "+x.second+"\n");
            for (auto& x:files) {
               const auto& up = x.second;
               res.write(x.first+ ":\n\tsize = "+to_string(up.size)+"\n"
                        + "\tpath = "+up.path+"\n"
                        + "\tname = "+up.name+"\n"
                        + "\tmime = "+up.mimetype+"\n");
            }
            res.end();
         });

      } else {
         res.writeHead(405);
         res.end("<html><body>405 Method Not Allowed</body></html>");
      }

   }).listen(3001);
}

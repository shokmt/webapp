//
// test for uploading file
// public/upload.html
//
#include <iostream>
#include <string>
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
            res.end("<html><body>404 Not Found</body></html>");
         } else {
            res.writeHead(200,{{"Content-Type", "text/html"},
                               {"Transfer-Encoding", "chunked"}});
            string line;
            while (getline(in, line))
               res.write(line+"\r\n");
            res.end();
         }
      } else if (pathname == "/upload" && req.method == "POST") {

         res.writeHead(200, {{"Content-Type", "text/plain; charset=UTF-8"}});
         http::Upload("./tmp")
           .on("field", [&](string f, string v) {
            res.write(f + ":" + v+"\n");
         }).on("file", [&](string f, const http::UploadFile& up) {
            res.write(f + ":\n\tsize = "+to_string(up.size)+"\n"
                        + "\tpath = "+up.path+"\n"
                        + "\tname = "+up.name+"\n"
                        + "\tmime = "+up.mimetype+"\n");
            
         }).on("end", [&]() {
            res.write("end of upload");
            res.end();
         }).parse(req);

      } else {
         res.writeHead(405);
         res.end("<html><body>405 Method Not Allowed</body></html>");
      }

   }).listen(3001);
}

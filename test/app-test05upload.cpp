//
// upload app
//
#include <webapp.hpp>
using namespace std;

int main(int argc, char *argv[])
{
   http::WebApp app;
   app.get("/:a", [](auto req, auto res) {
         res.sendFile("./public/upload.html");
         return true;
   });
   app.post("/upload", [](auto req, auto res) {
         http::Upload("./tmp").parse(req,
         [&](string err, auto fields, auto files) {
            if (!err.empty()) {
               res.send(err);
               return;
            }
            for (auto& x:fields)
               cout << x.first <<": "<< x.second << "\n";
            for (auto& x:files) {
               const auto& up = x.second;
               cout << x.first << ":\n"
                    << "\tsize = "+to_string(up.size)+"\n"
                    << "\tpath = "+up.path+"\n"
                    << "\tname = "+up.name+"\n"
                    << "\tmime = "+up.mimetype+"\n";
            }
            res.send("uploaded correctly");
         });
         return true;
   });

   http::createServer(app).listen(3001);
}

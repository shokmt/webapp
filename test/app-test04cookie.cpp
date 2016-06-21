//
// cookie test
//
#include "webapp.hpp"
using namespace std;

class T {
   int x;
public:
   T():x(0){}
   bool root(http::AppRequest& req, http::AppResponse& res) {
         cout << (++x) << "\n";
         if (req.cookie("name") == "abc")
            res.send("hello, world: <h1>"+to_string(x)+"</h1>");
         else {
            res.setCookie(http::Cookie("name","abc").maxage(10));
            res.send("hello, world");
         }
         return true;
   }
};

int main(int argc, char *argv[])
{
   T x;
   http::createServer(
      http::WebApp().get("/", &x, &T::root)
   ).listen(3001);
}

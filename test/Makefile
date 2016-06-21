INC=../
CXX=g++
CXXFLAGS=-O -g -Wall -pedantic -std=c++14 -I$(INC)

a.out:  test01hello.cpp  $(INC)/websvr.hpp $(INC)/ws.hpp
	$(CXX) $(CXXFLAGS) $<

$(INC)/webapp.hpp: $(INC)/websvr.hpp $(INC)/ws.hpp

$(INC)/ws.hpp: $(INC)/websvr.hpp

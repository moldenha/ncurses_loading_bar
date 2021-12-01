#include "load.h"
#include <string.h>
#include <unistd.h>
#include <thread>

//g++ main.cpp -o ctest -std=c++11 -pthread -lncurses -Incurses

std::string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}
void testFunc(bar *b, int objects, int sleep, int randCharacters, std::string ending){
	std::string numStr = std::to_string(objects);
	int back = 4+(numStr.size()*2)+ending.size();
	b->updateObjects(objects);
	b->updateBack(back);
	for(int i = 0; i < objects; i++){
		std::string rand = "[+] "+gen_random(randCharacters)+"\n";
		b->write(rand);
		b->update(ending);
		usleep(sleep);
	}
}


int main(){
	load main(2, 30, true);
	std::thread t1 = std::thread(testFunc, main.getBar(0), 50, 200000, 10, "EDB1");
	std::thread t2 = std::thread(testFunc, main.getBar(1), 500, 10000, 16, "EDB2");
	t1.join();
	t2.join();	
	main.end();
	return 0;
}

#include "boost/thread.hpp"
#include "boost/filesystem/path.hpp"

#include <vector>

namespace boost
{

void assertion_failed(char const * expr, char const * function, char const * file, long line)
{
}

}

class Worker
{
public:
    Worker(boost::barrier& barrier) 
        : mBarrier(barrier)
    {
    }

    void operator()()
    {
        mBarrier.wait();
        boost::filesystem::path path("foo");
    }

    boost::barrier& mBarrier;
};

int main(int argc, char **argv)
{
    // uncomment this to workaround the global object initialization problem
    //boost::filesystem::path path("foo");

    int count = 10;
    boost::barrier barrier(count + 1);

    Worker w(barrier);

    std::vector<boost::shared_ptr<boost::thread> > vThread;
    for (int i = 0; i < count; ++i)
        vThread.push_back(boost::shared_ptr<boost::thread>(new boost::thread(w)));

    barrier.wait();

    for (int i = 0; i < count; ++i)
        vThread[i]->join();

    return 0;
}
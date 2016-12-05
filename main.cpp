#include <boost/process.hpp>
#include <iostream>
#include <regex>
#include <exception>

namespace bp = boost::process;

bp::child start_child(std::string exec)
{
    std::vector<std::string> args;
    args.push_back(exec);
    bp::context ctx;
    ctx.stdout_behavior = bp::capture_stream();
    return bp::launch(exec, args, ctx);
}

int main(int argc, char* argv[])
{
    if (argc < 2) throw std::invalid_argument("not enough parameters");

    bp::child child = start_child(argv[1]);
    bp::pistream &is = child.get_stdout();
    std::string line;
    std::regex str("\\S+\\s*");
    std::smatch match;
    unsigned int w_counter = 0;
    while (std::getline(is, line))
    {
        while (std::regex_search(line, match, str))
        {
            w_counter += match.size();
            line = match.suffix();
        }
    }
    std::cout << "W_counter: " << w_counter << std::endl;
    bp::status s = child.wait();
    return s.exited() ? s.exit_status() : EXIT_FAILURE;
}

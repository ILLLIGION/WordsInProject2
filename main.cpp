#include <boost/process.hpp>
#include <iostream>
#include <regex>

namespace bp = boost::process;

bp::child start_child(std::string exec)
{
    std::vector<std::string> args;
    args.push_back(exec);
    bp::context ctx;
    ctx.stdout_behavior = bp::capture_stream();
    ctx.stdin_behavior = bp::capture_stream();
    return bp::launch(exec, args, ctx);
}

int main(int argc, char* argv[])
{
    if (argc < 2) return 0;

    bp::child child = start_child(argv[1]);
    bp::pistream &is = child.get_stdout();
    bp::postream &os = child.get_stdin();
    std::string line;
    std::regex str("\\S+\\s*");
    std::smatch match;
    int w_counter = 0;
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

#include <fcntl.h>
#include <gflags/gflags.h>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include "../ron/ron.hpp"

using namespace std;
using namespace ron;

struct MPS {
    bool operator()(char ch) const { return ch == '\\' || ch == '/'; }
};
string basename(string const& pathname) {
    return string(find_if(pathname.rbegin(), pathname.rend(), MPS()).base(),
                  pathname.end());
}
static int rmFiles(const char* pathname, const struct stat* sbuf, int type,
                   struct FTW* ftwb) {
    if (remove(pathname) < 0) {
        perror("ERROR: remove");
        return -1;
    }
    return 0;
}
#include <ftw.h>

Status rm_dir(string path) {
    if (nftw(path.c_str(), rmFiles, 10, FTW_DEPTH | FTW_MOUNT | FTW_PHYS) < 0) {
        return Status::IOFAIL.comment("can't rmdir");
    }
    return Status::OK;
}

bool file_exists(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

Status cdtmp() {
    const char* tmp = std::tmpnam(nullptr);
    mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    chdir(tmp);
    return Status::OK;  // yeah
}

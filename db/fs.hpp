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

Status rm_dir(const char* path) {
    if (nftw(path, rmFiles, 10, FTW_DEPTH | FTW_MOUNT | FTW_PHYS) < 0) {
        return Status::IOFAIL.comment(strerror(errno));
    }
    return Status::OK;
}

bool file_exists(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

class TmpDir {
    String cwd_;
    String tmp_;
    static constexpr size_t MAX_PATH_LEN{1024};

   public:
    TmpDir() = default;

    Status cd(String name) {
        char dir[MAX_PATH_LEN];
        getcwd(dir, MAX_PATH_LEN);
        cwd_ = String{dir};
        sprintf(dir, "/tmp/%s.XXXXXX", name.c_str());
        if (!mkdtemp(dir)) {
            return Status::IOFAIL.comment(strerror(errno));
        }
        tmp_ = String{dir};
        if (chdir(dir)) {
            return Status::IOFAIL.comment(strerror(errno));
        }
        return Status::OK;
    }

    Status back() {
        if (!cwd_.empty()) {
            chdir(cwd_.c_str());
        }
        if (!tmp_.empty()) {
            rm_dir(tmp_.c_str());
        }
        return Status::OK;
    }

    ~TmpDir() { back(); }
};

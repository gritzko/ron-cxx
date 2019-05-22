#ifndef RON_FS_HPP
#define RON_FS_HPP
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include "../ron/ron.hpp"

using namespace std;

namespace ron {

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

#define RET_IOFAIL \
    { return Status::IOFAIL.comment(strerror(errno)); }

class TmpDir {
    String cwd_;
    String tmp_;
    static constexpr size_t MAX_PATH_LEN{1024};

   public:
    TmpDir() = default;

    Status cd(const String& name) {
        char dir[MAX_PATH_LEN];
        if (nullptr == getcwd(dir, MAX_PATH_LEN)) {
            RET_IOFAIL;
        }
        cwd_ = String{dir};
        sprintf(dir, "/tmp/%s.XXXXXX", name.c_str());
        if (!mkdtemp(dir)) {
            RET_IOFAIL;
        }
        tmp_ = String{dir};
        if (chdir(dir)) {
            RET_IOFAIL;
        }
        return Status::OK;
    }

    Status back() {
        if (!cwd_.empty()) {
            if (0 < chdir(cwd_.c_str())) {
                RET_IOFAIL;
            }
        }
        if (!tmp_.empty()) {
            rm_dir(tmp_.c_str());
        }
        return Status::OK;
    }

    ~TmpDir() { back(); }
};

}  // namespace ron

#endif
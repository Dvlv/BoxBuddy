#include "distrobox.h"
#include <algorithm>
#include <array>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

std::string runCmdAndGetOutput(const char *cmd) {
    // https://stackoverflow.com/questions/52164723/how-to-execute-a-command-and-get-return-code-stdout-and-stderr-of-command-in-c
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
            result += buffer.data();
        }
    }

    return result;
}

std::vector<std::string> explodeString(const std::string &str, const char &ch) {
    // splits string by provided char
    // https://stackoverflow.com/questions/13172158/c-split-string-by-line
    auto result = std::vector<std::string>{};
    auto ss = std::stringstream{str};

    for (std::string line; std::getline(ss, line, ch);)
        result.push_back(line);

    return result;
}

static inline void ltrim(std::string &s) {
    // https://stackoverflow.com/questions/216823/how-to-trim-an-stdstring
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
}

static inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

std::vector<Distrobox::DBox> Distrobox::getAllBoxes() {
    std::vector<Distrobox::DBox> boxes;

    std::string dbListOut = runCmdAndGetOutput("distrobox list");

    std::vector<std::string> dbListOutLines = explodeString(dbListOut, '\n');

    for (int i = 1; i < dbListOutLines.size() - 1; ++i) {
        std::vector<std::string> fields = explodeString(dbListOutLines[i], '|');

        for (auto &field : fields) {
            trim(field);
        }
        Distrobox::DBox box = {.name = fields[1],
                               .distro = fields[3],
                               .ID = fields[0],
                               .status = fields[2]};

        boxes.push_back(box);
    }

    return boxes;
}

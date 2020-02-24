#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <unordered_set>

size_t get_weight(const std::string& str) {
    size_t weight = 0;

    for (auto& ch : str) {
        if (ch == '1') {
            ++weight;
        }
    }

    return weight;
}

char is_equal(const std::string& miniterm, const std::string& dnf) {
    char symbol = '*';

    for (size_t i = 0; i < miniterm.size(); ++i) {
        if (miniterm[i] != '~' && miniterm[i] != dnf[i]) {
            symbol = ' ';
            break;
        }
    }

    return symbol;
}

void write_table(size_t n, const std::unordered_set<std::string>& neccesary_miniterms, const std::vector<std::string>& dnf) {
    std::cout << std::endl << std::setw((n + 1)*(dnf.size() + 1)) << std::setfill('-') << ' ' << std::endl << std::setfill(' ');

    for (auto i = 0; i < dnf.size() + 1; i++) {
        if (i == 0) {
            std::cout << std::setw(n) << ' ' << '|';
        }
        else {
            std::cout << std::setw(n) << dnf[i - 1] << '|';
        }
    }

    std::cout << std::endl << std::setw((n + 1)*(dnf.size() + 1)) << std::setfill('-') << ' ' << std::endl << std::setfill(' ');

    for (auto& el : neccesary_miniterms) {
        std::cout << std::setw(n) << el << '|';
        for (auto j = 0; j < dnf.size(); j++) {
            std::cout << std::setw(n) << is_equal(el, dnf[j]) << '|';
        }
        std::cout << std::endl << std::setw((n + 1)*(dnf.size() + 1)) << std::setfill('-') << ' ' << std::endl << std::setfill(' ');
    }
}

int main() {
    size_t n = 0;

    std::cout << "n = ";
    std::cin >> n;

    std::ifstream fin{};
    std::string filename;

    std::cout << "file: ";
    std::cin >> filename;

    fin.open(filename);

    if (fin.is_open()) {
        std::unordered_map<size_t, std::vector<std::pair<std::string, bool>>> map;
        std::vector<std::string> dnf;
        std::string str;

        while (fin >> str) {
            map[get_weight(str)].push_back({str, true});
            dnf.push_back(str);
            str = "";
        }

        bool again = false;
        std::unordered_set<std::string> neccesary_miniterms{};

        do {
            again = false;
            std::unordered_map<size_t, std::vector<std::pair<std::string, bool>>> next_map;

            for (size_t i = 0; i < n; ++i) {
                if (map.find(i) == map.end()) continue;

                for (auto& el : map[i]) {

                    if (map.find(i+1) != map.end()) {
                        for (size_t j = 0; j < map[i+1].size(); ++j) {
                            std::string str1 = map[i + 1][j].first;
                            std::string str2 = el.first;
                            size_t diff = 0;
                            size_t pos = 0;

                            for (size_t k = 0; k < n; ++k) {
                                if (str2[k] == str1[k] || (k == n - 1 && diff == 0)) {
                                    if (k == n - 1) {
                                        if (diff == 1) {
                                            str2[pos] = '~';
                                        }
                                        else {
                                            str2[n - 1] = '~';
                                        }

                                        next_map[get_weight(str2)].push_back({str2, true});
                                        el.second = false;
                                        map[i+1][j].second = false;

                                        again = true;
                                    }
                                }
                                else if (diff == 0) {
                                    pos = k;
                                    ++diff;
                                }
                                else {
                                    break;
                                }
                            }
                        }
                    }

                    if (el.second) {
                        neccesary_miniterms.insert(el.first);
                    }

                }
            }

            map = next_map;
        }
        while (again);

        /*for (auto& i : neccesary_miniterms) {
            std::cout << i << std::endl;
        }*/

        write_table(n, neccesary_miniterms, dnf);

        fin.close();
    }
}

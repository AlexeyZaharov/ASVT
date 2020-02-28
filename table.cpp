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

bool is_equal(const std::string& miniterm, const std::string& dnf) {
    bool success = true;

    for (size_t i = 0; i < miniterm.size(); ++i) {
        if (miniterm[i] != '~' && miniterm[i] != dnf[i]) {
            success = false;
            break;
        }
    }

    return success;
}

void write_table(size_t n, const std::unordered_set<std::string>& neccesary_miniterms, const std::unordered_set<std::string>& dnf) {
    std::cout << std::endl << std::setw((n + 1)*(dnf.size() + 1)) << std::setfill('-') << ' ' << std::endl << std::setfill(' ');

    std::cout << std::setw(n) << ' ' << '|';

    for (auto& value : dnf) {
        std::cout << std::setw(n) << value << '|';
    }

    std::cout << std::endl << std::setw((n + 1)*(dnf.size() + 1)) << std::setfill('-') << ' ' << std::endl << std::setfill(' ');

    for (auto& el : neccesary_miniterms) {
        std::cout << std::setw(n) << el << '|';
        for (auto& value : dnf) {
            std::cout << std::setw(n) << (is_equal(el, value) ? '*' : ' ') << '|';
        }
        std::cout << std::endl << std::setw((n + 1)*(dnf.size() + 1)) << std::setfill('-') << ' ' << std::endl << std::setfill(' ');
    }
}

void get_solution(size_t n, std::unordered_set<std::string>& neccesary_miniterms, std::unordered_set<std::string>& dnf) {
    std::unordered_set<std::string> solution{};

    std::unordered_map<std::string, std::vector<std::string>> dnf_miniterm_map{};
    std::unordered_map<std::string, std::vector<std::string>> miniterm_dnf_map{};

    write_table(n, neccesary_miniterms, dnf);

    for (auto& el : neccesary_miniterms) {
        for (auto& value : dnf) {
            if (is_equal(el, value)) {
                dnf_miniterm_map[value].push_back(el);
                miniterm_dnf_map[el].push_back(value);
            }
        }
    }

    std::vector<std::string> unneccesary_dnf_values{};
    for (auto& value : dnf_miniterm_map) {
        if (value.second.size() == 1) {
            solution.insert(value.second[0]);
            for (auto& key : miniterm_dnf_map[value.second[0]]) {
                unneccesary_dnf_values.push_back(key);
            }
        }
    }

    for (auto& value : unneccesary_dnf_values) {
        if (dnf.find(value) != dnf.end()) {
            dnf.erase(dnf.find(value));
        }
    }

    for (auto& value : solution) {
        neccesary_miniterms.erase(neccesary_miniterms.find(value));
    }

    std::unordered_map<std::string, std::vector<std::string>> unused_miniterm{};
    for (auto& el : neccesary_miniterms) {
        for (auto& value : dnf) {
            if (!is_equal(el, value)) {
                unused_miniterm[el].push_back(value);
            }
        }
    }

    for (auto& miniterm : unused_miniterm) {
        if (miniterm.second.size() == dnf.size()) {
            neccesary_miniterms.erase(neccesary_miniterms.find(miniterm.first));
        }
    }

    std::cout << "Neccesary miniterms:" << std::endl;
    for (auto& value : solution) {
        std::cout << value << ' ';
    }

    std::cout << std:: endl << "Choose others neccesary solution:" << std::endl;
    write_table(n, neccesary_miniterms, dnf);
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
        std::unordered_set<std::string> dnf;
        std::string str;

        while (fin >> str) {
            map[get_weight(str)].push_back({str, true});
            dnf.insert(str);
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

        get_solution(n, neccesary_miniterms, dnf);

        fin.close();
    }
}

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <sstream>

size_t get_weight(const std::string& str) {
    size_t weight = 0;

    for (auto& ch : str) {
        if (ch == '1') {
            ++weight;
        }
    }

    return weight;
}

bool is_equal(const std::string& miniterm, const std::string& function_positive_value) {
    bool success = true;

    for (size_t i = 0; i < miniterm.size(); ++i) {
        if (miniterm[i] != '~' && miniterm[i] != function_positive_value[i]) {
            success = false;
            break;
        }
    }

    return success;
}

void write_table(size_t n, const std::unordered_set<std::string>& neccesary_miniterms, const std::unordered_set<std::string>& function_positive_value) {
    std::cout << std::endl << std::setw((2 * n + 1)*(function_positive_value.size() + 1)) << std::setfill('-') << ' ' << std::endl << std::setfill(' ');

    std::cout << std::setw(2 * n) << ' ' << '|';

    for (auto& value : function_positive_value) {
        std::cout << std::setw(2 * n) << value << '|';
    }

    std::cout << std::endl << std::setw((2 * n + 1)*(function_positive_value.size() + 1)) << std::setfill('-') << ' ' << std::endl << std::setfill(' ');

    for (auto& miniterm : neccesary_miniterms) {
        std::ostringstream out{};

        out << '{';
        for (int i = 0; i < miniterm.size(); ++i) {
            if (miniterm[n - 1 - i] == '0') {
                out << '-' << i;
            }
            else if (miniterm[n - 1 - i] == '1') {
                out << '+' << i;
            }
        }
        out << "}";

        std::cout << std::setw(2 * n) << out.str() << '|';
        for (auto& value : function_positive_value) {
            std::cout << std::setw(2 * n) << (is_equal(miniterm, value) ? '*' : ' ') << '|';
        }
        std::cout << std::endl << std::setw((2 * n + 1)*(function_positive_value.size() + 1)) << std::setfill('-') << ' ' << std::endl << std::setfill(' ');
    }
}

void get_solution(size_t n, std::unordered_set<std::string>& miniterms,
                  std::unordered_set<std::string>& function_positive_value) {

    std::unordered_set<std::string> neccesary_part_of_solution{};

    std::unordered_map<std::string, std::vector<std::string>> function_positive_value_miniterm_map{};
    std::unordered_map<std::string, std::vector<std::string>> miniterm_dnf_map{};

    write_table(n, miniterms, function_positive_value);

    for (auto& el : miniterms) {
        for (auto& value : function_positive_value) {
            if (is_equal(el, value)) {
                function_positive_value_miniterm_map[value].push_back(el);
                miniterm_dnf_map[el].push_back(value);
            }
        }
    }

    std::vector<std::string> unneccesary_function_positive_values{};
    for (auto& value : function_positive_value_miniterm_map) {
        if (value.second.size() == 1) {
            neccesary_part_of_solution.insert(value.second[0]);
            for (auto& key : miniterm_dnf_map[value.second[0]]) {
                unneccesary_function_positive_values.push_back(key);
            }
        }
    }

    for (auto& value : unneccesary_function_positive_values) {
        if (function_positive_value.find(value) != function_positive_value.end()) {
            function_positive_value.erase(function_positive_value.find(value));
        }
    }

    for (auto& value : neccesary_part_of_solution) {
        miniterms.erase(miniterms.find(value));
    }

    std::unordered_map<std::string, std::vector<std::string>> unused_miniterm{};
    for (auto& el : miniterms) {
        for (auto& value : function_positive_value) {
            if (!is_equal(el, value)) {
                unused_miniterm[el].push_back(value);
            }
        }
    }

    for (auto& miniterm : unused_miniterm) {
        if (miniterm.second.size() == function_positive_value.size()) {
            miniterms.erase(miniterms.find(miniterm.first));
        }
    }

    std::cout << "Neccesary miniterms:" << std::endl;
    for (auto& value : neccesary_part_of_solution) {
        std::ostringstream out{};
        out << '{';
        for (int i = 0; i < value.size(); ++i) {
            if (value[n - 1 - i] == '0') {
                out << '-' << i;
            }
            else if (value[n - 1 - i] == '1') {
                out << '+' << i;
            }
        }
        out << "}";

        std::cout << out.str() << ' ';
    }

    std::cout << std:: endl << "Choose others neccesary solution:" << std::endl;
    write_table(n, miniterms, function_positive_value);
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
        std::unordered_map<size_t, std::vector<std::pair<std::string, bool>>> map_of_miniterms;
        std::unordered_set<std::string> function_positive_values;
        std::string str;

        while (fin >> str) {
            map_of_miniterms[get_weight(str)].push_back({str, true});
            function_positive_values.insert(str);
            str = "";
        }

        std::unordered_set<std::string> neccesary_miniterms{};

        do {
            std::unordered_map<size_t, std::vector<std::pair<std::string, bool>>> next_map_of_miniterms;

            for (size_t i = 0; i < n; ++i) {
                if (map_of_miniterms.find(i) == map_of_miniterms.end()) continue;

                for (auto& el : map_of_miniterms[i]) {

                    if (map_of_miniterms.find(i + 1) != map_of_miniterms.end()) {
                        for (size_t j = 0; j < map_of_miniterms[i + 1].size(); ++j) {
                            std::string str1 = map_of_miniterms[i + 1][j].first;
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

                                        next_map_of_miniterms[get_weight(str2)].push_back({str2, true});
                                        el.second = false;
                                        map_of_miniterms[i + 1][j].second = false;
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

            map_of_miniterms = next_map_of_miniterms;
        }
        while (!map_of_miniterms.empty());

        get_solution(n, neccesary_miniterms, function_positive_values);

        fin.close();
    }
}

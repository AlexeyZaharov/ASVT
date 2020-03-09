#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <sstream>

std::vector<std::string> get_solution(std::unordered_map<std::string, size_t>& koef_times, std::unordered_map<std::string, std::unordered_set<std::string>>& equation_koef) {
    std::vector<std::string> solution;

    while(!equation_koef.empty()) {
        std::pair<std::string, size_t> max_coef{"---------------------------", 0};

        for (auto& koef : koef_times) {
            if (koef.second > max_coef.second || koef.second == max_coef.second && koef.first.size() < max_coef.first.size()) {
                max_coef = koef;
            }
        }

        solution.push_back(max_coef.first);

        std::vector<std::string> delete_equations;
        for (auto& equation : equation_koef) {
            if (equation.second.find(max_coef.first) != equation.second.end()) {
                delete_equations.push_back(equation.first);
            }
        }

        for(auto& equation : delete_equations) {
            for (auto& koef : equation_koef[equation]) {
                --koef_times[koef];
            }
            equation_koef.erase(equation_koef.find(equation));
        }
    }

    return solution;
}

int main() {
    int n = 6;
    std::ifstream fin{};
    std::string filename;

    std::cout << "file: ";
    std::cin >> filename;

    fin.open(filename);

    if (fin.is_open()) {
        std::unordered_set<std::string> function_positive_value;
        std::string str;

        while (fin >> str) {
            function_positive_value.insert(str);
            str = "";
        }

        std::vector<std::unordered_set<int>> set_of_koef{};
        for (int i = 0; i < n; ++i) {
            for (int j = i; j < n; ++j) {
                for (int k = j; k < n; ++k) {
                    for (int l = k; l < n; ++l) {
                        for (int m = l; m < n; ++m) {
                            for (int p = m; p < n; ++p) {
                                std::unordered_set<int> set_of_number{};
                                set_of_number.insert(i);
                                set_of_number.insert(j);
                                set_of_number.insert(k);
                                set_of_number.insert(l);
                                set_of_number.insert(m);
                                set_of_number.insert(p);

                                set_of_koef.push_back(set_of_number);
                            }
                        }
                    }
                }
            }
        }

        std::vector<std::unordered_set<int>> unique_set_of_koef{};
        for (int i = 0; i < set_of_koef.size(); ++i) {
            bool is_add = true;
            for (int j = 0; j < unique_set_of_koef.size(); ++j) {
                if (set_of_koef[i] == unique_set_of_koef[j]) {
                    is_add = false;
                    break;
                }
            }
            if (is_add) {
                unique_set_of_koef.push_back(set_of_koef[i]);
            }
        }

        std::vector<std::bitset<6>> integer_function_positive_values;
        for (size_t i = 0; i < 64; ++i) {
            integer_function_positive_values.emplace_back(std::bitset<6>{i});
        }

        std::unordered_set<std::string> set_of_bad_monoms;

        for (auto& function_value : integer_function_positive_values) {
            if (function_positive_value.find(function_value.to_string()) != function_positive_value.end()) {
                continue;
            }
            for (auto& koef : unique_set_of_koef) {
                std::string str_function_value = function_value.to_string();
                std::ostringstream out;

                out << '{';
                for (auto& i : koef) {
                    if (str_function_value[5 - i] == '0') {
                        out << '-' << i;
                    }
                    else {
                        out << '+' << i;
                    }
                }
                out << "}";

                set_of_bad_monoms.insert(out.str());
            }
        }

        std::cout << "System of equation:" << std::endl;

        std::unordered_map<std::string, std::unordered_set<std::string>> equation_koef;
        std::unordered_map<std::string, size_t> koef_times;

        for (auto& function_value : integer_function_positive_values) {
            if (function_positive_value.find(function_value.to_string()) == function_positive_value.end()) {
                continue;
            }

            std::string str_function_value = function_value.to_string();
            std::string one_equation = {};

            for (auto& koef : unique_set_of_koef) {
                std::ostringstream out;
                out << '{';
                for (auto &i : koef) {
                    if (str_function_value[5 - i] == '0') {
                        out << '-' << i;
                    } else {
                        out << '+' << i;
                    }
                }
                out << "}";

                if (set_of_bad_monoms.find(out.str()) == set_of_bad_monoms.end()) {
                    one_equation += out.str() + " + ";

                    equation_koef[str_function_value].insert(out.str());
                    ++koef_times[out.str()];
                }
            }

            std::cout << std::string(one_equation.begin(), one_equation.end() - 2) << "= " << str_function_value << std::endl;
        }

        auto solution = get_solution(koef_times, equation_koef);

        std::cout << std::endl << "Solution is:" << std::endl;
        for (auto& koef : solution) {
            std::cout << koef << ' ';
        }

        fin.close();
    }
}

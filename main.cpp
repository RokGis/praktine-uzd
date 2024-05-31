#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>
#include <regex>

void toLowerCase(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

std::string cleanWord(const std::string& word) {
    std::string clean_word;
    std::copy_if(word.begin(), word.end(), std::back_inserter(clean_word), [](char c) {
        return std::isalnum(c) || c == '-' || c == '_';
    });
    return clean_word;
}

bool isURL(const std::string &word) {
    const std::regex url_regex(R"((http://|https://|www\.|[a-zA-Z0-9.-]+\.(com|org|net|edu|gov|io|co|lt)))");
    return std::regex_search(word, url_regex);
}

void countWords(const std::string& text, std::map<std::string, int>& wordCount, std::map<std::string, std::set<int>>& wordLines,  std::map<std::string, int>& urlCount, std::map<std::string, std::set<int>> urls) {
    std::istringstream iss(text);
    std::string word;
    int line_number = 1;

    while (iss >> word) {
        if (word == "\n") {
            line_number++;
        }
        word = cleanWord(word);
        if (!word.empty()) {
            if(isURL(word))
            {
                urlCount[word]++;
                urls[word].insert(line_number);
            }
            toLowerCase(word);
            wordCount[word]++;
            wordLines[word].insert(line_number);
        }
    }
}

void findURLs(const std::string& text, std::set<std::string>& urls) {
    std::regex url_regex(R"((http://|https://|www\.|[a-zA-Z0-9.-]+\.(com|org|net|edu|gov|io|co|lt)))");
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), url_regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        urls.insert(match.str());
    }
}

int main() {
    std::ifstream inputFile("Untitled.txt");
    std::ofstream outputWordCount("word_count.txt");
    std::ofstream outputWordLocations("word_locations.txt");
    std::ofstream outputURLs("urls.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    std::string text((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    std::map<std::string, int> wordCount;
    std::map<std::string, std::set<int>> wordLines;
    std::map<std::string, int> urlCount;
    std::map<std::string, std::set<int>> urls;

    countWords(text, wordCount, wordLines, urlCount, urls);
    //findURLs(text, urls);

    for (const auto& [word, count] : wordCount) {
        if (count > 1) {
            outputWordCount << word << ": " << count << "\n";
        }
    }

    for (const auto& [word, lines] : wordLines) {
        if (lines.size() > 1) {
            outputWordLocations << word << ": ";
            for (int line : lines) {
                outputWordLocations << line << " ";
            }
            outputWordLocations << "\n";
        }
    }

    for (const auto &entry : urlCount) {
        outputURLs << entry.urls << "\n";
    }

    inputFile.close();
    outputWordCount.close();
    outputWordLocations.close();
    outputURLs.close();

    return 0;
}
